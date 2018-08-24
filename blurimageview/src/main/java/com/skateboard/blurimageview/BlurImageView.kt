package com.skateboard.blurimageview

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class BlurImageView(context: Context, attributeSet: AttributeSet?) : GLSurfaceView(context, attributeSet)
{
   init
   {
       setEGLContextClientVersion(3)
       setRenderer(BlurImageViewRender(context))
       renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
   }
}