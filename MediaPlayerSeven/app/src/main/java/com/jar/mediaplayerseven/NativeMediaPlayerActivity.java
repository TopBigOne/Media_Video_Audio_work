package com.jar.mediaplayerseven;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.SeekBar;

import com.jar.mediaplayerseven.databinding.ActivityNativeMediaPlayerBinding;
import com.jar.mediaplayerseven.media.FFMediaPlayer;
import com.jar.mediaplayerseven.media.MySurfaceView;

public class NativeMediaPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback, FFMediaPlayer.EventCallback {
    private static final String TAG = "MediaPlayerActivity";
    private static final String[] REQUEST_PERMISSIONS = {Manifest.permission.WRITE_EXTERNAL_STORAGE,};
    private static final int PERMISSION_REQUEST_CODE = 1;
    private MySurfaceView mSurfaceView = null;
    private FFMediaPlayer mMediaPlayer = null;
    private SeekBar mSeekBar = null;
    private boolean mIsTouch = false;
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/byteflow/one_piece.mp4";

    private ActivityNativeMediaPlayerBinding binding;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityNativeMediaPlayerBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


        mSurfaceView = binding.surfaceView;
        mSurfaceView.getHolder().addCallback(this);

        mSeekBar = binding.seekBar;

        initSeekBarChangeListener();

    }



    private void initSeekBarChangeListener() {
        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                Log.d(TAG, "onProgressChanged: ----invoke");

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                mIsTouch = true;
                Log.d(TAG, "onStartTrackingTouch: ----invoke");

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStopTrackingTouch() called with: progress = [" + seekBar.getProgress() + "]");
                if(mMediaPlayer != null) {
                    mMediaPlayer.seekToPosition(mSeekBar.getProgress());
                    mIsTouch = false;
                }

            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE);
        }
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        Log.d(TAG, "surfaceCreated: ");

    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        Log.d(TAG, "surfaceChanged: ");

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        Log.d(TAG, "surfaceDestroyed: ");

    }

    @Override
    public void onPointerCaptureChanged(boolean hasCapture) {
        super.onPointerCaptureChanged(hasCapture);
        Log.d(TAG, "onPointerCaptureChanged: ");
        mMediaPlayer.play();
    }

    @Override
    public void onPlayerEvent(int msgType, float msgValue) {
        Log.d(TAG, "onPlayerEvent: ");

    }


    protected boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }


}