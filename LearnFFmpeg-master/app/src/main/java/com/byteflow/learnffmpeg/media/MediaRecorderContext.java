package com.byteflow.learnffmpeg.media;

import android.util.Log;

import androidx.annotation.LongDef;

public abstract class MediaRecorderContext {
    private static final String TAG = "MediaRecorderContext : ";

    public static final int IMAGE_FORMAT_RGBA = 0x01;
    public static final int IMAGE_FORMAT_NV21 = 0x02;
    public static final int IMAGE_FORMAT_NV12 = 0x03;
    /**
     * 4个Y 分量，共用一个uv 分量；
     */
    public static final int IMAGE_FORMAT_I420 = 0x04;


    /**
     * 仅录制视频
     */
    public static final int RECORDER_TYPE_SINGLE_AUDIO = 1;
    /**
     * 仅录制音频
     */
    public static final int RECORDER_TYPE_AV           = 2;
    /**
     * 同时录制音频和视频,打包成 MP4 文件
     */
    public static final int RECORDER_TYPE_SINGLE_VIDEO = 0;

    // native 层 指针
    private long mNativeContextHandle = -1;

    protected void testNativeContextHandle() {
        Log.d(TAG, "testNativeContextHandle: mNativeContextHandle : " + mNativeContextHandle);
    }

    protected native void native_CreateContext();

    protected native void native_DestroyContext();

    protected native int native_Init();

    protected native int native_UnInit();

    protected native int native_StartRecord(int recorderType, String outUrl, int frameWidth, int frameHeight, long videoBitRate, int fps);

    protected native void native_OnAudioData(byte[] data, int len);

    protected native void native_OnPreviewFrame(int format, byte[] data, int width, int height);

    protected native int native_StopRecord();

    protected native void native_SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    protected native void native_OnSurfaceCreated();

    protected native void native_OnSurfaceChanged(int width, int height);

    protected native void native_OnDrawFrame();

    protected native void native_SetFilterData(int index, int format, int width, int height, byte[] bytes);

    protected native void native_SetFragShader(int index, String str);
}
