package com.jar.media_player_five

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.jar.media_player_five.databinding.ActivityMainBinding
import com.jar.media_player_five.media.FFMediaPlayer

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = FFMediaPlayer.GetFFmpegVersion()
    }


    companion object {

    }
}