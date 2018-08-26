# BlurImageView
高斯模糊图片

     bitmap = BitmapFactory.decodeResource(resources, R.drawable.pics, null)
        bitmap?.let {

            blurImageView.setImageBitmap(it)
        }
