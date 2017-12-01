#include "ioput.h"
//pula os comentários com base no caracter passado
void jumpComment(FILE *file, char c){
	char hash, line[70];
	//recebe o proximo caractere do arquivo
	hash= fgetc(file);
	//percorre o arquivo de imagem comparado os valores
	if (hash == c){
		//coloca leitor no fim da linha caso seja comentário
		fgets(line, 70, file);
		//chama novamente a função
		jumpComment(file, c);
	} else {
		//volta uma posição na leitura do arquivo
		fseek(file,-1,SEEK_CUR);
	}
}

//função pra verificar o tipo da imagem
Pixel** readImage(ObjectImage *objectImage){
	//compara o tipo e chama a função correspondente
	if(strcmp(objectImage->format, "ppm") == 0){
		readPPM(objectImage);
	}else if(strcmp(objectImage->format, "pgm") == 0){
		readPGM(objectImage);
	}else if(strcmp(objectImage->format, "bmp") == 0){
		readBMP(objectImage);
	}
}
//procura uma palavra num array de palavras
int searchWord(char *words[], char *word, int t){
	int i;
	//percorre o vetor de pavaras
	for (i = 0; i < t; i++){
		//verifica se a palavrase enconta no vetor
		if (strcmp(words[i], word) == 0)
			//retorna o indice que a palavra se encontra dento do array
			return i;
	}
	return -1;
}
//retorna um o objectImage com seus parametros preenchidos.
ObjectImage* newObjectImage(int argc, char *argv[]){
	ObjectImage *objectImage = calloc(1, sizeof(ObjectImage));
	//verifica a quantidade de parametros
	if (argc != 7){
		objectImage->initialized = false;
		fprintf(stderr, "Há parâmetros faltando.\n");
		exit(0);
	} else {//inicializa os valores
		objectImage->source      = argv[searchWord(argv, "-i", argc) + 1];//caminho da imagem
		objectImage->format      = argv[searchWord(argv, "-f", argc) + 1];//formato
		objectImage->destination = argv[searchWord(argv, "-o", argc) + 1];//diagnóstico
		objectImage->out		 = "out.ppm";//imagem ppm gerada no fim
		objectImage->initialized = true;
	}
	return objectImage;
}
//lê uma imagem no formato PPM (P6 OU P3) e retorna uma matriz de pixels
Pixel** readPPM(ObjectImage *objectImage){
	int i, j,mp;//contadores e pixel máximo
	char hash,line[70],header[70],eol;//auxiliares
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
		fprintf(stderr, "Erro na abertura de arquivo.\n");
		exit(0);
	}else{
		fscanf(img, "%s", header);//lê o tipo de imagem

		fscanf(img, "%c", &eol);//pula o '\n'
 		//lê largura,altura  e pixel máximo da imagem
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->width);
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->height);
		jumpComment(img, '#');
		fscanf(img, "%d", &mp);
		if(strcmp(header, "P6") == 0){//pula mais um '\n' para imagens binárias
			fscanf(img, "%c", &eol);
		}

		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		if (image == NULL){
			fprintf(stderr, "Erro na alocação dinamica.\n");
			exit(0);
		}else{
			for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		}
		//preencher matriz de pixel com seus valores rgb coom base no cabeçalho da imagem
		if(strcmp(header, "P3") == 0){
			for (i = 0; i < objectImage->height; i++){
				for (j = 0; j < objectImage->width; j++){
					//atribui 3 bytes a um pixel(RGB)
					fscanf(img, "%d", &image[i][j].r);
					fscanf(img, "%d", &image[i][j].g);
					fscanf(img, "%d", &image[i][j].b);
				}
			}		
		}else{
			for (i = 0; i < objectImage->height; i++){
				for (j = 0; j < objectImage->width; j++){
					fscanf(img, "%c", &image[i][j].r);
					fscanf(img, "%c", &image[i][j].g);
					fscanf(img, "%c", &image[i][j].b);
				}
			}

		}

	}
	fclose(img);
	return image;
}
//lê uma imagem no formato PBM (P1) e retorna uma matriz de pixels
Pixel** readPBM(ObjectImage *objectImage){
	int i, j;//contador
	char hash,line[70];//auxiliares
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
		fprintf(stderr, "Erro na abertura de arquivo.\n");
		exit(0);
	}else{
		fgets(line, 70, img);
		//altura e largura
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->width);
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->height);
		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		if (image == NULL){
			fprintf(stderr, "Erro na alocação dinamica.\n");
			exit(0);
		}else{
			for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		}
		//preencher matriz de pixel com seus valores rgb com 0 ou 255;
		for (i = 0; i < objectImage->height; i++){
			for (j = 0; j < objectImage->width; j++){
				int byte;
				fscanf(img, "%1d", &byte);
				if(byte == 0){//atribui branco e preto aos pixels
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
	}
	fclose(img);
	return image;
}
//lê uma imagem no formato PGM (P3) e retorna uma matriz de pixels
Pixel** readPGM(ObjectImage *objectImage){
	int i, j;
	char hash,line[70],header[70],eol;;
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
		fprintf(stderr, "Erro na abertura de arquivo.\n");
		exit(0);
	}else{
		//pega a informação se é binaria ou não
		fscanf(img, "%s", header);

		fscanf(img, "%c", &eol);//pula '\n'

		//lê largura e altura da imagem e pixel máximo
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->width);
		
		jumpComment(img, '#');
		fscanf(img, "%d", &objectImage->height);

		int mp;
		jumpComment(img, '#');
		fscanf(img, "%d", &mp);

		//matriz pixel
		image = calloc(objectImage->height,sizeof(Pixel));
		if (image == NULL){
			fprintf(stderr, "Erro na alocação dinamica.\n");
			exit(0);
		}else{
			for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		}
		//preencher matriz de pixel com seus valores rgb com base no header
			if(strcmp(header, "P2") == 0){
			for (i = 0; i < objectImage->height; i++){
				for (j = 0; j < objectImage->width; j++){
					int byte = 0;
					//lê o byte e replica pro RGB
					fscanf(img, "%d", &byte);
					image[i][j].r = byte;
					image[i][j].g = byte;
					image[i][j].b = byte;

				}
			}		
		}else{
			for (i = 0; i < objectImage->height; i++){
				for (j = 0; j < objectImage->width; j++){
					int byte = 0;
					//lê o byte em char e depois replica o valor inteiro pro RGB
					fscanf(img, "%c", &byte);
					image[i][j].r = byte;
					image[i][j].g = byte;
					image[i][j].b = byte;
				}
			}

		}


	}
	fclose(img);
	return image;
}
//lê uma imagem no formato BMP e retorna uma matriz de pixels
Pixel** readBMP(ObjectImage *objectImage){
	int i, j,mp;// contadores e pixel máximo
	char hash,line[70],header [70],eol;//auxliares 
	Pixel **image,**teste;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
		fprintf(stderr, "Erro na abertura de arquivo.\n");
		exit(0);
	}else{
		//lê o cabeçalho do arquivo
		fseek(img, 18, SEEK_SET); 
		//lê altura e largura
		fread(&objectImage->width, sizeof(int), 1,img); 
		fread(&objectImage->height, sizeof(int), 1, img);
		//coloca o poteiro de leitura para o ultimo byte do cabeçalho 
		fseek(img,96, SEEK_CUR);
		int up=0;
		int padding = objectImage->width % 4;
		//aloca a matriz de pixels
		image = calloc(objectImage->height,sizeof(Pixel));
		if (image == NULL){
			fprintf(stderr, "Erro na alocação dinamica.\n");
			exit(0);
		}else{
			for (i = 0; i < objectImage->height; i++)
				image[i] = calloc(objectImage->width,sizeof(Pixel));
		}
		//faz a leitura dos pixels de baixo para cima e da esquerda para a direita
		for (i = objectImage->height - 1; i >=0; i--){
			for (j = 0; j < objectImage->width; j++){
				fread(&image[i][j].b, sizeof(char), 1,img); 
				fread(&image[i][j].g, sizeof(char), 1,img); 
				fread(&image[i][j].r, sizeof(char), 1,img); 	
			}
			/*como o bmp necessita que as linhas sejam multiplas de 4 ele adiciona
			caracteres quaisquer para completar as linhas com base no resto da divisão
			da linha por 4, assim precisamos saber qual esse resto para desconsiderar esses
			caracteres adicionados ao final de cada linha.
			*/
			if(padding % 4 !=0){
				fread(&up,padding, 1,img); 
			}	
		}
	}

	fclose(img);
	return image;
}
//escreve a imagem lida em um arquivo .ppm
void writeImage(ObjectImage *objectImage, Pixel **image){
	int i, j;
	FILE *img; 
	img = fopen(objectImage->out,"w");

	fprintf(img, "%s\n%d %d\n255\n", 
		"P3", 
		objectImage->width, 
		objectImage->height);
	
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
//escreve o diagnóstico com base na porcentagem de pixels com e sem catarata
void writeDiagnosis(ObjectImage objectImage, double v, double threshold){
	FILE *file; 
	file = fopen(objectImage.destination,"w");
	//verifica se a porcentagem é maior q o limiar
	if (v >= threshold)
		fprintf(file, "O olho POSSUI catarata (%.2lf %% de comprometimento da pupila).\n", v);
	else
		fprintf(file, "O olho NÃO possui catarata.\n");
	fclose(file);
}

Pixel** readImageLegacy(ObjectImage *objectImage){
	int i, j;
	char hash,line[70];
	Pixel **image;

	FILE *img; //variavel para ler as imagens;
	img = fopen(objectImage->source,"r+");//abre a imagem e habilita leitura e escrita.
	
	//verifica se houve erro na abertura do arquivo
	if( img == NULL ) {
 		fprintf(stderr, "Erro na abertura de arquivo.\n");
		exit(0);
 	}else{
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
		if (image == NULL){
			fprintf(stderr, "Erro na alocação dinamica.\n");
			exit(0);
		}else{
			for (i = 0; i < objectImage->height; i++)
			image[i] = calloc(objectImage->width,sizeof(Pixel));
		}
		//preencher matriz de pixel com seus valores rgb
		for (i = 0; i < objectImage->height; i++){
			for (j = 0; j < objectImage->width; j++){
				fscanf(img, "%d", &image[i][j].r);
				fscanf(img, "%d", &image[i][j].g);
				fscanf(img, "%d", &image[i][j].b);
			}
		}
		
 	}
	fclose(img);
	return image;
}