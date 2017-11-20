#include <stdio.h>
#include <string.h>

typedef enum {
	false,
	true
}Boolean;

typedef struct {
	char *source,
		 *format,
		 *destination;
    int initialized,
    	width,
    	height;
} ObjectImage;

typedef struct {
	int r,
		g,
		b;	
} Pixel;

/*procura o índice em que uma palavra se encontra em um vetor de palavras */
int searchWord(char *words[], char *word, int t);

/*interpreta os argumentos passados na execução do programa final, independentemente da ordem*/
ObjectImage* newObjectImage(int argc, char *argv[]);

/* */
Pixel** readImage(ObjectImage *objectImage);

/* */
void writeImage(ObjectImage *objectImage, Pixel **image);
