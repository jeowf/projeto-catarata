#include "ioput.h"
void jumpComment(FILE *file, char c){
	char hash, line[70];
	hash= fgetc(file);
	//printf("%c\n", hash);

	if (hash == c){
		fgets(line, 70, file);
		jumpComment(file, c);
	} else {
		fseek(file,-1,SEEK_CUR);
	}
}

//função pra verificar o tipo da imagem
Pixel** readImage(ObjectImage *objectImage){
	//compara o tipo e chama a função correspondente
	if(strcmp(objectImage->format, "ppm") == 0){
		readPPM(objectImage);
	}else if(strcmp(objectImage->format, "pbm") == 0){
		readPBM(objectImage);
	}else if(strcmp(objectImage->format, "pgm") == 0){
		readPGM(objectImage);
	}else if(strcmp(objectImage->format, "bmp") == 0){
		readBMP(objectImage);
	}
}
//função pra escrever com base no tipo da imagem
void writeImage(ObjectImage *objectImage, Pixel **image){
	//compara o tipo e chama a função correspondente
	if(strcmp(objectImage->format, "ppm") == 0){
		writePPM(objectImage, image);
	}else if(strcmp(objectImage->format, "pbm") == 0){
		writePBM(objectImage, image);
	}else if(strcmp(objectImage->format, "pgm") == 0){
		writePGM(objectImage, image);
	}else if(strcmp(objectImage->format, "bmp") == 0){
		writeBMP(objectImage, image);
	}
}
//procura uma palavra num array de palavras
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
		objectImage->out		 = "out.ppm";
		objectImage->initialized = true;
	}
	return objectImage;
}

Pixel** readPPM(ObjectImage *objectImage){
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
		int mp;
		fscanf(img, "%d", &mp);
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

Pixel** readPBM(ObjectImage *objectImage){
	int i, j;
	char hash,line[70];
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
 		printf( "Erro na abertura do arquivo!\n" );
 	}else{
 		fgets(line, 70, img);

 		//char *TESTE = jumpComment(img, '#');
 		/*
 		objectImage->width = atoi (jumpComment(img, '#'));
 		printf("%d\n", objectImage->width );
 		objectImage->height = atoi (jumpComment(img, '#'));
 		printf("%d\n", objectImage->height );

 		mp = atoi (jumpComment(img, '#'));*/

 		jumpComment(img, '#');
 		fscanf(img, "%d", &objectImage->width);
 		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->height);
		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		//preencher matriz de pixel com seus valores rgb
		for (i = 0; i < objectImage->height; i++){
			for (j = 0; j < objectImage->width; j++){
				int byte;
				fscanf(img, "%1d", &byte);
					if(byte == 0){
						image[i][j].r = 255;
						image[i][j].g = 255;
						image[i][j].b = 255;
					}else{
						image[i][j].r = 0;
						image[i][j].g = 0;
						image[i][j].b = 0;
					}
				}
			}

		//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
 	}
	fclose(img);
	return image;
}

Pixel** readPGM(ObjectImage *objectImage){
	int i, j;
	char hash,line[70],header[70],eol;;
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
 		printf( "Erro na abertura do arquivo!\n" );
 	}else{

 		fscanf(img, "%s", header);

 		fscanf(img, "%c", &eol);

		//lê largura e altura da imagem
		jumpComment(img, '#');
 		fscanf(img, "%d", &objectImage->width);
 		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->height);
		int mp;
		jumpComment(img, '#');
		fscanf(img, "%d", &mp);
		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		//preencher matriz de pixel com seus valores rgb
		for (i = 0; i < objectImage->height; i++){
			for (j = 0; j < objectImage->width; j++){
				int byte;
				fscanf(img, "%d", &byte);
				image[i][j].r = byte;
				image[i][j].g = byte;
				image[i][j].b = byte;
			}
		}

		//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
 	}
	fclose(img);
	return image;
}

