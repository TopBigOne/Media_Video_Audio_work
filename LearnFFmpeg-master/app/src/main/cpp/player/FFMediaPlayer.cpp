//
// Created by dev on 2023/2/18.
//

#include "FFMediaPlayer.h"
#include "OpenSLRender.h"
#include "NativeRender.h"
#include "LogUtil.h"

void
FFMediaPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int videoRenderType, jobject surface) {
    jniEnv->GetJavaVM(&m_JavaVM);
    // 创建全局引用
    m_JavaObj = jniEnv->NewGlobalRef(obj);
    m_VideoDecoder = new VideoDecoder(url);
    m_AudioDecoder = new AudioDecoder(url);
    switch (videoRenderType) {
        case VIDEO_RENDER_OPENGL:
            // todo
            break;
        case VIDEO_RENDER_ANWINDOW:
            m_VideoRender = new NativeRender(jniEnv, surface);
            m_VideoDecoder->setVideoRender(m_VideoRender);
            break;
        case VIDEO_RENDER_3D_VR:
            // todo
            break;
        default:
            break;
    }

    m_AudioRender = new OpenSLRender();
    m_AudioDecoder->setAudioRender(m_AudioRender);

    m_VideoDecoder->SetMessageCallback(this, PostMessage);
    m_AudioDecoder->SetMessageCallback(this, PostMessage);
}

void FFMediaPlayer::UnInit() {
    LOGCATE("FFMediaPlayer::UnInit");
    if (m_VideoDecoder) {
        delete m_VideoDecoder;
        m_VideoDecoder = nullptr;
    }

    if (m_VideoRender) {
        delete m_VideoRender;
        m_VideoRender = nullptr;
    }

    if (m_AudioDecoder) {
        delete m_AudioDecoder;
        m_AudioDecoder = nullptr;
    }

    if (m_AudioRender) {
        delete m_AudioRender;
        m_AudioRender = nullptr;
    }

    // VideoGLRender::ReleaseInstance();

    bool isAttach = false;
    GetJNIEnv(&isAttach)->DeleteGlobalRef(m_JavaObj);
    if (isAttach)
        GetJavaVM()->DetachCurrentThread();

}

void FFMediaPlayer::Play() {
    if (m_VideoDecoder) {
        m_VideoDecoder->Start();
    }
    if (m_AudioDecoder) {
        m_AudioDecoder->Start();
    }

}

void FFMediaPlayer::Pause() {
    LOGCATE("FFMediaPlayer::Pause");
    if (m_VideoDecoder)
        m_VideoDecoder->Pause();

    if (m_AudioDecoder)
        m_AudioDecoder->Pause();

}

void FFMediaPlayer::Stop() {
    LOGCATE("FFMediaPlayer::Stop");
    if (m_VideoDecoder)
        m_VideoDecoder->Stop();

    if (m_AudioDecoder)
        m_AudioDecoder->Stop();

}

void FFMediaPlayer::SeekToPosition(float position) {
    LOGCATE("FFMediaPlayer::SeekToPosition position=%f", position);
    if (m_VideoDecoder)
        m_VideoDecoder->SeekToPosition(position);

    if (m_AudioDecoder)
        m_AudioDecoder->SeekToPosition(position);

}

long FFMediaPlayer::GetMediaParams(int paramType) {
    LOGCATE("FFMediaPlayer::GetMediaParams paramType=%d", paramType);
    long value = 0;
    switch (paramType) {
        case MEDIA_PARAM_VIDEO_WIDTH:
            value = m_VideoDecoder != nullptr ? m_VideoDecoder->GetVideoWidth() : 0;
            break;
        case MEDIA_PARAM_VIDEO_HEIGHT:
            value = m_VideoDecoder != nullptr ? m_VideoDecoder->GetVideoHeight() : 0;
            break;
        case MEDIA_PARAM_VIDEO_DURATION:
            value = m_VideoDecoder != nullptr ? m_VideoDecoder->GetDuration() : 0;
            break;
        default:
            break;
    }
    return value;


}

JNIEnv *FFMediaPlayer::GetJNIEnv(bool *isAttach) {
    JNIEnv *env;
    int status;
    if (m_JavaVM == nullptr) {
        LOGCATE("FFMediaPlayer::GetJNIEnv m_JavaVM == nullptr");
        return nullptr;
    }
    *isAttach = false;
    // todo 易错点
    status = m_JavaVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    if (status != JNI_OK) {
        status = m_JavaVM->AttachCurrentThread(&env, nullptr);
        if (status != JNI_OK) {
            return nullptr;
        }
        *isAttach = true;
    }

    return env;
}

jobject FFMediaPlayer::GetJavaObj() {
    return m_JavaObj;
}

JavaVM *FFMediaPlayer::GetJavaVM() {
    return m_JavaVM;
}

void FFMediaPlayer::PostMessage(void *context, int msgType, float msgCode) {
    if (context == nullptr) {
        return;
    }
    LOGCATD("FFMediaPlayer::PostMessage# msgType: %d,msgCode:%f", msgType, msgCode);
    auto *player = static_cast<FFMediaPlayer *>(context);
    bool isAttach = false;
    JNIEnv *env = player->GetJNIEnv(&isAttach);
    if (env == nullptr) {
        LOGCATE("FFMediaPlayer::PostMessage: the JNIEnv is null.");
        return;
    }
    jobject javaObj = player->GetJavaObj();
    jmethodID mid = env->GetMethodID(env->GetObjectClass(javaObj),
                                     JAVA_PLAYER_EVENT_CALLBACK_API_NAME, "(IF)V");
    env->CallVoidMethod(javaObj, mid, msgType, msgCode);
    if (isAttach) {
        player->GetJavaVM()->DetachCurrentThread();
    }


}
