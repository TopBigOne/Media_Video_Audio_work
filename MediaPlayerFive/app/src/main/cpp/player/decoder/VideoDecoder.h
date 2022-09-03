//
// Created by dev on 2022/9/3.
//

#ifndef MEDIAPLAYERFIVE_VIDEODECODER_H
#define MEDIAPLAYERFIVE_VIDEODECODER_H


extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/jni.h>
};

#include "../render/video/VideoRender.h"
#include "../render/audio/AudioRender.h"
#include "../../recorder/SingleVideoRecorder.h"
#include "DecoderBase.h"

class VideoDecoder : public DecoderBase {

public:
    VideoDecoder(char *url) {
        Init(url, AVMEDIA_TYPE_VIDEO);
    }

    virtual ~VideoDecoder() {
        UnInit();
    }

    int GetVideoWidth() {
        return m_VideoWidth;
    }

    int GetVideoHeight() {
        return m_VideoHeight;
    }

    void SetVideoRender(VideoRender *videoRender) {
        m_VideoRender = videoRender;
    }

private:
    virtual void OnDecoderReady();

    virtual void OnDecoderDone();

    virtual void OnFrameAvailable(AVFrame *frame);

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;

    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame *m_RGBAFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;

    VideoRender *m_VideoRender = nullptr;
    SwsContext *m_SwsContext = nullptr;
    SingleVideoRecorder *m_pVideoRecorder = nullptr;
};


#endif //MEDIAPLAYERFIVE_VIDEODECODER_H
