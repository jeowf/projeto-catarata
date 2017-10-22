#include "ioput.h"

int searchWord(char *words[], char *word, int t){
	int i;
	for (i = 0; i < t; i++){
		if (strcmp(words[i], word) == 0)
			return i;
	}
	return -1;
}

ObjectImage newObjectImage(int argc, char *argv[]){
	ObjectImage objectImage;
	if (argc != 7){
		objectImage.initialized = false;
	} else {
		objectImage.source      = argv[searchWord(argv, "-i", argc) + 1];
		objectImage.format      = argv[searchWord(argv, "-f", argc) + 1];
		objectImage.destination = argv[searchWord(argv, "-o", argc) + 1];
		objectImage.initialized = true;
	}
	return objectImage;
}

void readImage(ObjectImage objectImage, Pixel **image){
	if (objectImage.initialized == true){
		FILE *img; //variavel para ler as imagens;
		img = fopen(objectImage.source,"r+");//abre a imagem e habilita leitura e escrita.
		
		//verifica se houve erro na abertura do arquivo
		if( img == NULL ) {
	 		printf( "Erro na abertura do arquivo!\n" );
	 	}
	}
	return ;
}