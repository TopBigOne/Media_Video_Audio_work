#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
#include <libavformat/avformat.h>
}


const char *TAG = "xiao_ya";

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

void ffmpeg_log_output(void *ptr, int level, const char *fmt, va_list vl) {
    LOGD("[ffmpeg log]%s", fmt);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jar_mediaplayer_1one_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    av_log_set_callback(ffmpeg_log_output);
    AVFormatContext *pContext = avformat_alloc_context();
    const char *url = "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8";
    avformat_open_input(&pContext, url, nullptr, nullptr);
    avformat_find_stream_info(pContext, nullptr);

    return env->NewStringUTF(hello.c_str());
}