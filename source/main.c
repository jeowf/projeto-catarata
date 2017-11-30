//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage *objectImage = newObjectImage(argc, argv);
	Pixel **image = readImage(objectImage);

	//converte imagem para escala cinza
	applyGrayScale(image, objectImage->width, objectImage->height);

	//faz copia da imagem para ser usada posteriormente
	Pixel **imageOrig = copyImage(image, objectImage->width, objectImage->height);
	
	applyGaussian(image, objectImage->width, objectImage->height);

	//binariza a imagem com objetivo de separar pontos de luz
	int maxIntensity = getMaxPixelIntensity(image, objectImage->width, objectImage->height);
	int **binSpecImage = getBinImage(image, objectImage->width, objectImage->height, maxIntensity);

	//binariza a imagem com objetivo de encontrar a área da íris
	int **binAreaImage = getBinImage(image, objectImage->width, objectImage->height, -70);
	//image = createBinImage(binAreaImage, objectImage->width, objectImage->height);
	//image = createBinImage(binSpecImage, objectImage->width, objectImage->height);
	//writeImage(objectImage, image);

	Circle estimatedCenter = estimateCenter(binSpecImage, objectImage->width, objectImage->height);
	free(binSpecImage);

	fill (binAreaImage, estimatedCenter.x, estimatedCenter.y, 0, 1);
	//image = createBinImage(binAreaImage, objectImage->width, objectImage->height);
	//writeImage(objectImage, image);

	/*
	//DEBUG
	int i, j;
	for (i = 0; i < objectImage->height; i++){
		for (j = 0; j < objectImage->width; j++){
			if (i == estimatedCenter.y || j ==  estimatedCenter.x){
				image[i][j].r = 255;
				image[i][j].g = 0;
				image[i][j].b = 0;


			}
		}
	}
	*/


	Circle irisCenter = fastFindCircle(binAreaImage, objectImage->width, objectImage->height, estimatedCenter);
	free(binAreaImage);

	Pixel **irisRegion = cropImage(imageOrig, &objectImage->width, &objectImage->height, irisCenter);
	free(imageOrig);
	Pixel **imageFinal = copyImage(irisRegion, objectImage->width, objectImage->height);

	//int **aaa = getBinImage(irisRegion, objectImage->width, objectImage->height, -70);
	//irisRegion = createBinImage(aaa, objectImage->width, objectImage->height);

	
	/*
	//DEBUG
	for (i = 0; i < objectImage->height; i++){
		for (j = 0; j < objectImage->width; j++){
			if (i == irisCenter.y || j ==  irisCenter.x){
				image[i][j].r = 0;
				image[i][j].g = 255;
				image[i][j].b = 0;


			}
		}
	}
	writeImage(objectImage, image);
	*/

	
	applySobel(irisRegion, objectImage->width, objectImage->height);
	int **edges = getBinImage(irisRegion, objectImage->width, objectImage->height, 10);
	//irisRegion = createBinImage(edges, objectImage->width, objectImage->height);
	//int w = countPixels(edges, objectImage->width, objectImage->height, 1);
	//int b = countPixels(edges, objectImage->width, objectImage->height, 0);
	//printf("w:%d  b:%d\n",w, b );

	//int r =  0.9 * irisCenter.r;
	//printf("%d\n", r);
	int whitePixels = countPixels(edges, objectImage->width, objectImage->height, 1);
	int blackPixels = objectImage->width*objectImage->height - whitePixels;
	double kRmax = 1 - fmax(whitePixels - blackPixels, 0)/(objectImage->width*objectImage->height),
		   kRmin = kRmax * 0.5;
	printf("KRMAX %.2lf KRMIN %.2lf\n",kRmax*irisCenter.r, kRmin*irisCenter.r );

	int **im = getBinImage(imageFinal, objectImage->width, objectImage->height, maxIntensity-5);	
	//int **im = getBinImage(imageFinal, objectImage->width, objectImage->height, maxIntensity);	
	//imageFinal = createBinImage(im, objectImage->width, objectImage->height);
	writeImage(objectImage, imageFinal);

	Circle iris = findCircle(edges, objectImage->width, objectImage->height, kRmin *  irisCenter.r, kRmax * irisCenter.r);
	///////Circle flash = findCircle(edges, objectImage->width, objectImage->height, 5, 20);
	
	////////drawCircle(imageFinal, objectImage->width, objectImage->height, flash, 1);

	/*
	Circle flash = findCircle(im, objectImage->width, objectImage->height, kRmin*irisCenter.r*0.1,kRmin*0.5*irisCenter.r);
	drawCircle(imageFinal, objectImage->width, objectImage->height, flash, 1);
	*/

	fillImage (imageFinal, iris, 200, getMediumPixel (imageFinal,iris));
	applyGaussian(image, objectImage->width, objectImage->height);

	drawCircle(imageFinal, objectImage->width, objectImage->height, iris, 1);
	

	writeImage(objectImage, imageFinal);

/*
	printf("%d, %d\n", estimatedCenter.x, estimatedCenter.y);
	Pixel **imageK = cropImage(imageB, &objectImage->width, &objectImage->height, estimatedCenter);
	Pixel **imagecopia = copyImage(imageK, objectImage->width, objectImage->height);
	applyGaussian(imageK, objectImage->width, objectImage->height);
	applySobel(imageK, objectImage->width, objectImage->height);
	int **binImage2 = getBinImage(imageK, objectImage->width, objectImage->height, 10);
	//image = createBinImage(binImage, objectImage->width, objectImage->height);
	
	Circle c = findCircle(binImage2, objectImage->width, objectImage->height, 80,160);
	
	drawCircle(imagecopia, objectImage->width, objectImage->height, c, 1);
	writeImage(objectImage, imagecopia);
*/
	
	return 0;

	//Circle c = {349, 439, 82}; //Catarata.ppm   1015X759 = 770385 (0.01064402863) 0.0163839089
	//Circle c = {391, 490, 140}; //Catarata2.ppm 1198X770 = 922460 (0.01517680983)
	//Circle c = {326, 371, 154}; //Normal.ppm    1167X739 = 862413 (0.01785687367)
	//Circle c = {205, 239, 64}; //Normal2.ppm    610X480  = 292800 (0.02185792349)

}
