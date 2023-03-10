//
// Created by dev on 2023/3/7.
//

#ifndef LEARNFFMPEG_DECODERBASE_H
#define LEARNFFMPEG_DECODERBASE_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
};

#include "Decoder.h"
#include <thread>

#define MAX_PATH 2048
#define DELAY_THRESHOLD 100  // 100ms
using namespace std;

enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP,
};
enum DecoderMsg {
    MSG_DECODER_INIT_ERROR,
    MSG_DECODER_READY,
    MSG_DECODER_DONE,
    MSG_REQUEST_RENDER,
    MSG_DECODING_TIME,
};

class DecoderBase : public Decoder {
public:
    DecoderBase() {};

    virtual ~DecoderBase() {}

    virtual void Start();

    virtual void Pause();

    virtual void Stop();

    virtual float GetDuration() {
        return m_Duration * 1.0f / 1000;

    }

    virtual void SeekToPosition(float position);

    virtual float GetCurrentPosition();
    virtual void ClearCache(){}

    /**
     * todo 为什么直接在头文件里 进行赋值？
     * @param context
     * @param callback
     */
    virtual void SetMessageCallback(void *context, MessageCallback callback) {
        m_MsgContext = context;
        m_MsgCallback = callback;
    }

    virtual void SetAVSyncCallback(void *context, AVSyncCallback callback) {
        m_AVDecoderContext = context;
        m_AVSyncCallback = callback;
    }


protected:
    // todo 什么时候初始化的？
    void *m_MsgContext = nullptr;
    // todo 什么时候初始化的？
    MessageCallback m_MsgCallback = nullptr;

    virtual int Init(const char *url, AVMediaType mediaType);

    virtual void UnInit();

    virtual void OnDecoderReady() = 0;

    virtual void OnDecoderDone() = 0;

    // 解码数据回调
    virtual void OnFrameAvailable(AVFrame *frame) = 0;

    AVCodecContext *GetCodecContext() {
        return m_AVCodecContext;
    }

private:
    int InitFFDecoder();

    void UnInitDecoder();

    // 启动解码线程
    void StartDecodingThread();

    // 音视频循环解码
    void DecodingLoop();

    // 更新时间戳
    void UpdateTimeStamp();

    // 音视频同步
    long AVSync();

    // 解码一个packet 编码数据
    int DecodeOnePacket();

    // 线程函数
    static void DoAVDecoding(DecoderBase *decoder);

    // 封装格式上下文
    AVFormatContext *m_AVFormatContext = nullptr;
    // 解码器上下文
    AVCodecContext *m_AVCodecContext = nullptr;
    // 解码器
    AVCodec *m_AVCodec = nullptr;
    // 解码器的数据包
    AVPacket *m_Packet = nullptr;
    // 解码的帧
    AVFrame *m_Frame = nullptr;
    // 数据类的类型
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    // 文件地址
    char m_Url[MAX_PATH] = {0};
    // 当前播放时间
    long m_CurTimeStamp = 0;
    // 播放的起始时间
    long m_StartTimeStamp = -1;
    // 总时长
    long m_Duration = 0;
    // 数据流索引
    int m_StreamIndex = -1;
    // 锁和条件变量
    mutex m_Mutex;
    condition_variable m_Cond;
    // todo C++ 的thread 是怎么用的啊？
    thread *m_Thread = nullptr;
    // seek position
    volatile float m_SeekPosition = 0;
    volatile bool m_SeekSuccess = false;
    // 解码器状态
    volatile int m_DecoderState = STATE_UNKNOWN;
    void *m_AVDecoderContext = nullptr;
    AVSyncCallback m_AVSyncCallback = nullptr;

};


#endif //LEARNFFMPEG_DECODERBASE_H
