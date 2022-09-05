
#include <string>


#include <cstdio>
#include <cstring>
#include "player/PlayerWrapper.h"
#include "player/render/video/VideoGLRender.h"
#include "player/render/video/VRGLRender.h"
#include "player/render/audio/OpenSLRender.h"
#include "recorder/MediaRecorderContext.h"
#include "util/LogUtil.h"
#include "jni.h"

extern "C" {
#include <libavcodec/version.h>
#include <libavcodec/avcodec.h>
#include <libavformat/version.h>
#include <libavutil/version.h>
#include <libavfilter/version.h>
#include <libswresample/version.h>
#include <libswscale/version.h>
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1GetFFmpegVersion(JNIEnv *env,
                                                                              jclass clazz) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "libavcodec : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(strBuffer, "\nlibavformat : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(strBuffer, "\nlibavutil : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVUTIL_VERSION));
    strcat(strBuffer, "\nlibavfilter : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFILTER_VERSION));
    strcat(strBuffer, "\nlibswresample : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    strcat(strBuffer, "\nlibswscale : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWSCALE_VERSION));
    strcat(strBuffer, "\navcodec_configure : \n");
    strcat(strBuffer, avcodec_configuration());
    strcat(strBuffer, "\navcodec_license : ");
    strcat(strBuffer, avcodec_license());
    LOGCATE("GetFFmpegVersion\n%s", strBuffer);

    //ASanTestCase::MainTest();

    return env->NewStringUTF(strBuffer);
}






extern "C"
JNIEXPORT jlong JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1Init(JNIEnv *env, jobject thiz,
                                                                  jstring url, jint player_type,
                                                                  jint render_type,
                                                                  jobject surface) {


}
extern "C"
JNIEXPORT void JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1Play(JNIEnv *env, jobject thiz,
                                                                  jlong player_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1SeekToPosition(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jlong player_handle,
                                                                            jfloat position) {


}
extern "C"
JNIEXPORT void JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1Pause(JNIEnv *env, jobject thiz,
                                                                   jlong player_handle) {

    PlayerWrapper *ffMediaPlayer = reinterpret_cast<PlayerWrapper *>(player_handle);
    ffMediaPlayer->Pause();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1Stop(JNIEnv *env, jobject thiz,
                                                                  jlong player_handle) {
    // TODO: implement native_Stop()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jar_media_1player_1five_media_FFMediaPlayer_native_1UnInit(JNIEnv *env, jobject thiz,
                                                                    jlong player_handle) {
    // TODO: implement native_UnInit()
}