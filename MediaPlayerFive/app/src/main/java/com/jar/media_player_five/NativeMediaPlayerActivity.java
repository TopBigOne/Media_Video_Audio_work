package com.jar.media_player_five;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.SeekBar;

import com.jar.media_player_five.databinding.ActivityMainBinding;
import com.jar.media_player_five.databinding.ActivityNativeMediaPlayerBinding;
import com.jar.media_player_five.media.FFMediaPlayer;
import com.jar.media_player_five.media.MySurfaceView;

public class NativeMediaPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback, FFMediaPlayer.EventCallback {

    private ActivityNativeMediaPlayerBinding binding;

    private static final String TAG = "MediaPlayerActivity";
    private static final String[] REQUEST_PERMISSIONS = {Manifest.permission.WRITE_EXTERNAL_STORAGE,};
    private static final int PERMISSION_REQUEST_CODE = 1;
    private MySurfaceView mSurfaceView = null;
    private FFMediaPlayer mMediaPlayer = null;
    private SeekBar mSeekBar = null;
    private boolean mIsTouch = false;
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/byteflow/one_piece.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityNativeMediaPlayerBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


        mSurfaceView = binding.surfaceView;
        mSurfaceView.getHolder().addCallback(this);

        mSeekBar = binding.seekBar;
        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                mIsTouch = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStopTrackingTouch() called with: progress = [" + seekBar.getProgress() + "]");
                if (mMediaPlayer != null) {
                    mMediaPlayer.seekToPosition(mSeekBar.getProgress());
                    mIsTouch = false;
                }

            }
        });

    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void onPlayerEvent(int msgType, float msgValue) {

    }
}