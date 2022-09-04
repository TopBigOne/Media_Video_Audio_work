package com.jar.mediaplayerseven;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.jar.mediaplayerseven.databinding.ActivityMainBinding;
import com.jar.mediaplayerseven.media.FFMediaPlayer;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(FFMediaPlayer.GetFFmpegVersion());
    }






}