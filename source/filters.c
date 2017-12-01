#include "filters.h"
#include <math.h>

//Realiza uma cópia da imagem passada por parâmetro
Pixel** copyImage(Pixel **image, int width, int height){
	int i, j; //contadores
	Pixel **newImage = calloc(height,sizeof(Pixel)); //nova imagem

	//verifica se houve erro na alocação
	if (newImage == NULL){
		fprintf(stderr, "Falha de alocação\n");
		exit(0);
	}

	//aloca a nova imagem, copiando para si os valores da imagem passada por parâmetro
	for (i = 0; i < height; i++){
		newImage[i] = calloc(width,sizeof(Pixel));
		for (j = 0; j< width; j++)
			newImage[i][j] = image[i][j];
	}

	return newImage;
}

//Aplica o filtro de escala cinza à imagem passada por parâmetro
void applyGrayScale(Pixel **image, int width, int height){
	int i, j; //contadores

	//percorre a imagem
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			//calcula o valor cinza do pixel
			int pixelValue = image[i][j].r * 0.30 +
							 image[i][j].g * 0.59 +
							 image[i][j].b * 0.11 ;

			//atribui o valor calculado aos canais RGB da imagem	
			image[i][j].r = pixelValue;
			image[i][j].g = pixelValue;
			image[i][j].b = pixelValue;
		}
	}
}

void applyGaussian(Pixel **image, int width, int height){
	int x, y, a, b, //contadores
		tam = 5, //tamanho da mascara
		m = tam/2, //delimitações da mascara
		weight = 159, //peso da mascara
		//mascara gaussiana
		mask[5][5] = {{2, 4, 5, 4, 2},
					  {4, 9, 12, 9, 4},
					  {5, 12, 15, 12, 5},
					  {4, 9, 12, 9, 4},
					  {2, 4, 5, 4, 2}};

	//realiza uma cópia da imagem passada por parâmetro para usá-la nos cálculos
	Pixel **imageOrig = copyImage(image, width, height);

	//percorre a imagem
	for (x = 0 + 2; x < height - 2; x++){
		for (y = 0 + 2; y < width - 2; y++){
			
			//verifica os pixels da vizinhança com base no tamanho da máscara
			for (a = -m; a <= m; a++){
				for (b = -m; b <= m; b++){
					//calcula o valor do novo pixel com base nos pesos da máscara
					//multiplicando os pixels da vizinhança
					image[x][y].r += mask[a+m][b+m]*imageOrig[x+a][y+b].r;
					image[x][y].g += mask[a+m][b+m]*imageOrig[x+a][y+b].g;
					image[x][y].b += mask[a+m][b+m]*imageOrig[x+a][y+b].b;

				}
			}

			//divide o valor do pixel pelo peso total da máscara
			image[x][y].r /= weight;
			image[x][y].g /= weight;
			image[x][y].b /= weight;

		}
	}

	return;
}

void applySobel(Pixel **image, int width, int height){
	int x, y, a, b,
		tam = 3,
		m = tam/2,
		//mascara horizontal
		maskx[3][3] = {{-1, 0, 1},
					  {-2, 0, 2},
					  {-1, 0, 1}},
		//mascara vertical
		masky[3][3] = {{1, 2, 1},
					  {0, 0, 0},
					  {-1, -2, -1}};
	Pixel gx, gy, g; //pixels auxiliares

	//realiza uma cópia da imagem passada por parâmetro para usá-la nos cálculos
	Pixel **imageOrig = copyImage(image, width, height);

	//percorre a imagem
	for (x = 0 + 1; x < height - 1; x++){
		for (y = 0 + 1; y < width - 1; y++){
			
			//reseta o valor dos pixels auxiliares
			gx.r = 0; gx.b = 0; gx.g = 0;
			gy.r = 0; gy.b = 0; gy.g = 0;

			//calcula o valor do novo pixel com base nos pesos da máscara
			//multiplicando os pixels da vizinhança
			for (a = -m; a <= m; a++){
				for (b = -m; b <= m; b++){
					//calcula as arestas horizontais
					gx.r += maskx[a+m][b+m]*imageOrig[x+a][y+b].r;
					gx.g += maskx[a+m][b+m]*imageOrig[x+a][y+b].g;
					gx.b += maskx[a+m][b+m]*imageOrig[x+a][y+b].b;

					//calcula as arestas verticais
					gy.r += masky[a+m][b+m]*imageOrig[x+a][y+b].r;
					gy.g += masky[a+m][b+m]*imageOrig[x+a][y+b].g;
					gy.b += masky[a+m][b+m]*imageOrig[x+a][y+b].b;
				}
			}

			//calcula a magnitude do novo pixel
			g.r = sqrt(gx.r*gx.r + gy.r*gy.r);
			g.g = sqrt(gx.g*gx.g + gy.g*gy.g);
			g.b = sqrt(gx.b*gx.b + gy.b*gy.b);

			//substitui o pixel calculado pelo respectivo pixel na imagem passada como parâmetro
			image[x][y] = g;
		}
	}

	return;
}

