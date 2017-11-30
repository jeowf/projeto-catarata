#include "ioput.h"

typedef enum {
	FLASH_AREA 					 = 32,
	R_CROP_MAX 					 = 210,
	PIXEL_CATARACT_MIN_THRESHOLD = 90,
	PIXEL_CATARACT_MAX_THRESHOLD = 230,
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

void drawCircle (Pixel **image, int width, int height, Circle c, int margin, float opacity);

double cataractDiagnosis (Pixel **image, Circle pupil, Circle flash);

Circle estimateCenter(int **image, int width, int height);

void histogram(Pixel **image, int width, int height);

Circle fastFindCircle(int **image, int width, int height, Circle c);

int distanceOfPoints(int x1, int y1, int x2, int y2);

int findEdge (int **image, int origX, int origY, int value, int horizontal, int vertical);

int isEdgeValid(int **image, int origX, int origY, int value, int horizontal, int vertical, int range);

int getMaxPixelIntensity(Pixel **image, int width, int height);

void fill (int **image, int origX, int origY, int v1, int v2);

void fillImage (Pixel **image, Circle c, int threshold, int v);

int countPixels(int **image, int width, int height, int v);

int getMediumPixel (Pixel **image, Circle c);

void excludeOutsideCircle(int **image, int width, int height, Circle c);
