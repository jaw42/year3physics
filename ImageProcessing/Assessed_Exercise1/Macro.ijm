for(i=1; i<=11; i++){
	//Starting with an initial hand made mask section, and subsequently the 
	// previously the created one, a new mask section is made by drawing out the 
	// next  diagonal line of pixels. This is then saved
	open("/JPEG_compression/mask"+i-1+".tif");
	makeLine(i+1, 0, 0, i+1);
	run("Draw");
	saveAs("Tiff", "/JPEG_compression/mask"+i+".tif");

	//Using a predefined macro, the mask section is copied the relavent number
	// of times to cover the whole image,this is then arranged into a full mask,
	// and saved.
	run("Select All");
	runMacro("/JPEG_compression/mask_copy_macro.txt");
	run("Make Montage...", "columns=32 rows=32 scale=1 first=1 last=1024 increment=1 border=0 font=12");
	saveAs("Tiff", "/JPEG_compression/Montage"+i+".tif");
	
	// The image to be compressed is opened and the DCT taken of it. 
	open("/JPEG_compression/bridge.tif");
	run("DCT ", "block=8 multiply=1.00000 resize=[split image into tiles of size N x N]");

	//The DCT and the mask that has been created are multiplied together such 
	// that the DCT is unaffected where the mask is non-zero and removed where 
	// the mask is zero.
	imageCalculator("Multiply create", "DCT","Montage"+i+".tif");
	selectWindow("Result of DCT");

	//The inverse of this new ``image'' is then taken to return to the image 
	// proper, with the removal of some information resulting in a smaller image
	// file size and reduced quality
	run("DCT ", "block=8 multiply=1.00000 inverse resize=[split image into tiles of size N x N]");
	saveAs("Jpeg", "/JPEG_compression/InverseDCT"+i+".jpg");

	// The various windows created are closed.
	close();
	selectWindow("Result of DCT");
	close();
	selectWindow("Montage"+i+".tif");
	close();
	selectWindow("DCT");
	close();
	selectWindow("bridge.tif");
	close();
	selectWindow("mask"+i+".tif");
	close();
}


