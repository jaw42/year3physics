for(i=0; i<=15; i++){
	open("/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression lena2/mask"+i+".tif");
	makeLine(i, 0, 0, i);
	run("Draw");
	saveAs("Tiff", "/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression lena2/mask"+i+1+".tif");

	run("Select All");
	runMacro("/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/mask_copy_macro.txt");
	run("Make Montage...", "columns=32 rows=32 scale=1 first=1 last=1024 increment=1 border=0 font=12");
	saveAs("Tiff", "/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression lena2/Montage"+i+".tif");

	open("/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/lena.tif");
	run("DCT ", "block=8 multiply=1.00000 resize=[split image into tiles of size N x N]");

	imageCalculator("Multiply create", "DCT","Montage"+i+".tif");
	selectWindow("Result of DCT");

	run("DCT ", "block=8 multiply=1.00000 inverse resize=[split image into tiles of size N x N]");
	saveAs("Jpeg", "/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression lena2/lena"+i+".jpg");

	close();
	selectWindow("Result of DCT");
	close();
	selectWindow("Montage"+i+".tif");
	close();
	selectWindow("DCT");
	close();
	selectWindow("lena.tif");
	close();
	selectWindow("mask"+i+1+".tif");
	close();
}
