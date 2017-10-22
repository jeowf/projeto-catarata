#include "ioput.h"
//#include "filters.h"
#include <stdio.h>

int main(int argc, char *argv[]){

	ObjectImage objectImage = newObjectImage(argc, argv);
	Pixel **image;
	printf("TEST: %s\n", objectImage.source);
	readImage(objectImage, image);
	return 0;
}