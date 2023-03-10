//
// Created by dev on 2023/2/21.
//

#ifndef LEARNFFMPEG_AUDIODECODER_H
#define LEARNFFMPEG_AUDIODECODER_H

extern "C" {
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
}

#include "AudioRender.h"
#include "Decoder.h"
#include "DecoderBase.h"

static const int AUDIO_DST_SAMPLE_RATE = 44100;
static const int AUDIO_DST_CHANNEL_COUNTS = 2;
static const uint64_t AUDIO_DST_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
static const int AUDIO_DST_BIT_RATE = 64000;
static const int ACC_NB_SAMPLES = 1024;

class AudioDecoder : public DecoderBase {
public:
    AudioDecoder(char *url) {
        Init(url, AVMEDIA_TYPE_AUDIO);
    }

    virtual ~AudioDecoder() {
        UnInit();
    }

    void setAudioRender(AudioRender *audioRender) {
        m_AudioRender = audioRender;
    }

private:
    virtual void OnDecoderReady();

    virtual void OnDecoderDone();

    virtual void OnFrameAvailable(AVFrame *frame);

    virtual void ClearCache();

    const AVSampleFormat DST_SAMPLE_FORMAT = AV_SAMPLE_FMT_S16;
    AudioRender *m_AudioRender = nullptr;
    SwrContext *m_SwrContext = nullptr;
    uint8_t *m_AudioOutBuffer = nullptr;
    // number of sample per channel;
    int m_nbSamples = 0;
    // dst frame data size
    // todo dst 是什么意思？ 目标的意思吗？
    int m_DstFrameDataSize = 0;


};


#endif //LEARNFFMPEG_AUDIODECODER_H
