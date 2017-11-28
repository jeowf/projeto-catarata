#include "filters.h"
#include <math.h>

Pixel** copyImage(Pixel **image, int width, int height){
	int i, j;
	Pixel **newImage = calloc(height,sizeof(Pixel));
	for (i = 0; i < height; i++){
		newImage[i] = calloc(width,sizeof(Pixel));
		for (j = 0; j< width; j++)
			newImage[i][j] = image[i][j];
	}
	return newImage;
}

void applyGrayScale(Pixel **image, int width, int height){
	int i, j;
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			int pixelValue = image[i][j].r * 0.30 +
							 image[i][j].g * 0.59 +
							 image[i][j].b * 0.11 ;	
			image[i][j].r = pixelValue;
			image[i][j].g = pixelValue;
			image[i][j].b = pixelValue;
		}
	}
}

void applyGaussian(Pixel **image, int width, int height){
	int x, y, a, b,
		tam = 5,
		m = tam/2,
		weight = 159,
		mask[5][5] = {{2, 4, 5, 4, 2},
					  {4, 9, 12, 9, 4},
					  {5, 12, 15, 12, 5},
					  {4, 9, 12, 9, 4},
					  {2, 4, 5, 4, 2}};

	Pixel **imageOrig = copyImage(image, width, height);
	for (x = 0 + 2; x < height - 2; x++){
		for (y = 0 + 2; y < width - 2; y++){
			
			for (a = -m; a <= m; a++){
				for (b = -m; b <= m; b++){
					
					image[x][y].r += mask[a+m][b+m]*imageOrig[x+a][y+b].r;
					image[x][y].g += mask[a+m][b+m]*imageOrig[x+a][y+b].g;
					image[x][y].b += mask[a+m][b+m]*imageOrig[x+a][y+b].b;

				}
			}

			image[x][y].r /= weight;
			image[x][y].g /= weight;
			image[x][y].b /= weight;

		}
	}

	free(imageOrig);

	return;
}

void applySobel(Pixel **image, int width, int height){
	int x, y, a, b,
		tam = 3,
		m = tam/2,
		maskx[3][3] = {{-1, 0, 1},
					  {-2, 0, 2},
					  {-1, 0, 1}},
		masky[3][3] = {{1, 2, 1},
					  {0, 0, 0},
					  {-1, -2, -1}};
	Pixel gx, gy, g;

	Pixel **imageOrig = copyImage(image, width, height);
	for (x = 0 + 1; x < height - 1; x++){
		for (y = 0 + 1; y < width - 1; y++){
			
			gx.r = 0; gx.b = 0; gx.g = 0;
			gy.r = 0; gy.b = 0; gy.g = 0;

			for (a = -m; a <= m; a++){
				for (b = -m; b <= m; b++){
					
					gx.r += maskx[a+m][b+m]*imageOrig[x+a][y+b].r;
					gx.g += maskx[a+m][b+m]*imageOrig[x+a][y+b].g;
					gx.b += maskx[a+m][b+m]*imageOrig[x+a][y+b].b;

					gy.r += masky[a+m][b+m]*imageOrig[x+a][y+b].r;
					gy.g += masky[a+m][b+m]*imageOrig[x+a][y+b].g;
					gy.b += masky[a+m][b+m]*imageOrig[x+a][y+b].b;
				}
			}


			g.r = sqrt(gx.r*gx.r + gy.r*gy.r);
			g.g = sqrt(gx.g*gx.g + gy.g*gy.g);
			g.b = sqrt(gx.b*gx.b + gy.b*gy.b);

			image[x][y] = g;
		}
	}

	free(imageOrig);

	return;
}

Pixel** cropImage(Pixel **image, int *width, int *height, Circle c){
	//c.r = 105; //para normal2
	int i, j,
		size = c.r * 2 ; 

	
	int verticalI 	= c.y - c.r,
		verticalF 	= c.y + c.r,
		horizontalI = c.x - c.r,
		horizontalF = c.x + c.r;

	Pixel **newImage = calloc(size, sizeof(Pixel*));
	for (i = verticalI; i < verticalF; i++){
		newImage[i-verticalI] = calloc(size, sizeof(Pixel));
	}

	for (i = verticalI; i < verticalF; i++){
		for (j = horizontalI; j < horizontalF; j++){
			//printf("i %d   j %d\n", (i-verticalI), (j-horizontalI) );
			newImage[i-verticalI][j-horizontalI].r = image[i][j].r;
			newImage[i-verticalI][j-horizontalI].g = image[i][j].g;
			newImage[i-verticalI][j-horizontalI].b = image[i][j].b;
		}
		//printf("%d\n",(i-verticalI));
	}


	
	*width = size;
	*height = size;
	//printf("%d %d\n", *width, *height);

	return newImage;

}

