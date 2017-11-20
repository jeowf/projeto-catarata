//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage *objectImage = newObjectImage(argc, argv);
	Pixel **image = readImage(objectImage);

	applyGrayScale(image, objectImage->width, objectImage->height);
	applyGaussian(image, objectImage->width, objectImage->height);
	applySobel(image, objectImage->width, objectImage->height);

	int **binImage = getBinImage(image, objectImage->width, objectImage->height, 50);
	//image = createBinImage(binImage, objectImage->width, objectImage->height);

	writeImage(objectImage, image);
	
	return 0;
}
