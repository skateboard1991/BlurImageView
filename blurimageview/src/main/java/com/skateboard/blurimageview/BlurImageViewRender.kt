package com.skateboard.blurimageview

import android.content.Context
import android.graphics.Bitmap
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import java.io.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class BlurImageViewRender(private val context: Context, private var bitmap: Bitmap?) : GLSurfaceView.Renderer
{

    private var srcWidth: Int = 0

    private var srcHeight: Int = 0

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?)
    {

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
        prepare(width, height)
        GLES30.glViewport(0, 0, width, height)
    }

    private fun prepare(width: Int, height: Int)
    {
        srcWidth = width
        srcHeight = height
        bitmap?.let {
            val vertex = readSlgl("vertex.slgl")
            val fragment = readSlgl("fragment.slgl")
            prepare(vertex, fragment, it, width, height)
        }
    }

    fun setImageBitmap(bitmap: Bitmap)
    {
        this.bitmap = bitmap
        prepare(srcWidth, srcHeight)
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

    external fun prepare(vertex: String, fragment: String, bitmap: Bitmap, scrWidth: Int, scrHeight: Int)

    external fun draw()
}