for(i=1; i<=11; i++){
open("/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/lena.tif");
run("DCT ", "block=8 multiply=1.00000 resize=[split image into tiles of size N x N]");
open("/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression/Montage"+i+".tif");
imageCalculator("Multiply create", "DCT","Montage"+i+".tif");
selectWindow("Result of DCT");
run("DCT ", "block=8 multiply=1.00000 inverse resize=[split image into tiles of size N x N]");
saveAs("Tiff", "/home/josh/Documents/Physics/Year 3/ImageProcessing/Assessed_Exercise1/manual compression/InverseDCT"+i+".tif");

close();
selectWindow("Result of DCT");
close();
selectWindow("Montage"+i+".tif");
close();
selectWindow("DCT");
close();
selectWindow("lena.tif");
close();
}
