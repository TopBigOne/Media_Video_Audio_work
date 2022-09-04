package com.jar.mediaplayerseven.media;

/**
 * @author : dev
 * @version :
 * @Date :  2022/9/4 22:15
 * @Desc :
 */
public class FFMediaPlayer {
    //gl render type
    public static final int VIDEO_GL_RENDER = 0;
    public static final int AUDIO_GL_RENDER = 1;
    public static final int VR_3D_GL_RENDER = 2;

    //player type
    public static final int FFMEDIA_PLAYER = 0;
    public static final int HWCODEC_PLAYER = 1;

    static {
        System.loadLibrary("mediaplayerseven");
    }


    public static String GetFFmpegVersion() {
        return native_GetFFmpegVersion();
    }

    private static native String native_GetFFmpegVersion();
}
