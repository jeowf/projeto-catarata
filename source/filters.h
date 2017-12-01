#include "ioput.h"

typedef enum {
	FLASH_AREA 					 = 32,  //tamanho dos lados da área de detecção de flash
	PIXEL_CATARACT_MIN_THRESHOLD = 76,  //limiar mínimo de pixel de catarata
	PIXEL_CATARACT_MAX_THRESHOLD = 230, //limite de pixel de catarata
	DIAGNOSIS_CATARACT_THRESHOLD = 60   //limiar de detecção de catarata
}Constants;

//Descreve um círculo
typedef struct {
	int x, y, r;
} Circle;

/*
Retorna uma copia da imagem dada
Parâmetros: imagem, largura e altura
Retorno: imagem 
*/
Pixel** copyImage(Pixel **image, int width, int height);

/* Aplica o filtro de escala de cinza a imagem dada
Parâmetros: imagem, largura, altura
Retorno: vazio */
void applyGrayScale(Pixel **image, int width, int height);

/* Aplica o filtro gaussiano (desfoque) à imagem dada
Parâmetros: imagem, largura, altura
Retorno: vazio */
void applyGaussian(Pixel **image, int width, int height);

/*Aplica o filtro Sobel (detecção de arestas) à imagem dada
Parâmetros: imagem, largura, altura
Retorno: vazio */
void applySobel(Pixel **image, int width, int height);

/*Retorna uma matriz do tipo inteiro com valores binários, determinados pelo threshold, com 
base na imagem dada,  em escala cinza. Pixels com valor cinza maior que o treshold são 1 e, menor, 0
Parâmetros: imagem, largura, altura, limiar de binarização
Retorno: imagem binária*/
int** getBinImage(Pixel **image, int width, int height, int threshold);

/*Encontra um círculo na imagem binária dada
Parâmetros: imagem, largura, altura, raio mínimo, raio máximo
Retorno: círculo */
Circle findCircle(int **image, int width, int height, int rmin, int rmax);

/*Calcula o percentual de um valor em relação ao total 
Parâmetros: valor total, valor atual
Retorno: valor percentual */
double getPercentual (int vTotal, int vCurrent);

/*Recorta a imagem passada na região que comporta o círculo passado como parâmetro 
Parâmetros: imagem, largura, altura, círculo que estará contido na imagem cortada
Retorno: imagem cortada*/
Pixel** cropImage(Pixel **image, int *width, int *height, Circle c);

/*Desenha o círculo dado na imagem fornecida
Parâmetros: imagem, largura, altura, círculo que será desenhado, largura da linha, opacidade ao entorno do círculo
Retorno: nulo*/
void drawCircle (Pixel **image, int width, int height, Circle c, int margin, float opacity);

/*Retorna o percentual de comprometimento da pupila 
Parâmetros: image, pupila, flash
Retorno: percentual de comprometimento da pupila*/
double cataractDiagnosis (Pixel **image, Circle pupil, Circle flash);

/*Estima o centro da íris com base na posição do flash da imagem binária dada
Parâmetros: imagem, largura, altura
Retorno: círculo (flash) */
Circle estimateCenter(int **image, int width, int height);

/*Encontra a íris com base na coordenada do flash na imagem binária dada
Parâmetros: imagem, largura, altura, flash
Retorno: círculo (íris)*/
Circle fastFindCircle(int **image, int width, int height, Circle c);

/*Calcula distância de dois pontos 
Parâmetros: coordenadas dos pontos P1 (x1, y1) e P2 (x2,y2)
Retorno: distância*/
int distanceOfPoints(int x1, int y1, int x2, int y2);

/*Encontra uma aresta na imagem binária dada 
Parâmetros: imagem, posição de origem X e Y, valor a ser procurado, deslocamento horizontal e vertical
Retorno: distância*/
int findEdge (int **image, int origX, int origY, int value, int horizontal, int vertical);

/*Retorna o valor do pixel de maior intensidade numa imagem dada
Parâmetros: imagem, largura, altura
Retorno: intensidade máxima da imagem*/
int getMaxPixelIntensity(Pixel **image, int width, int height);

/*Método recursivo que preenche uma região com um valor numa imagem binária dada 
Parâmetros: imagem, posição de origem X e Y, valor que será substituído, novo valor
Retorno: nulo*/
void fill (int **image, int origX, int origY, int v1, int v2);

/*Conta a quantidade de pixels de um valor numa imagem binária dada 
Parâmetros: imagem, largura, altura, valor de contagem
Retorno: quantidade*/
int countPixels(int **image, int width, int height, int v);

/*Exclui os valores fora do círculo passado na imagem binária dada 
Parâmetros: imagem, largura, altura, círculo
Retorno: nulo*/
int** excludeOutsideCircle(int **image, int width, int height, Circle c);
