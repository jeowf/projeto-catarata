//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage *objectImage = newObjectImage(argc, argv);
	Pixel **image = findFormat(objectImage);

	printf("Resizing\n");
	image = squareImage(image, &objectImage->width, &objectImage->height, 2);
	Pixel **imageB = copyImage(image, objectImage->width, objectImage->height);

	//image = resizeImage(image, &objectImage->width, &objectImage->height, 2);
	//printf("%d %d \n",  objectImage->width, objectImage->height);
	printf("Applying gray scale\n");
	applyGrayScale(image, objectImage->width, objectImage->height);


	Circle c = {349, 439, 82}; //Catarata.ppm
	//Circle c = {391, 490, 140}; //Catarata2.ppm
	//Circle c = {326, 371, 154}; //Normal.ppm
	//Circle c = {205, 239, 64}; //Normal2.ppm
	//cataractDiagnosis (image, c);
	//drawCircle(image, objectImage->width, objectImage->height, c, 3);
	/*
	printf("Applying gaussian filter\n");
	applyGaussian(image, objectImage->width, objectImage->height);
	
	printf("Applying sobel filter\n");
	applySobel(image, objectImage->width, objectImage->height);

	printf("Binarizing image\n");
	int **binImage = getBinImage(image, objectImage->width, objectImage->height, 20);
	image = createBinImage(binImage, objectImage->width, objectImage->height);
	*/
	
	//findWrite(objectImage, image);
	writeDiagnosis(*objectImage, cataractDiagnosis(image, c), 60.0);

/*	
	printf("Finding the circle\n");
	//Circle c = {349, 439, 82};
	Circle c = findCircle(binImage, objectImage->width, objectImage->height);
	//image = plotImage(binImage, objectImage->width, objectImage->height);
	
	printf("Writing image at %s\n", objectImage->destination);
	drawCircle(imageB, objectImage->width, objectImage->height, c, 3);
	//writeImage(objectImage, image);
	writeImage(objectImage, imageB);
	

	*/
	return 0;

}
