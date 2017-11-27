#include "ioput.h"


typedef struct {
	int x, y, r;
} Circle;

/*Aplica o filtro de escala de cinza a imagem dada*/
void applyGrayScale(Pixel **image, int width, int height);

/*Retorna a copia da imagem dada*/
Pixel** copyImage(Pixel **image, int width, int height);


//void convolution(Pixel **image, int width, int height, int (*f)(int, int), int filterWidth, int filterHeight);


/*Aplica o filtro gaussiano (desfoque) à imagem dada*/
void applyGaussian(Pixel **image, int width, int height);


/*Aplica o filtro Sobel (detecção de arestas) à imagem dada*/
void applySobel(Pixel **image, int width, int height);

/*Retorna uma matriz do tipo inteiro com valores binários, determinados pelo threshold, com base na imagem dada,  em escala cinza. Pixels com valor cinza maior que o treshold são 1 e, menor, 0*/

int** getBinImage(Pixel **image, int width, int height, int threshold);

Pixel** createBinImage(int **image, int width, int height);

Circle findCircle(int **image, int width, int height);

double getPercentual (int vTotal, int vCurrent);

Pixel** squareImage(Pixel **image, int *width, int *height, int d);

Pixel** resizeImage(Pixel **image, int *width, int *height, int d);

int nearDivisible(int value, int d);

Pixel** plotImage (int **image, int width, int height);

void drawCircle (Pixel **image, int width, int height, Circle c, int margin);

double cataractDiagnosis (Pixel **image, Circle c);