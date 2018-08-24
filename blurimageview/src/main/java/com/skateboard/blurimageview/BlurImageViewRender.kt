package com.skateboard.blurimageview

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import java.io.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class BlurImageViewRender(private val context: Context) : GLSurfaceView.Renderer
{


    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?)
    {
        val vertex = readSlgl("vertex.slgl")
        val fragment = readSlgl("fragment.slgl")
        val bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.timg)
        prepare(vertex, fragment, bitmap)
        bitmap.recycle()
    }

    private fun readSlgl(fileName: String): String
    {
        val buffer = StringBuffer()
        try
        {
            val inReader = BufferedReader(InputStreamReader(context.assets.open(fileName)))
            var item = inReader.readLine()
            while (item != null)
            {
                buffer.append(item).append("\n")
                item = inReader.readLine()
            }
            inReader.close()
        } catch (e: IOException)
        {
            e.printStackTrace()
        }

        return buffer.toString()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int)
    {
        GLES30.glViewport(0, 0, width, height)
    }

    override fun onDrawFrame(gl: GL10?)
    {
        draw()
    }

    companion object
    {
        init
        {
            System.loadLibrary("blurimageview")
        }
    }

    external fun prepare(vertex: String, fragment: String, bitmap: Bitmap)

    external fun draw()
}