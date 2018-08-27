package com.skateboard.blurimageview

import android.content.Context
import android.graphics.Bitmap
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class BlurImageView(context: Context, attributeSet: AttributeSet?) : GLSurfaceView(context, attributeSet)
{
    private var isSetRender = false

    private var render: BlurImageViewRender

    init
    {
        setEGLContextClientVersion(3)
        render = BlurImageViewRender(context, null)
        setRenderer(render)
        renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        isSetRender = true
    }

    fun setImageBitmap(bitmap: Bitmap)
    {
        queueEvent {

            render.setImageBitmap(bitmap)
        }
    }

    override fun onResume()
    {
        if (isSetRender)
        {
            super.onResume()
        }
    }

    override fun onPause()
    {
        if (isSetRender)
        {
            super.onPause()
        }
    }
}