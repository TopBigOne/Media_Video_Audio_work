//
// Created by dev on 2023/2/21.
//

#include "LogUtil.h"
#include "AudioDecoder.h"


void AudioDecoder::OnDecoderReady() {
    if (m_AudioRender) {
        AVCodecContext *codeCtx = GetCodecContext();
        m_SwrContext = swr_alloc();

        av_opt_set_int(m_SwrContext, "in_channel_layout", codeCtx->channel_layout, 0);
        av_opt_set_int(m_SwrContext, "out_channel_layout", AUDIO_DST_CHANNEL_LAYOUT, 0);

        av_opt_set_int(m_SwrContext, "in_sample_rate", codeCtx->sample_rate, 0);
        av_opt_set_int(m_SwrContext, "out_sample_rate", AUDIO_DST_SAMPLE_RATE, 0);

        av_opt_set_int(m_SwrContext, "in_sample_fmt", codeCtx->sample_fmt, 0);
        av_opt_set_int(m_SwrContext, "out_sample_fmt", DST_SAMPLE_FORMAT, 0);

        swr_init(m_SwrContext);
        LOGCATE("AudioDecoder::OnDecoderReady audio metadata sample rate: %d, channel: %d, format: %d, frame_size: %d, layout: %lld",
                codeCtx->sample_rate, codeCtx->channels, codeCtx->sample_fmt, codeCtx->frame_size,
                codeCtx->channel_layout);

        m_nbSamples = (int) av_rescale_rnd(ACC_NB_SAMPLES, AUDIO_DST_SAMPLE_RATE,
                                           codeCtx->sample_rate, AV_ROUND_UP);
        m_DstFrameDataSize = av_samples_get_buffer_size(nullptr, AUDIO_DST_CHANNEL_COUNTS,
                                                        m_nbSamples,
                                                        DST_SAMPLE_FORMAT, 1);
        m_AudioOutBuffer = (uint8_t *) malloc(m_DstFrameDataSize);
        // audio render 初始化
        m_AudioRender->Init();
    }

}

void AudioDecoder::OnFrameAvailable(AVFrame *frame) {
    LOGCATI("AudioDecoder::OnFrameAvailable");
    if (m_AudioRender == nullptr) {
        return;
    }
    int result = swr_convert(m_SwrContext, &m_AudioOutBuffer, m_DstFrameDataSize / 2,
                             (const uint8_t **) frame->data, frame->nb_samples);
    if (result > 0) {
        // 触发音频绘制
        m_AudioRender->RenderAudioFrame(m_AudioOutBuffer, m_DstFrameDataSize);
    } else{
        LOGCATE("AudioDecoder::OnFrameAvailable : swr_convert in ERROR");
    }

}

void AudioDecoder::OnDecoderDone() {
    if (m_AudioRender) {
        m_AudioRender->UnInit();
    }
    if (m_AudioOutBuffer) {
        // 广亚，一定要记住，，，free啊，，，
        free(m_AudioOutBuffer);
        m_AudioOutBuffer = nullptr;
    }
    if (m_SwrContext) {
        swr_free(&m_SwrContext);
        m_SwrContext = nullptr;
    }
}

void AudioDecoder::ClearCache() {
    if (m_AudioRender){
        m_AudioRender->ClearAudioCache();
    }

}
