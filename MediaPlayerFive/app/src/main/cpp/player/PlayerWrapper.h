//
// Created by dev on 2022/9/4.
//

#ifndef MEDIAPLAYERFIVE_PLAYERWRAPPER_H
#define MEDIAPLAYERFIVE_PLAYERWRAPPER_H


#include <jni.h>
#include <HWCodecPlayer.h>
#include <FFMediaPlayer.h>

static const int FFMEDIA_PLAYER = 0;
static const int HWCODEC_PLAYER = 1;

class PlayerWrapper {
public:
    PlayerWrapper(){};
    virtual ~PlayerWrapper(){};

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int playerType, int renderType, jobject surface);
    void UnInit();

    void Play();
    void Pause();
    void Stop();
    void SeekToPosition(float position);
    long GetMediaParams(int paramType);
    void SetMediaParams(int paramType, jobject obj);

private:
    MediaPlayer* m_MediaPlayer = nullptr;

};


#endif //MEDIAPLAYERFIVE_PLAYERWRAPPER_H
