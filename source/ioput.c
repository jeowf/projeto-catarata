#include "ioput.h"

int searchWord(char *words[], char *word, int t){
	int i;
	for (i = 0; i < t; i++){
		if (strcmp(words[i], word) == 0)
			return i;
	}
	return -1;
}

ObjectImage* newObjectImage(int argc, char *argv[]){
	ObjectImage *objectImage = calloc(1, sizeof(ObjectImage));
	if (argc != 7){
		objectImage->initialized = false;
	} else {
		objectImage->source      = argv[searchWord(argv, "-i", argc) + 1];
		objectImage->format      = argv[searchWord(argv, "-f", argc) + 1];
		objectImage->destination = argv[searchWord(argv, "-o", argc) + 1];
		objectImage->initialized = true;
	}
	return objectImage;
}

Pixel** readImage(ObjectImage *objectImage){
	int i, j;
	char hash,line[70];
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
 		printf( "Erro na abertura do arquivo!\n" );
 	}else{
 		//PREPARAR METODO DE APAGAR COMENTARIOS
 		while(hash != EOF){//enquanto a variavel hash for diferente de -1 ele ler o arquivo
			if(hash == '#'){
				fgets(line, 70, img);//lê uma linha inteira que começa com #
				break;
			}else{
				hash = getc(img);//lê o proximo elemento do arquivo
				if(hash == EOF){//caso n exista # no arquivo ele volta para o inicio e seta a leitura depois do 'P3';
				fseek(img, 3, SEEK_SET);// seta a leitura a partir do 3º byte
				}
			}
		}

		//lê largura e altura da imagem
		fscanf(img, "%d", &objectImage->width);
		fscanf(img, "%d", &objectImage->height);

		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));

		//preencher matriz de pixel com seus valores rgb
		for (i = 0; i < objectImage->height; i++){
			for (j = 0; j < objectImage->width; j++){
				fscanf(img, "%d", &image[i][j].r);
				fscanf(img, "%d", &image[i][j].g);
				fscanf(img, "%d", &image[i][j].b);
			}
		}
		

		//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
 	}
	fclose(img);
	return image;
}

void writeImage(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->destination,"w");

	fprintf(img, "%s\n%d %d\n", 
			"P3", 
			objectImage->width, 
			objectImage->height);
	//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
	
	for (i = 0; i < objectImage->height; i++){
		for (j = 0; j < objectImage->width; j++){
			fprintf(img, "%d\n%d\n%d\n", 
					image[i][j].r,
					image[i][j].g,
					image[i][j].b);

		}
	}
	fclose(img);
	return ;
}