package com.skateboard.blurimageview

import android.content.Context
import android.graphics.Bitmap
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class BlurImageView(context: Context, attributeSet: AttributeSet?) : GLSurfaceView(context, attributeSet)
{
    init
    {
        setEGLContextClientVersion(3)

    }

    fun setImageBitmap(bitmap: Bitmap)
    {
        setRenderer(BlurImageViewRender(context, bitmap))
        renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
    }
}