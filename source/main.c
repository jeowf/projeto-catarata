//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage *objectImage = newObjectImage(argc, argv);
	Pixel **image = readImage(objectImage);

	//printf("Recortando região da pupila\n");
	//image = cropImage(image, &objectImage->width, &objectImage->height);

	//printf("Aplicando escala cinza\n");
	applyGrayScale(image, objectImage->width, objectImage->height);

	Pixel **imageB = copyImage(image, objectImage->width, objectImage->height);

	//estimateCenter(image, objectImage->width, objectImage->height);
	//findWrite(objectImage, image);

	int **binImage = getBinImage(image, objectImage->width, objectImage->height, 254);
	//image = createBinImage(binImage, objectImage->width, objectImage->height);

	//Circle c = {349, 439, 82}; //Catarata.ppm   1015X759 = 770385 (0.01064402863) 0.0163839089
	//Circle c = {391, 490, 140}; //Catarata2.ppm 1198X770 = 922460 (0.01517680983)
	//Circle c = {326, 371, 154}; //Normal.ppm    1167X739 = 862413 (0.01785687367)
	//Circle c = {205, 239, 64}; //Normal2.ppm    610X480  = 292800 (0.02185792349)
	//cataractDiagnosis (image, c);
	//drawCircle(image, objectImage->width, objectImage->height, c, 3);
	
	//printf("Aplicando filtro gaussiano\n");
	//applyGaussian(image, objectImage->width, objectImage->height);
	
	//printf("Aplicando filtro sobel\n");
	//applySobel(image, objectImage->width, objectImage->height);
	Circle estimatedCenter = estimateCenter(binImage, objectImage->width, objectImage->height);
	printf("%d, %d\n", estimatedCenter.x, estimatedCenter.y);
	Pixel **imageK = cropImage(imageB, &objectImage->width, &objectImage->height, estimatedCenter);
	Pixel **imagecopia = copyImage(imageK, objectImage->width, objectImage->height);
	applyGaussian(imageK, objectImage->width, objectImage->height);
	applySobel(imageK, objectImage->width, objectImage->height);
	int **binImage2 = getBinImage(imageK, objectImage->width, objectImage->height, 10);
	//image = createBinImage(binImage, objectImage->width, objectImage->height);
	
	//Circle c = findCircle(binImage2, objectImage->width, objectImage->height, 80,160);
	Circle c = findCircle(binImage2, objectImage->width, objectImage->height, 60,70);
	
	drawCircle(imagecopia, objectImage->width, objectImage->height, c, 1);
	writeImage(objectImage, imagecopia);
	//histogram(image, objectImage->width, objectImage->height);
	//printf("Binarizando a imagem\n");
	//int **binImage = getBinImage(image, objectImage->width, objectImage->height, 10);
	//image = createBinImage(binImage, objectImage->width, objectImage->height);
	
	
	//findWrite(objectImage, image);
	//writeDiagnosis(*objectImage, cataractDiagnosis(image, c), 60.0);

	//Circle c = findCircle(binImage, objectImage->width, objectImage->height, 60,120);
	
	//drawCircle(image, objectImage->width, objectImage->height, c, 1);

	//findWrite(objectImage, image);

	/*
	printf("Detectando pupila\n");
	//Circle c = {349, 439, 82};
	//Circle c = findCircle(binImage, objectImage->width, objectImage->height, 138,140);
	Circle c = findCircle(binImage, objectImage->width, objectImage->height, 150,160);
	//image = plotImage(binImage, objectImage->width, objectImage->height);
	
	printf("Escrevendo imagem em %s\n", objectImage->destination);
	drawCircle(imageB, objectImage->width, objectImage->height, c, 1);
	//writeImage(objectImage, image);
	//writeImage(objectImage, imageB);

	findWrite(objectImage, imageB);
*/
	
	return 0;

}