/*
Pixel** cropImage(Pixel **image, int *width, int *height){
	///*int i, j,
		h = 0, v = 0,
		margin = abs((*width - *height)/2);

	//calcula os limites da imagem com base na borda
	if (*width > *height){
		*width = *height + margin;
		h = margin;
	}
	else{
		*height = *width + margin;
		v = margin;
	}

	int min = fmin(*height, *width);
	Pixel **newImage = calloc(min, sizeof(Pixel*));
	for (i = v; i < *height; i++){
		newImage[i] = calloc(min, sizeof(Pixel));
		for (j = h; j < *width; j++){
			newImage[i-v][j-h].r = image[i][j].r;
			newImage[i-v][j-h].g = image[i][j].g;
			newImage[i-v][j-h].b = image[i][j].b;
		}
	}
	
	*width = min;
	*height = min;

	return newImage;//


	//AQUI Q TA FUNFANDO, EM CIMA EH SO BACKUP
	//criar região quadrada no centro da imagem
	int i, j, x = 350,
		h = 0, v = 0,
		margin = abs((*width - *height)/2);
	*width -= x;
	*height -= x;
	int horizontal = 250;
	int vertical = 160;

	//calcula os limites da imagem com base na borda
	if (*width > *height){
		*width = *height + margin;
		h = margin;
	} else{
		*height = *width + margin;
		v = margin;
	}

	int min = fmin(*height, *width);
	Pixel **newImage = calloc(min, sizeof(Pixel*));
	for (i = v + vertical; i < *height + vertical ; i++){
		newImage[i-v-vertical] = calloc(min, sizeof(Pixel));
		for (j = h + horizontal; j < *width + horizontal; j++){
			newImage[i-v-vertical][j-h-horizontal].r = image[i][j].r;
			newImage[i-v-vertical][j-h-horizontal].g = image[i][j].g;
			newImage[i-v-vertical][j-h-horizontal].b = image[i][j].b;
		}
	}
	*width = min;
	*height = min;

	return newImage;

}*/

/*
int** getBinImage(Pixel **image, int width, int height, int threshold){
	int i, j;
	int a[256] = {0};
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			a[image[i][j].r]++;
		}
	}
	printf("opacity\n");
	for (i = 0; i < 256; i++){
		printf("%d, %d\n", i, a[i]);
		
	}
	int **binImage = calloc (height, sizeof(int));
	for (i = 0; i < height; i++)
		binImage[i] = calloc(width, sizeof(int));
	/*
	int i, j, k, l,
		posX, posY, valueXY;
	
	//
	int **binImage = calloc (height, sizeof(int));
	for (i = 0; i < height; i++)
		binImage[i] = calloc(width, sizeof(int));

	//
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			if (i > 0 && i < height-1 && 
				j > 0 && j < width-1){

				valueXY = image[i][j].r;
				for (k = i-1; k <= i+1; k++){
					for (l = j-1; l <= j+1; l++){
						if (image[i][j].r <= valueXY){
							valueXY = image[i][j].r;
							posY = i+k;
							posX = j+l;
						}

					}
				}
				//printf("%d\n", valueXY);

				binImage[posY][posX] = 1;

			}
			printf("i: %d   j: %d\n", i, j);
		}

	}

	return binImage;
}
*/

void histogram(Pixel **image, int width, int height){
	int i, j;
	int a[256] = {0};
	//printf("asusanads\n");

	for (i = 0; i < height; i++){

		for (j = 0; j < width; j++){

			a[(int) fmin(image[i][j].r, 255)]++;
		}
	}
	//printf("asusanads\n");
	for (i = 0; i < 256; i++){
		printf("%d, %d\n", i, a[i]);
		
	}

	return;
	
}