//Recorta a imagem passada como parâmetro na região base no círculo passado como parâmetro
Pixel** cropImage(Pixel **image, int *width, int *height, Circle c){
	int i, j, //contadores
		size = c.r * 2; //tamanho dos lados da nova imagem
	
	int verticalI 	= c.y - c.r, //valor mínimo Y da nova imagem
		verticalF 	= c.y + c.r, //valor máximo Y da nova imagem
		horizontalI = c.x - c.r, //valor mínimo X da nova imagem
		horizontalF = c.x + c.r; //valor máximo X da nova imagem

	Pixel **newImage = calloc(size, sizeof(Pixel*)); //nova imagem

	//verifica se houve erro na alocação de memoria
	if (newImage == NULL){
		fprintf(stderr, "Falha de alocação\n");
		exit(0);
	}

	//termina de alocar a imagem
	for (i = verticalI; i < verticalF; i++){
		newImage[i-verticalI] = calloc(size, sizeof(Pixel));
	}

	//percorre a imagem recortada entre seus limites
	for (i = verticalI; i < verticalF; i++){
		for (j = horizontalI; j < horizontalF; j++){

			//copia os pixels da imagem maior para a menor
			newImage[i-verticalI][j-horizontalI].r = image[i][j].r;
			newImage[i-verticalI][j-horizontalI].g = image[i][j].g;
			newImage[i-verticalI][j-horizontalI].b = image[i][j].b;
		}
	}

	//altera os valores de tamanho da imagem
	*width = size;
	*height = size;

	return newImage;

}

//Retorna uma imagem binária a partir de um threshold dado
int** getBinImage(Pixel **image, int width, int height, int threshold){
	int i, j, //contadores
		**binImage = calloc (height, sizeof(int)); //imagem binaria
	
	//verifica se houve erro na alocação da imagem
	if (binImage == NULL){
		fprintf(stderr, "Falha de alocação\n");
		exit(0);
	}

	//percorre a imagem binaria
	for (i = 0; i < height; i++){
		//aloca as linhas
		binImage[i] = calloc(width,sizeof(int));

		for (j = 0; j < width; j++){
			
			//caso o threshold seja positivo, os pixels acima desse serão 1 e os demais 0
			//o comportamento oposto ocorre quanto o threshold é negativo
			if (threshold >= 0){
				if (image[i][j].r >= threshold)
					binImage[i][j] = true;
				else
					binImage[i][j] = false;
			} else {
				if (image[i][j].r >= (threshold)*-1)
					binImage[i][j] = false;
				else
					binImage[i][j] = true;
			}
			
		}
	}

	return binImage;
}

//Retorna a porcentagem com base nos valores total e atual, dados
double getPercentual (int vTotal, int vCurrent){
	double out = (double) (vCurrent * 100)/vTotal;
	return out;
}

//Retorna um círculo com base em raios máximo e mínimo, dados, em uma imagem
Circle findCircle(int **image, int width, int height, int rmin, int rmax){
	double cosValues[360], //valores de cosseno
		   sinValues[360]; //valores de seno
	int x, y, r, t, a, b, //contadores
		rmag = rmax - rmin; //tamanho da terceira dimensão da matriz A
		
	//calcula previamente valores de seno e cosseno em [0;2pi)
	for (t = 0; t < 360; t++){
		cosValues[t] = cos(t*M_PI/180);
		sinValues[t] = sin(t*M_PI/180);
	}

	//alocan matriz height X width X rmag
	int ***A = calloc(height, sizeof(int ***));

	//verifica se houve erro na alocação
	if (A == NULL){
		fprintf(stderr, "Falha de alocação\n");
		exit(0);
	}

	//aloca as demais dimensões da matriz
	for (x = 0; x < height; x++){
		A[x] = calloc(width, sizeof(int*));
		for (y = 0; y < width; y++){
			A[x][y] = calloc(rmag, sizeof(int));
		}
	}

	int aux = 0, //variavel auxiliar
	    total = width * height; //total de pixels
	double per; //percentual

	//percorre a imagem
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			//mostra o progresso do algoritmo
			aux++;
			per = getPercentual(total, aux);
			printf("\r%.2lf %%  ", per);
			fflush(stdout);

			//caso encontre um pixel de aresta
			if (image[x][y] == true){

				//para cada possível raio
				for (r = 0; r < rmag; r++){

					//para cada ângulo entre [0;2pi)
					for (t = 0; t < 360; t++){
						//calcula o centro do circulo 
						a = (int) (x - (r+rmin)*cosValues[t]);
						b = (int) (y - (r+rmin)*sinValues[t]);

						//verifica se o centro encontrado forma círculos válidos dentro da imagem						
						if ((a-(r+rmin)) >= 0 && (b-(r+rmin)) >= 0 && 
							(a+(r+rmin)) < height && (b+(r+rmin)) < width){ 

							A[a][b][r] +=1;

						}
						
					}
				}
			}

		}
	}

	aux = 0;
	Circle c = {0, 0, 0};
	a = 0; //maior valor de soma de interseções de centros de círculo

	//para todos os raios possíveis
	for (r = 0; r < rmag; r++){	

		//percorre a imagem
		for (x = 0; x < height; x++){
			for (y = 0; y < width; y++){
				//se uma soma na posição A[x][y][r] maior que a anterior foi encontrada,
				//atualiza o maior valor
				if (A[x][y][r] > a){
					a = A[x][y][r];
					c.r = r + rmin;	
					c.x = x;
					c.y = y;
				}
			}
		}

	}

	return c;
}

