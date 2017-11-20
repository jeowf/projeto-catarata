#include "ioput.h"

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

//void findCircle(Pixel **image, int width, int height);