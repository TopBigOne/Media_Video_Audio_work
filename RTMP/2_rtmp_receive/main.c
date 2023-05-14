#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "librtmp/log.h"
#include "librtmp/rtmp.h"

#include "stdbool.h"

#define RTMP_URL "rtmp://ns8.indexforce.com/home/mystream"

int main() {
    int  n_read         = 0;
    bool is_live_stream = 0;
    int  buffer_size    = 1024 * 1024 * 10;
    char *buf           = malloc(buffer_size);
    memset(buf, 0, buffer_size);
    long count_buffer_size = 0;
    FILE *fp               = fopen("../receive.flv", "wb");
    if (fp == NULL) {
        RTMP_LogPrintf("Open File Error.\n");
        return -1;
    }
    int result = -1;

    // step 1:
    RTMP *rtmp = RTMP_Alloc();
    // step 2:
    RTMP_Init(rtmp);
    // step 3: set url
    result = RTMP_SetupURL(rtmp, RTMP_URL);
    if (!result) {
        RTMP_LogPrintf("RTMP_SetupURL Error.\n");
        return -1;
    }
    if (is_live_stream) {
        rtmp->Link.lFlags |= RTMP_LF_LIVE;
    }

    // step 4: set buffer continue time
    RTMP_SetBufferMS(rtmp, 3600 * 1000);

    // step 5: connect
    result = RTMP_ConnectStream(rtmp, 0);
    if (!result) {
        RTMP_LogPrintf("RTMP_ConnectStream Error.\n");
        return -1;
    }
    // step 6: read data by LOOP
    while ((n_read = RTMP_Read(rtmp, buf, buffer_size)) != 0) {
        fwrite(buf, 1, (size_t) n_read, fp);
        count_buffer_size += n_read;
        RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n", n_read, count_buffer_size * 1.0 / 1024);
    }

    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    if (rtmp != NULL) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        rtmp = NULL;
    }


    return 0;
}
