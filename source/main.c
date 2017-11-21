//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage *objectImage = newObjectImage(argc, argv);
	Pixel **image = readImage(objectImage);

	printf("Resizing\n");
	image = squareImage(image, &objectImage->width, &objectImage->height, 2);
	Pixel **imageB = copyImage(image, objectImage->width, objectImage->height);

	//image = resizeImage(image, &objectImage->width, &objectImage->height, 2);
	//printf("%d %d \n",  objectImage->width, objectImage->height);
	printf("Applying gray scale\n");
	applyGrayScale(image, objectImage->width, objectImage->height);

	printf("Applying gaussian filter\n");
	applyGaussian(image, objectImage->width, objectImage->height);
	
	printf("Applying sobel filter\n");
	applySobel(image, objectImage->width, objectImage->height);

	printf("Binarizing image\n");
	int **binImage = getBinImage(image, objectImage->width, objectImage->height, 20);
	image = createBinImage(binImage, objectImage->width, objectImage->height);

	writeImage(objectImage, image);

	
	printf("Finding the circle\n");
	//Circle c = {349, 439, 82};
	Circle c = findCircle(binImage, objectImage->width, objectImage->height);
	//image = plotImage(binImage, objectImage->width, objectImage->height);
	
	printf("Writing image at %s\n", objectImage->destination);
	drawCircle(imageB, objectImage->width, objectImage->height, c, 3);
	//writeImage(objectImage, image);
	writeImage(objectImage, imageB);
	

	
	return 0;
}
