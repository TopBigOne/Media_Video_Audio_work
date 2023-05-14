#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "librtmp/rtmp.h"


#define FLV_PATH "../av_file/cuc_ieschool.flv"
#define RTMP_ULR "rtmp://localhost/live/inrtmp"
// 1 hour
#define ONE_HOUR (3600*1000)

#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

#define HTON16(x)  ((x>>8&0xff)|(x<<8&0xff00))
#define HTON24(x)  ((x>>16&0xff)|(x<<16&0xff0000)|(x&0xff00))
#define HTON32(x)  ((x>>24&0xff)|(x>>8&0xff00)|\
    (x<<8&0xff0000)|(x<<24&0xff000000))
#define HTONTIME(x) ((x>>16&0xff)|(x<<16&0xff0000)|(x&0xff00)|(x&0xff000000))

typedef enum ByteType {
    ONT_BYTE,
    TWO_BYTE,
    THREE_BYTE,
    FOUR_BYTE
} BYTE_TYPE;


int readByte(BYTE_TYPE type, uint32_t *block, FILE *fp) {
    int size = 1;
    if (type == ONT_BYTE) {
        size = 1;
    } else if (type == TWO_BYTE) {
        size = 2;
    } else if (type == THREE_BYTE) {
        size = 3;
    } else if (type == FOUR_BYTE) {
        size = 4;
    }

    if (fread(block, size, 1, fp) != 1) {
        return 0;
    }
    if (type == TWO_BYTE) {
        *block = HTON16(*block);
    } else if (type == THREE_BYTE) {
        *block = HTON24(*block);
    } else if (type == FOUR_BYTE) {
        *block = HTON32(*block);
    }
    return 1;
}

int peekU8(uint32_t *u8, FILE *fp) {
    if (fread(u8, 1, 1, fp) != 1) {
        return 0;
    }
    fseek(fp, -1, SEEK_CUR);
    return 1;
}

/**
 * read 4 byte and convert to time format
 * @param u_time
 * @param fp
 * @return
 */
int read_time(uint32_t *u_time, FILE *fp) {

    if (readByte(FOUR_BYTE, u_time, fp) != 1) {
        return 0;
    }
    *u_time = HTONTIME(*u_time);
    return 1;
}

int publish_using_packet() {
    RTMP       *rtmp          = NULL;
    RTMPPacket *packet        = NULL;
    uint32_t   start_time     = NULL;
    uint32_t   now_time       = NULL;
    // the timestamp of the previous frame
    long       pre_frame_time = 0;
    long       last_time      = 0;
    int        b_next_is_key  = 1;
    uint32_t   pre_tag_size   = 0;

    // packet attributes
    uint32_t type        = 0; // audio type or video type.
    uint32_t data_length = 0;
    uint32_t time_stamp  = 0;
    uint32_t stream_id   = 0;

    FILE *flv_fp = NULL;
    flv_fp = fopen(FLV_PATH, "rb");
    if (NULL == flv_fp) {
        perror("Open File Error");
        return -1;
    }

    rtmp = RTMP_Alloc();
    rtmp->Link.timeout = 5;
    if (!RTMP_SetupURL(rtmp, RTMP_ULR)) {
        RTMP_Log(RTMP_LOGERROR, "connect error.\n");
        RTMP_Free(rtmp);
        return -1;
    }
    if (!RTMP_Connect(rtmp, 0)) {
        RTMP_Log(RTMP_LOGERROR, "connect stream error.\n");
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        return -1;
    }
    packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, 1024 * 64);
    RTMPPacket_Reset(packet);
    packet->m_hasAbsTimestamp = 0;
    packet->m_nChannel        = 0x04;
    packet->m_nInfoField2     = rtmp->m_stream_id;
    puts("start to send data.");

    // | flv head 9byte | previousTag 4byte | meta data | previous tag size | video tag  | previous tag size | audio tag
    // | 0x06 : audio | previousTag 4byte | meta data | previous tag size | video tag  | previous tag size | audio tag
    // | 0x08 : video | previousTag 4byte | meta data | previous tag size | video tag  | previous tag size | audio tag
    // jump over flv header
    fseek(flv_fp, 9, SEEK_SET); //seek_set :
    // jump over  previous tag size
    fseek(flv_fp, 4, SEEK_CUR);

    start_time = RTMP_GetTime();
    while (1) {
        if ((((now_time - RTMP_GetTime()) - start_time) < pre_frame_time) && b_next_is_key) {
            // wait for 1 sec
            // if the send process is too fast
            // this mechanism is not very good ,need some improvement.
            if (pre_frame_time > last_time) {
                RTMP_LogPrintf("TimeStamp:%8lu ms\n", pre_frame_time);
                last_time = pre_frame_time;
            }
            sleep(1);
            continue;
        }

        // not quite the same as FLV spec.
        if (!readByte(ONT_BYTE, &type, flv_fp)) {
            break;
        }

        if (!readByte(THREE_BYTE, &data_length, flv_fp)) {
            break;
        }

        if (read_time(&time_stamp, flv_fp) != 1) {
            break;
        }

        if (!readByte(THREE_BYTE, &stream_id, flv_fp)) {
            break;
        }

        if (type != 0x08 && type != 0x09) {
            // jump over non_audio and non_video_frame
            // jump over next previousTag Size at the same time.
            fseek(flv_fp, data_length + 4, SEEK_CUR);
            continue;
        }
        // todo data length 是 flv的文件大小吗？
        if (fread(packet->m_body, 1, data_length, flv_fp) != data_length) {
            break;
        }

        packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
        packet->m_nTimeStamp = time_stamp;
        packet->m_packetType = type;
        packet->m_nBodySize  = data_length,
                pre_frame_time = time_stamp;
        if (!RTMP_IsConnected(rtmp)) {
            RTMP_Log(RTMP_LOGERROR, "rtmp is not connect\n");
            break;
        }
        // 0 or 1: 将其保留在呼叫队列中，直到结果到达
        if (!RTMP_SendPacket(rtmp, packet, 0)) {
            RTMP_Log(RTMP_LOGERROR, "Send Error\n");
            break;
        }
        if (!readByte(FOUR_BYTE, &pre_tag_size, flv_fp)) {
            break;
        }
        if (!peekU8(&type, flv_fp)) {
            break;
        }
        if (type == 0x09) {
            if (fseek(flv_fp, 11, SEEK_CUR) != 0) {
                break;
            }

            if (!peekU8(&type, flv_fp)) {
                break;
            }
            if (type == 0x17) {
                b_next_is_key = 1;
            } else {
                b_next_is_key = 0;
            }
            fseek(flv_fp, -11, SEEK_CUR);
        }

    }

    puts("send data over ");

    if (flv_fp != NULL) {
        fclose(flv_fp);
        flv_fp = NULL;
    }
    if (rtmp != NULL) {
        RTMPPacket_Free(packet);
        free(packet);
        packet = NULL;
    }

    return 0;

}

