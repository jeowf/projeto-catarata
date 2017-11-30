#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//tipo booleano
typedef enum {
	false,
	true
}Boolean;
//tipo de dado que guarda informações sobre a imagem
typedef struct {
	char *source,
		 *format,
		 *destination,
 		 *out;
    int initialized,
    	width,
    	height;
} ObjectImage;
//tipo de dado que guarda os valores rgb de cada pixel da imagem
typedef struct {
	int r,
		g,
		b;		
} Pixel;

/*procura o índice em que uma palavra se encontra em um vetor de palavras */
int searchWord(char *words[], char *word, int t);

/*interpreta os argumentos passados na execução do programa final, independentemente da ordem*/
ObjectImage* newObjectImage(int argc, char *argv[]);

/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo ppm*/
Pixel** readPPM(ObjectImage *objectImage);
/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo pbm*/
Pixel** readPBM(ObjectImage *objectImage);
/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo pgm*/
Pixel** readPGM(ObjectImage *objectImage);
/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo bmp*/
Pixel** readBMP(ObjectImage *objectImage);
//Primeiro método de ler PPM
Pixel** readImageLegacy(ObjectImage *objectImage);

/*Cria um arquivo de imagem com base nos parâmentros passados (ObjectImage) e na imagem*/
void writeImage(ObjectImage *objectImage, Pixel **image);
/*Carrega a imagem com base no formato -f*/
Pixel** readImage(ObjectImage* objectImage);
/*escreve o diagóstico num arquivo .txt*/
void writeDiagnosis(ObjectImage objectImage, double v, double threshold);
