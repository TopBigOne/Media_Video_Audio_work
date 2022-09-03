package com.jar.media_player_five.media;

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

    public static String GetFFmpegVersion() {
        return native_GetFFmpegVersion();
    }

    private static native String native_GetFFmpegVersion();
}