//Desenha um círculo passado como parâmetro na imagem dada
void drawCircle (Pixel **image, int width, int height, Circle c, int margin, float opacity){
	int x, y, t, r; //contadores

	//percorre a imagem
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			//se o pixel estiver fora da região do circulo, será multiplicado por um fator
			//escurecedor
			if (x < (c.x - c.r) || x > (c.x + c.r) ||
				y < (c.y - c.r) || y > (c.y + c.r)){
				image[x][y].r *= opacity;
				image[x][y].g *= opacity;
				image[x][y].b *= opacity;
			} else if (sqrt(pow((x - c.x), 2) + pow((y - c.y), 2)) > c.r) {
				image[x][y].r *= opacity;
				image[x][y].g *= opacity;
				image[x][y].b *= opacity;
			}
		}
	}


	//desenha linha vermelha 'margin' vezes
	for (r = 0; r < margin; r++){
		for (t = 0; t < 360; t++){
			//obtem a coordenada de um ponto na circunferência do circulo
			x = (int) c.x + c.r*cos(t*M_PI/180);
			y = (int) c.y + c.r*sin(t*M_PI/180);

			//atribui a cor vermelha à borda do círculo
			image[x][y].r = 255;
			image[x][y].g = 0;
			image[x][y].b = 0;
		}
		c.r++;
	}

	return;
}


double cataractDiagnosis (Pixel **image, Circle pupil, Circle flash){
	int i, j, //contadores
		cTotal = 0, //numero de pixels com catarata
		nTotal = 0; //numero de pixels dentro do circulo

	for (i = (pupil.x - pupil.r); i <= (pupil.x + pupil.r); i++){
		for (j = (pupil.y - pupil.r); j <= (pupil.y + pupil.r); j++){
			//se o pixel estiver dentro da pupila e fora do flash
			if (sqrt(pow((i - pupil.x), 2) + pow((j - pupil.y), 2)) <= pupil.r &&
				sqrt(pow((i - flash.x), 2) + pow((j - flash.y), 2)) > flash.r){
				
				//se for um pixel de catarata
				if (image[i][j].r > PIXEL_CATARACT_MIN_THRESHOLD &&
					image[i][j].r < PIXEL_CATARACT_MAX_THRESHOLD)
					cTotal++;
				nTotal++;
			}
		}
	}

	//retorna o percentual de comprometimento da pupila
	return (double) cTotal/nTotal*100;
}

//Estima um possível centro do olho com base na coordenada do flash
Circle estimateCenter(int **image, int width, int height){
	int x, y, max, i, j, aux; //variáveis de contagem e auxiliares
		
	Circle c = {0, 0, 0}; //circulo genérico

	max = 0;

	//as possiveis regioes de flash sao calculadas pela maior soma de pixels de flash
	//numa matrix 32X32

	//percorre a imagem respeitando os limites
	for (y = 0 + FLASH_AREA; y < height - FLASH_AREA; y++){
		for (x = 0 + FLASH_AREA; x < width - FLASH_AREA; x++){
			
			aux = 0;

			//se o pixel for de flash, soma na variavel auxiliar
			if (image[y][x] == 1){
				for (i = -FLASH_AREA; i <= FLASH_AREA; i++){
					for (j = -FLASH_AREA; j <= FLASH_AREA; j++){
						aux += image[y + i][x + j];
						image[y + i][x + j] = 0;
					}
				}
			}

			//o pixel que tiver a maior área, provavelmente será um flash
			if (max < aux){
				max = aux;
				c.x = x;
				c.y = y;
			}
		}
	}

	return c;
}

