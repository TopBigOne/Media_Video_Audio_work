//
// Created by dev on 2023/2/21.
//

#include "VideoDecoder.h"

void VideoDecoder::OnDecoderReady() {
    m_VideoWidth = GetCodecContext()->width;
    m_VideoHeight = GetCodecContext()->height;
    if (m_MsgContext && m_MsgCallback) {
        m_MsgCallback(m_MsgContext, MSG_DECODER_READY, 0);
    }

    // case 1:
    if (m_VideoRender == nullptr) {
        LOGCATE("VideoDecoder::OnDecoderReady m_VideoRender == null");
        return;
    }

    // case 2:
    int dstSize[2] = {0};
    // 绘制： 初始化
    m_VideoRender->Init(m_VideoWidth, m_VideoHeight, dstSize);
    m_RenderWidth = dstSize[0];
    m_RenderHeight = dstSize[1];
    // 原生绘制

//        if (m_VideoRender->GetRenderType() == VIDEO_RENDER_ANWINDOW) {
//            int fps = 25;
//            long videoBitRate = m_RenderWidth * m_RenderHeight * fps * 0.2;
//            m_pVideoRecorder = new SingleVideoRecorder("/sdcard/learnffmpeg_output.mp4",
//                                                       m_RenderWidth, m_RenderHeight, videoBitRate,
//                                                       fps);
//            m_pVideoRecorder->StartRecord();
//        }
    m_RGBAFrame = av_frame_alloc();
    int bufferSize = av_image_get_buffer_size(DST_PIXEL_FORMAT, m_RenderWidth, m_RenderHeight, 1);
    m_FrameBuffer = static_cast<uint8_t *>(av_malloc(bufferSize * sizeof(uint8_t)));
    av_image_fill_arrays(m_RGBAFrame->data,
                         m_RGBAFrame->linesize,
                         m_FrameBuffer, DST_PIXEL_FORMAT,
                         m_RenderWidth, m_RenderHeight, 1);

    m_SwsContext = sws_getContext(
            m_VideoWidth,
            m_VideoHeight,
            GetCodecContext()->pix_fmt,
            m_RenderWidth,
            m_RenderHeight,
            DST_PIXEL_FORMAT,
            SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

}

void VideoDecoder::OnDecoderDone() {
    if (m_MsgContext && m_MsgCallback)
        m_MsgCallback(m_MsgContext, MSG_DECODER_DONE, 0);

    if (m_VideoRender)
        m_VideoRender->UnInit();

    if (m_RGBAFrame != nullptr) {
        av_frame_free(&m_RGBAFrame);
        m_RGBAFrame = nullptr;
    }

    if (m_FrameBuffer != nullptr) {
        free(m_FrameBuffer);
        m_FrameBuffer = nullptr;
    }

    if (m_SwsContext != nullptr) {
        sws_freeContext(m_SwsContext);
        m_SwsContext = nullptr;
    }

    // todo free the VideoRecoder;


}

void VideoDecoder::OnFrameAvailable(AVFrame *frame) {
    LOGCATI("VideoDecoder::OnFrameAvailable frame=%p", frame);
    AVPixelFormat pix_fmt = GetCodecContext()->pix_fmt;
    if (m_VideoRender != nullptr && frame != nullptr) {
        LOGCATI("VideoDecoder::OnFrameAvailable frame[w,h]=[%d, %d],format=%d,[line0,line1,line2]=[%d, %d, %d]",
                frame->width, frame->height, GetCodecContext()->pix_fmt, frame->linesize[0],
                frame->linesize[1], frame->linesize[2]);
        NativeImage image;
        int renderType = m_VideoRender->GetRenderType();
        if (renderType == VIDEO_RENDER_ANWINDOW) {
            sws_scale(m_SwsContext, frame->data, frame->linesize, 0,
                      m_VideoHeight, m_RGBAFrame->data, m_RGBAFrame->linesize);

            image.format = IMAGE_FORMAT_RGBA;
            image.width = m_RenderWidth;
            image.height = m_RenderHeight;
            image.ppPlane[0] = m_RGBAFrame->data[0];
            image.pLineSize[0] = image.width * 4;
        } else if (pix_fmt == AV_PIX_FMT_YUV420P || pix_fmt == AV_PIX_FMT_YUVJ420P) {
            // todo...


        } else if (pix_fmt == AV_PIX_FMT_NV12) {
            // todo...

        } else if (pix_fmt == AV_PIX_FMT_NV21) {
            // todo...

        } else if (pix_fmt == AV_PIX_FMT_RGBA) {
            // todo...

        } else {
            // todo...

        }


        m_VideoRender->RenderVideoFrame(&image);
        // todo....
        /*
         *
         *  if(m_pVideoRecorder != nullptr) {
            m_pVideoRecorder->OnFrame2Encode(&image);
        }
         */

    }
    if (m_MsgContext && m_MsgCallback) {
        m_MsgCallback(m_MsgContext, MSG_REQUEST_RENDER, 0);
    }


}
