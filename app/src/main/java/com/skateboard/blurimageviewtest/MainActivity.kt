package com.skateboard.blurimageviewtest

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity()
{

    private var bitmap: Bitmap? = null

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        bitmap = BitmapFactory.decodeResource(resources, R.drawable.timg, null)
        bitmap?.let {

            blurImageView.setImageBitmap(it)
        }
        // Example of a call to a native method
        //        sample_text.text = stringFromJNI()
    }


    override fun onResume()
    {
        super.onResume()
        blurImageView.onResume()
    }

    override fun onPause()
    {
        super.onPause()
        blurImageView.onPause()
    }


    override fun onDestroy()
    {
        super.onDestroy()
        bitmap?.recycle()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object
    {

        // Used to load the 'native-lib' library on application startup.
        init
        {
            System.loadLibrary("native-lib")
        }
    }
}