//Encontra o círculo que contém o círculo dado
Circle fastFindCircle(int **image, int width, int height, Circle c){
	int a = 0, aux = 0, v = 0, //contadores e auxiliares
		iPosX, fPosX, iPosY, fPosY; //posições inicial e final, horizontalmente e inicial e final, verticalmente
	Circle center = {0,0,0}; //círculo genérico

	//encontra a primeira aresta, a partir da coordenada do flash, nas quatro direções
	//(para esquerda, para direita, para baixo, para cima, respectivamente)
	iPosX = findEdge (image, c.x, c.y, v, -1, 0);
	fPosX = findEdge (image, c.x, c.y, v, 1, 0);
	iPosY = findEdge (image, c.x, c.y, v, 0, -1);
	fPosY = findEdge (image, c.x, c.y, v, 0, 1);

	//calcula o centro do círculo com base nos pontos médios
	center.x = (iPosX + fPosX)/2;
	center.y = (iPosY + fPosY)/2;

	//calcula o maior raio (caso seja uma elipse)
	center.r = fmax(distanceOfPoints(iPosX, c.y, center.x, center.y), 
			   distanceOfPoints(c.x, iPosY, center.x, center.y));

	return center;
}

//Preenche uma região de uma imagem binária
void fill (int **image, int origX, int origY, int v1, int v2){
	//onde tiver v1, ele preenche com v2
	if (image[origY][origX] == v1)
		image[origY][origX] = v2;

	//verifica se o pixel nas quatro direções podem ser preenchidos, caso sim,
	//os preenche de forma recursiva

	if (image[origY][origX+1] == v1)
		fill (image, origX+1, origY, v1, v2);

	if (image[origY][origX-1] == v1)
		fill (image, origX-1, origY, v1, v2);

	if (image[origY+1][origX] == v1)
		fill (image, origX, origY+1, v1, v2);

	if (image[origY-1][origX] == v1)
		fill (image, origX, origY-1, v1, v2);

}

//Retorna a posição do primeiro pixel encontrado numa imagem binária conforme o valor passado como parâmetro
int findEdge (int **image, int origX, int origY, int value, int horizontal, int vertical){
	int aux = 0, range = 6; //auxiliares

	//procura por um pixel de aresta de forma horizontal e retorna sua posição
	if (horizontal != 0){
		while ((image[origY][origX + horizontal*aux] != value) && (aux < origX)){
			aux++;
		}

		return (origX + horizontal*aux);
	}

	//procura por um pixel de aresta de forma vertical e retorna sua posição
	if (vertical != 0){
		while ((image[origY + vertical*aux][origX] != value) && (aux < origY)){
			aux++;
		}

		return (origY + vertical*aux);
	}

	return 0;
}

//Calcula a distância entre dois pontos
int distanceOfPoints(int x1, int y1, int x2, int y2){
	int d = (int) sqrt(pow((x1-x2),2) + pow((y1-y2),2));
	return d;
}

//Retorna o maior valor de pixel da imagem dada
int getMaxPixelIntensity(Pixel **image, int width, int height){
	int i, j, d = 0; //contadores e auxiliares

	//percorre a imagem
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){

			//se encontrar um valor maior que o anterior, recebê-lo
			if (image[i][j].r > d){
				d = image[i][j].r;
			}

			//se encontrar um valor 255, retorna-o
			if (d == 255){
				return d; 
			}
		}
	}
	return d;
}

//Conta a quantidade de pixels numa imagem binária, que possuem um valor
int countPixels(int **image, int width, int height, int v){
	int i, j, d = 0;
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			if (image[i][j] == v)
				d++;
		}
	}
	return d;
}

//Retorna uma imagem binária com valores nulos fora da região do círculo dado
int** excludeOutsideCircle(int **image, int width, int height, Circle c){
	int x, y, //contadores
		**outImage = calloc(height, sizeof(int*)); //imagem binária

	//verifica se houve erro na alocação da imagem
	if (outImage == NULL){
		fprintf(stderr, "Falha de alocação\n");
		exit(0);
	}

	//percorre a imagem
	for (y = 0; y < height; y++){
		outImage[y] = calloc(width, sizeof(int));
		for (x = 0; x < width; x++){

			//se o pixel estiver dentro do círculo, copia ele para a nova imagem binária
			if ( (x > c.x-c.r) && (x < c.x+c.r) && (y > c.y-c.r) && (y < c.y+c.r)) 
				outImage[y][x] = image[y][x];

		}
	}
	return outImage;
}