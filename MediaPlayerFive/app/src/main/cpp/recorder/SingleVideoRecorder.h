//
// Created by dev on 2022/9/3.
//

#ifndef MEDIAPLAYERFIVE_SINGLEVIDEORECORDER_H
#define MEDIAPLAYERFIVE_SINGLEVIDEORECORDER_H


extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "../../../common/ThreadSafeQueue.h"
#include "../../../util/ImageDef.h"
#include "../../../util/LogUtil.h"
#include "thread"

using namespace std;

class SingleVideoRecorder {
public:
    SingleVideoRecorder(const char *outUrl, int frameWidth, int frameHeight, long bitRate, int fps);

    ~SingleVideoRecorder();

    int StartRecord();

    int OnFrame2Encode(NativeImage *inputFrame);

    int StopRecord();

private:
    static void StartH264EncoderThread(SingleVideoRecorder *context);

    int EncodeFrame(AVFrame *pFrame);

private:
    ThreadSafeQueue<NativeImage *> m_frameQueue;
    char m_outUrl[1024] = {0};
    int m_frameWidth;
    int m_frameHeight;
    int m_frameIndex = 0;
    long m_bitRate;
    int m_frameRate;
    AVPacket m_avPacket;
    AVFrame *m_pFrame = nullptr;
    uint8_t *m_pFrameBuffer = nullptr;
    AVCodec *m_pCodec = nullptr;
    AVStream *m_pStream = nullptr;
    AVCodecContext *m_pCodecCtx = nullptr;
    AVFormatContext *m_pFormatCtx = nullptr;
    thread *m_encodeThread = nullptr;
    SwsContext *m_SwsContext = nullptr;
    volatile int m_exit = 0;
};


#endif //MEDIAPLAYERFIVE_SINGLEVIDEORECORDER_H
