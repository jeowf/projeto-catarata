#include "ioput.h"

typedef enum {
	FLASH_AREA 					 = 32,
	R_CROP_MAX 					 = 210,
	PIXEL_CATARACT_THRESHOLD 	 = 96,
	DIAGNOSIS_CATARACT_THRESHOLD = 60
}Constants;

typedef struct {
	int x, y, r;
} Circle;

/*Retorna a copia da imagem dada*/
Pixel** copyImage(Pixel **image, int width, int height);

/*Aplica o filtro de escala de cinza a imagem dada*/
void applyGrayScale(Pixel **image, int width, int height);

/*Aplica o filtro gaussiano (desfoque) à imagem dada*/
void applyGaussian(Pixel **image, int width, int height);


/*Aplica o filtro Sobel (detecção de arestas) à imagem dada*/
void applySobel(Pixel **image, int width, int height);

/*Retorna uma matriz do tipo inteiro com valores binários, determinados pelo threshold, com base na imagem dada,  em escala cinza. Pixels com valor cinza maior que o treshold são 1 e, menor, 0*/

int** getBinImage(Pixel **image, int width, int height, int threshold);

Pixel** createBinImage(int **image, int width, int height);

Circle findCircle(int **image, int width, int height, int rmin, int rmax);

double getPercentual (int vTotal, int vCurrent);

//Pixel** cropImage(Pixel **image, int *width, int *height);
Pixel** cropImage(Pixel **image, int *width, int *height, Circle c);

Pixel** plotImage (int **image, int width, int height);

void drawCircle (Pixel **image, int width, int height, Circle c, int margin);

double cataractDiagnosis (Pixel **image, Circle c);

Circle estimateCenter(int **image, int width, int height);

void histogram(Pixel **image, int width, int height);