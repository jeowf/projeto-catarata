#include "ioput.h"


typedef struct {
	int x, y, r;
} Circle;

/* */
void applyGrayScale(Pixel **image, int width, int height);

/* */
Pixel** copyImage(Pixel **image, int width, int height);

/* */
//void convolution(Pixel **image, int width, int height, int (*f)(int, int), int filterWidth, int filterHeight);

/* */
//int applyGaussian(int x, int y);
void applyGaussian(Pixel **image, int width, int height);


/* */
void applySobel(Pixel **image, int width, int height);

/* */
//int maxmin(int p, int min, int max);

/*AJEITAR ESSE COMMENT
Retorna uma matriz inteira referente À iamgem original levando em considera~ção q essa está numa escala cina*/
int** getBinImage(Pixel **image, int width, int height, int threshold);

Pixel** createBinImage(int **image, int width, int height);

Circle findCircle(int **image, int width, int height);

double getPercentual (int vTotal, int vCurrent);

Pixel** squareImage(Pixel **image, int *width, int *height, int d);

Pixel** resizeImage(Pixel **image, int *width, int *height, int d);

int nearDivisible(int value, int d);

Pixel** plotImage (int **image, int width, int height);

void drawCircle (Pixel **image, int width, int height, Circle c, int margin);