Pixel** readBMP(ObjectImage *objectImage){
	int i, j,mp;
	char hash,line[70],header [70],eol;
	Pixel **image,**teste;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
 		printf( "Erro na abertura do arquivo!\n" );
 	}else{
 		//PREPARAR METODO DE APAGAR COMENTARIOS
 		//fgets(header,70,img);
 			fseek(img, 18, SEEK_SET); 
    		fread(&objectImage->width, sizeof(int), 1,img); 
    		fread(&objectImage->height, sizeof(int), 1, img); 
 			fseek(img, 0, SEEK_SET);
 			fscanf(img, "%s", header); 
 		/*
 		while(mp != EOF){
 			fscanf(img, "%c", &mp);
 			printf("%c\n", mp);
 		}
 		*/
 
 		//char *TESTE = jumpComment(img, '#');
 		/*
 		objectImage->width = atoi (jumpComment(img, '#'));
 		printf("%d\n", objectImage->width );
 		objectImage->height = atoi (jumpComment(img, '#'));
 		printf("%d\n", objectImage->height );

 		mp = atoi (jumpComment(img, '#'));*/
		
		//lê largura e altura da imagem
		//matriz pixel
		image = calloc( objectImage->height,sizeof(Pixel));
		for (i = 0; i <  objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
			int up = 0;
		//preencher matriz de pixel com seus valores rgb
				for (i = objectImage->height -1; i >=0; i--){
					fscanf(img, "%c", &up);
					fscanf(img, "%c", &up);	
					for (j = 0; j < objectImage->width; j++){
				/*image[i][j].r = atoi(jumpComment(img, '#'));
				image[i][j].g = atoi(jumpComment(img, '#'));
				image[i][j].b = atoi(jumpComment(img, '#'));*/
					fscanf(img, "%c", &image[i][j].r);
					fscanf(img, "%c", &image[i][j].g);
					fscanf(img, "%c", &image[i][j].b);	
				}
					fscanf(img, "%c", &up);	
						
			}
			teste = calloc(objectImage->height,sizeof(Pixel));
				for (i = 0; i < objectImage->height; i++)
				teste[i] = calloc(80,sizeof(Pixel));


			for (i = objectImage->height -1 ; i >=0; i--){
				for (j = 0; j < (objectImage->width - 26); j++){
						teste[i][j] = image[i][j];
						image[i][j] = image[i][j+26];
						image[i][j+26] = teste[i][j];

				}
			}

		//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
	fclose(img);
	free(teste);
	return image;
	}
}

void writePPM(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->out,"w");

	fprintf(img, "%s\n%d %d\n255\n", 
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

void writeBMP(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->out,"w");

	fprintf(img, "%s\n%d %d\n255\n", 
			"P3",
			objectImage->width, 
			objectImage->height);
	//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
	
	for (i = 0; i < objectImage->height; i++){
		for (j = 0; j < objectImage->width; j++){
			fprintf(img, "%d\n%d\n%d\n", 
					image[i][j].b,
					image[i][j].g,
					image[i][j].r);
		}
	}
	fclose(img);
	return ;
}

void writePBM(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->out,"w");

	fprintf(img, "%s\n%d %d\n1\n", 
			"P3",
			objectImage->width, 
			objectImage->height);
	//printf("p[%d][%d] - r:%d g:%d b:%d\n", 758, 1014, image[758][1014].r, image[758][1014].g, image[758][1014].b );
	
	for (i = 0; i < objectImage->height; i++){
		for (j = 0; j < objectImage->width; j++){
			fprintf(img, "%d %d %d \n", 
					image[i][j].r,
					image[i][j].g,
					image[i][j].b);
		}
	}
	fclose(img);
	return ;
}

void writePGM(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->out,"w");

	fprintf(img, "%s\n%d %d\n255\n", 
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

void writeDiagnosis(ObjectImage objectImage, double v, double threshold){
	FILE *file; 
	file = fopen(objectImage.destination,"w");
	if (v >= threshold)
		fprintf(file, "O olho POSSUI catarata (%.2lf %% de comprometimento da pupila).\n", v);
	else
		fprintf(file, "O olho NÃO possui catarata.\n");
	fclose(file);
}
