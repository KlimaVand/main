package com.af.euroblight2.objects;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.util.Date;
import java.text.SimpleDateFormat;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;

@SuppressLint("SimpleDateFormat")
public class ReportPhoto implements Serializable {
	
	/** Serializable Id */
	
	private static final long serialVersionUID = 1L;
	
	// Variables
	private String path;
	private static final String PUBLIC_DIRECTORY_NAME = "BlightTracker";
	private static final int TARGET_WIDTH = 640;
	private static final int TARGET_HEIGHT = 480;
	
	/** Public Methods */
	
	public String getPath() {
		return path;
	}
	
	public Intent getMediaScanIntent() {
	    Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
	    File f = new File(path);
	    Uri contentUri = Uri.fromFile(f);
	    mediaScanIntent.setData(contentUri);
	    return mediaScanIntent;
	}
	
	public File createImageFile() throws IOException {
	    // Create an image file name
	    String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
	    String imageFileName = "JPEG_" + timeStamp + "_";
	    File storageDir = Environment.getExternalStoragePublicDirectory(PUBLIC_DIRECTORY_NAME);
	    if (!storageDir.exists()) {
	    	storageDir.mkdir();
	    }
	    File image = File.createTempFile(
	        imageFileName,  /* prefix */
	        ".jpg",         /* suffix */
	        storageDir      /* directory */
	    );

	    // Save a file: path for use with ACTION_VIEW intents
	    path = image.getAbsolutePath();
	    return image;
	}
	
	public Bitmap getBitmap() {
		BitmapFactory.Options bitmapOptions = new BitmapFactory.Options();
		// Get the desired dimensions 
		int targetW = TARGET_WIDTH;
		int targetH = TARGET_HEIGHT;

		// Get the dimensions of the bitmap  
		bitmapOptions.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(path, bitmapOptions);
		int photoW = bitmapOptions.outWidth;
		int photoH = bitmapOptions.outHeight;
		    
		// Determine how much to scale down the image
		int scaleFactor = Math.min(photoW/targetW, photoH/targetH);
		
		// Decode the image file into a Bitmap sized as desired
	    bitmapOptions.inJustDecodeBounds = false;
	    bitmapOptions.inSampleSize = scaleFactor;
	    bitmapOptions.inPurgeable = true;
	    Bitmap bitmap = BitmapFactory.decodeFile(path, bitmapOptions);
	    return bitmap;
	}	
}