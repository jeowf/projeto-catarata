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

/*Procura o índice em que uma palavra se encontra em um vetor de palavras
Parâmetros: array de palavras, palavra,  tamanho do array
Retorno: indice em que a palavra se encontra no vetor */
int searchWord(char *words[], char *word, int t);

/*interpreta os argumentos passados na execução do programa final, independentemente da ordem
Parâmetros: quantidade de patametros, array com os parametros
Retorno: uma imagem com todos os valores inicializados*/
ObjectImage* newObjectImage(int argc, char *argv[]);

/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo ppm
Parâmetros: Objeto de Imagem
Retorno: uma matriz de pixels RGB*/
Pixel** readPPM(ObjectImage *objectImage);

/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo pbm
Parâmetros: Objeto de Imagem
Retorno: uma matriz de pixels RGB*/
Pixel** readPBM(ObjectImage *objectImage);

/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo pgm
Parâmetros:Objeto de Imagem
Retorno: uma matriz de pixels RGB*/
Pixel** readPGM(ObjectImage *objectImage);

/*Retorna uma imagem com base nos parâmetros passados (ObjectImage) do tipo bmp
Parâmetros: Objeto de Imagem
Retorno: uma matriz de pixels RGB*/
Pixel** readBMP(ObjectImage *objectImage);

/*Primeiro método de ler PPM (usado em caso de erro dos demais métodos de leitura)
Parâmetros: Objeto de Imagem
Retorno: uma matriz de pixels RGB*/
Pixel** readImageLegacy(ObjectImage *objectImage);

/*Cria um arquivo de imagem com base nos parâmentros passados (ObjectImage) e na imagem
Parâmetros: uma imagem, matriz de pixels RGB
Retorno: não possui retorno, ao fim cria um arquivo .ppm*/
void writeImage(ObjectImage *objectImage, Pixel **image);

/*Carrega a imagem com base no formato -f
Parâmetros: uma objeto de imagem
Retorno: a função correspondente ao tipo da imagem de entrada*/
Pixel** readImage(ObjectImage* objectImage);

/*Escreve o diagóstico num arquivo .txt
Parâmetros: uma objeto de imagem, porcentagem,limiar
Retorno: nada, ao fim da execução cria um arquivo.txt*/
void writeDiagnosis(ObjectImage objectImage, double v, double threshold);
