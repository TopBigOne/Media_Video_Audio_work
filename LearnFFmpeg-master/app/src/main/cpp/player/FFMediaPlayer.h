//
// Created by dev on 2023/2/18.
//

#ifndef LEARNFFMPEG_FFMEDIAPLAYER_H
#define LEARNFFMPEG_FFMEDIAPLAYER_H

#include "MediaPlayer.h"

/**
 *  这里依赖 音视频解码和音视频 渲染
 */
class FFMediaPlayer : public MediaPlayer {
public:
    FFMediaPlayer() {}

    virtual ~FFMediaPlayer() {};

    virtual void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface);

    virtual void UnInit();

    virtual void Play();

    virtual void Pause();

    virtual void Stop();

    virtual void SeekToPosition(float position);

    virtual long GetMediaParams(int paramType);

private:
    virtual JNIEnv *GetJNIEnv(bool *isAttach);

    virtual jobject GetJavaObj();

    virtual JavaVM *GetJavaVM();

    static void PostMessage(void *context, int msgType, float msgCode);

    VideoDecoder *m_VideoDecoder = nullptr;
    AudioDecoder *m_AudioDecoder = nullptr;

    VideoRender *m_VideoRender = nullptr;
    AudioRender *m_AudioRender = nullptr;


};


#endif //LEARNFFMPEG_FFMEDIAPLAYER_H