int** getBinImage(Pixel **image, int width, int height, int threshold){
	int i, j,
		**binImage = calloc (height, sizeof(int));

	for (i = 0; i < height; i++){
		binImage[i] = calloc(width,sizeof(int));
		for (j = 0; j < width; j++){
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

//metodo de debug
Pixel** createBinImage(int **image, int width, int height){
	int i, j,
		w = 167, b = 8;
	Pixel **imageRes = calloc(height,sizeof(Pixel));
	for (i = 0; i < height; i++){
		imageRes[i] = calloc(width,sizeof(Pixel));
		for (j = 0; j< width; j++)
			if (image[i][j] == true){
				imageRes[i][j].r = w;
				imageRes[i][j].g = w;
				imageRes[i][j].b = w;
			} else {
				imageRes[i][j].r = b;
				imageRes[i][j].g = b;
				imageRes[i][j].b = b;
			}
	}
	return imageRes;
}

//metodo de debug
double getPercentual (int vTotal, int vCurrent){
	double out = (double) (vCurrent * 100)/vTotal;
	return out;
}

//testar essa abordagem caso nao de em nada
//http://laid.delanover.com/hough-transform-circle-detection-and-space-reduction/
Circle findCircle(int **image, int width, int height, int rmin, int rmax){
	double cosValues[360],
		   sinValues[360];
	int x, y, r, t, a, b,
		rmag = rmax - rmin;
		
	//calcula previamente valores de seno e cosseno em [0;2pi)
	for (t = 0; t < 360; t++){
		cosValues[t] = cos(t*M_PI/180);
		sinValues[t] = sin(t*M_PI/180);
	}

	//alocando matrix height X width X rmag
	int ***A = calloc(height, sizeof(int ***));
	for (x = 0; x < height; x++){
		A[x] = calloc(width, sizeof(int*));
		for (y = 0; y < width; y++){
			A[x][y] = calloc(rmag, sizeof(int));
		}
	}

	//variaveis debug 
	int aux = 0, total = width * height;
	double per;

	//para todos os pixels que compoem a imagem
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			
			//debug
			aux++;
			per = getPercentual(total, aux);
			printf("\r%.2lf %%", per);
			fflush(stdout);
			//fimdebug

			//para os pixels de arestas
			if (image[x][y] == true){
				for (r = 0; r < rmag; r++){
					for (t = 0; t < 360; t++){
						a = (int) (x - (r+rmin)*cosValues[t]);
						b = (int) (y - (r+rmin)*sinValues[t]);

						
						//verifica se o centro encontrado forma círculos válidos						
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
	/*
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){
			a = 0;

			for (r = 0; r < rmag; r++){
				a += A[x][y][r];	
			}

			if (a >= aux){
				aux = a;
				c.x = x;
				c.y = y;
			} 
		}
	}

	for (r = 0; r < rmag; r++){
		printf("R%d  V:%d\n", (r + rmin), A[c.x][c.y][r] );
		if (A[c.x][c.y][r] > c.r)
			c.r = r + rmin;	
	}
	*/
	a = 0;
	for (r = 0; r < rmag; r++){
		//printf("R%d  V:%d\n", (r + rmin), A[c.x][c.y][r] );
		
		
		for (x = 0; x < height; x++){
			for (y = 0; y < width; y++){
				if (A[x][y][r] > a){
					a = A[x][y][r];
					c.r = r + rmin;	
					c.x = x;
					c.y = y;
				}
			}
		}
		//printf("\nR:%d (%d) at x:%d y:%d", c.r, A[c.x][c.y][r], c.x, c.y);
		//printf("\n{x:%d y:%d, R:%d}", c.x, c.y, c.r);

	}

	//DEBUG
	//
	/*
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			a = 0;
			for (r = 0; r < rmag; r++){
				a += A[x][y][r];	
			}
			image[x][y] = a;
		}
	}*/

	//printf("\n%d", A[350][568][80]  );
	printf("\nx:%d y:%d r:%d (%d)\n", c.x, c.y, c.r, aux);

	return c;
}

void drawCircle (Pixel **image, int width, int height, Circle c, int margin){
	double pi = 3.14;
	int x, y, t, r; 
	
	float opacity = 0.2;

	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){
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


	//desenha linha vermelha
	for (r = 0; r < margin; r++){
		for (t = 0; t < 360; t++){
			x = (int) c.x + c.r*cos(t*M_PI/180);
			y = (int) c.y + c.r*sin(t*M_PI/180);

			image[x][y].r = 255;
			image[x][y].g = 0;
			image[x][y].b = 0;
		}
		c.r++;
	}

	return;
}

//DEBUG
Pixel** plotImage (int **image, int width, int height){
	int i, j, max = 0, a;
	Pixel **imageRes = calloc(height,sizeof(Pixel));
	for (i = 0; i < height; i++){
		imageRes[i] = calloc(width,sizeof(Pixel));
		for (j = 0; j< width; j++){
			if (image[i][j] > max)
				max = image[i][j];
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j< width; j++){
			a = (255*image[i][j])/max;
			imageRes[i][j].r = a;
			imageRes[i][j].g = a;
			imageRes[i][j].b = a;
		}
	}
	return imageRes;

}

double cataractDiagnosis (Pixel **image, Circle c){
	int i, j,
		//pTotal = 0, //total 
		cTotal = 0, //numero de pixels com catarata
		nTotal = 0; //numero de pixels dentro do circulo

	for (i = (c.x - c.r); i <= (c.x + c.r); i++){
		for (j = (c.y - c.r); j <= (c.y + c.r); j++){
			//se o pixel estiver dentro do circulo
			if (sqrt(pow((i - c.x), 2) + pow((j - c.y), 2)) <= c.r){
				if (image[i][j].r >= PIXEL_CATARACT_THRESHOLD)
					cTotal++;
				nTotal++;
			}
		}
	}
	return (double) cTotal/nTotal*100;
}

Circle estimateCenter(int **image, int width, int height){
	int x, y, max, i, j, aux,
		*horizontalProjection = calloc(width, sizeof(int)),
		*verticalProjection = calloc(height, sizeof(int));

	Circle c = {0, 0, R_CROP_MAX};
	if (fmin(width, height) < 3*R_CROP_MAX)
		c.r /= 2;

	max = 0;
	for (y = 0 + FLASH_AREA; y < height - FLASH_AREA; y++){
		for (x = 0 + FLASH_AREA; x < width - FLASH_AREA; x++){
			
			aux = 0;
			if (image[y][x] == 1){
				for (i = -FLASH_AREA; i <= FLASH_AREA; i++){
					for (j = -FLASH_AREA; j <= FLASH_AREA; j++){
						aux += image[y + i][x + j];
						image[y + i][x + j] = 0;
					}
				}
			}

			if (max < aux){
				max = aux;
				c.x = x;
				c.y = y;
			}
		}
	}

	return c;
}

//209
//82
//132
//206