/**
 * publish using rtmp_write();
 * @return
 */
int publish_using_write() {
    uint32_t start_time     = 0;
    uint32_t now_time       = 0;
    uint32_t pre_frame_time = 0;
    uint32_t last_time      = 0;
    int      b_next_is_key  = 0;
    char     *p_file_buf    = NULL;

    // read from tag header
    uint32_t type        = 0;
    uint32_t data_length = 0;
    uint32_t timestamp   = 0;
    RTMP     *rtmp       = NULL;
    FILE     *fp         = NULL;
    fp = fopen(FLV_PATH, "rb");
    if (NULL == fp) {
        RTMP_LogPrintf("Open File Error.\n");
        return -1;
    }
    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    rtmp->Link.timeout = 5;
    if (!RTMP_SetupURL(rtmp, RTMP_ULR)) {
        RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
        RTMP_Free(rtmp);
        return -1;
    }
    // 开启写模式
    RTMP_EnableWrite(rtmp);
    RTMP_SetBufferMS(rtmp, ONE_HOUR);
    if (!RTMP_Connect(rtmp, NULL)) {
        RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
        RTMP_Free(rtmp);
        return -1;
    }

    if (!RTMP_ConnectStream(rtmp, 0)) {
        RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        return -1;
    }
    puts("start to send data.");
    // jump over FLV header
    fseek(fp, 9, SEEK_SET);
    // jump over previousTagSize0
    fseek(fp, 4, SEEK_CUR);
    start_time = RTMP_GetTime();
    while (1) {
        if (((now_time - RTMP_GetTime() - start_time) < pre_frame_time) && b_next_is_key) {
            if (pre_frame_time < last_time) {
                last_time = pre_frame_time;
            }
            sleep(1);
            continue;
        }
        // jump over type
        fseek(fp, 1, SEEK_CUR);
        if (!readByte(THREE_BYTE, &data_length, fp)) {
            break;
        }
        if (!read_time(&timestamp, fp)) {
            break;
        }
        // jump back
        fseek(fp, -8, SEEK_CUR);
        int buffer_size = 11 + 4 + data_length;
        p_file_buf = (char *) malloc(buffer_size);
        memset(p_file_buf, 0, buffer_size);
        if (fread(p_file_buf, 1, buffer_size, fp) != buffer_size) {
            break;
        }

        pre_frame_time = timestamp;
        if (!RTMP_IsConnected(rtmp)) {
            RTMP_Log(RTMP_LOGERROR, "rtmp is not connect\n");
            break;
        }
        if (!RTMP_Write(rtmp, p_file_buf, buffer_size)) {
            RTMP_Log(RTMP_LOGERROR, "Rtmp Write Error\n");
            break;
        }
        free(p_file_buf);
        p_file_buf = NULL;
        if (!peekU8(&type, fp)) {
            break;
        }
        if (type == 0x09) {
            if (fseek(fp, 11, SEEK_CUR) != 0) {
                break;
            }
            if (!peekU8(&type, fp)) {
                break;
            }
            if (type == 0x17) {
                b_next_is_key = 1;
            } else {
                b_next_is_key = 0;
            }
        }


    }
    RTMP_LogPrintf("\nSend Data Over\n");
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    if (rtmp != NULL) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        rtmp = NULL;
    }
    if (p_file_buf != NULL) {
        free(p_file_buf);
        p_file_buf = NULL;
    }
    return 0;


}


void test_rtmp();


void test_rtmp() {
    puts("test rtmp.");
    RTMP *rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    if (rtmp == NULL) {
        puts("rtmp 初始化失败.");
        return;
    }

    FILE *fp = NULL;
    fp = fopen(FLV_PATH, "rb");
    if (fp == NULL) {
        perror("open fail.");
        return;
    }
    puts("open OK.");

}

int main() {
    test_rtmp();
    publish_using_packet();
    return 0;
}
