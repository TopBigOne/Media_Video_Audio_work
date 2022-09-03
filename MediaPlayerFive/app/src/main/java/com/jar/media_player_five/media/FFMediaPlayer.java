package com.jar.media_player_five.media;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.view.Surface;

/**
 * @author : dev
 * @version :
 * @Date :  2022/9/3 16:40
 * @Desc :
 */
public class FFMediaPlayer {
    static {
        System.loadLibrary("media_player_five");
    }

    //player type
    public static final int FFMEDIA_PLAYER = 0;
    public static final int HWCODEC_PLAYER = 1;

    private long mNativePlayerHandle = 0;
    private AudioTrack mAudioTrack;

    /**
     * jni call java
     * @param sampleRateInHz
     * @param nb_channels
     */
    public void createTrack(int sampleRateInHz, int nb_channels) {
        int channelConfig;
        if (nb_channels == 1) {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO;
        } else if (nb_channels == 2) {
            channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
        } else {
            channelConfig = AudioFormat.CHANNEL_OUT_MONO;
        }
        int bufferSize = AudioTrack.getMinBufferSize(sampleRateInHz, channelConfig, AudioFormat.ENCODING_PCM_16BIT);
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRateInHz, channelConfig, AudioFormat.ENCODING_PCM_16BIT, bufferSize, AudioTrack.MODE_STREAM);
        mAudioTrack.play();
    }

    public static String GetFFmpegVersion() {
        return native_GetFFmpegVersion();
    }

    public void init(String url, int videoRenderType, Surface surface) {
        mNativePlayerHandle = native_Init(url, FFMEDIA_PLAYER, videoRenderType, surface);
    }

    public void init(String url, int playerType, int videoRenderType, Surface surface) {
        mNativePlayerHandle = native_Init(url, playerType, videoRenderType, surface);
    }

    public void play() {
        native_Play(mNativePlayerHandle);
    }

    public void pause() {
        native_Pause(mNativePlayerHandle);
    }

    public void seekToPosition(float position) {
        native_SeekToPosition(mNativePlayerHandle, position);
    }

    public void stop() {
        native_Stop(mNativePlayerHandle);
    }

    public void unInit() {
        native_UnInit(mNativePlayerHandle);
        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack.release();
        }
    }


    private static native String native_GetFFmpegVersion();

    private native long native_Init(String url, int playerType, int renderType, Object surface);

    private native void native_Play(long playerHandle);

    private native void native_SeekToPosition(long playerHandle, float position);

    private native void native_Pause(long playerHandle);

    private native void native_Stop(long playerHandle);

    private native void native_UnInit(long playerHandle);


    public interface EventCallback {
        void onPlayerEvent(int msgType, float msgValue);
    }
}
