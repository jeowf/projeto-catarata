#include "filters.h"
#include <math.h>

//descartar
Pixel** squareImage(Pixel **image, int *width, int *height, int d){
	
	//criar região quadrada no centro da imagem
	*height = nearDivisible(*height, d);
	*width  = nearDivisible(*width, d);

	int i, j,
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

	return newImage;

}

//deu errado
Pixel** resizeImage(Pixel **image, int *width, int *height, int d){
	int i, j, k, l;

	Pixel **newImage = calloc(*height/d, sizeof(Pixel*)),
		  p;
	for (i = 0; i < *height/d; i++)
		newImage[i] = calloc(*width/d, sizeof(Pixel));

	for (i = 0; i < *height; i+=d){
		for (j = 0; j < *width; j+=d){
			p.r += image[i][j].r;
			p.g += image[i][j].g;
			p.b += image[i][j].b;

			newImage[i/d][j/d].r = p.r;
			newImage[i/d][j/d].g = p.g;
			newImage[i/d][j/d].b = p.b;
			/*
			for (k = 0; k < d; k++){
				for (l = 0; l<d; l++){
					//printf("i:%d j:%d   k:%d l:%d  x:%d y:%d\n", i, j, k, l, (i/d), (j/d));
					p.r += image[i+k][j+l].r;
					p.g += image[i+k][j+l].g;
					p.b += image[i+k][j+l].b;


				}
			}
			
			//printf("\n>>x:%d y:%d\n", (i/d), (j/d));
			newImage[i/d][j/d].r = (p.r)/(d*d);
			newImage[i/d][j/d].g = (p.g)/(d*d);
			newImage[i/d][j/d].b = (p.b)/(d*d);
			//printf("--x:%d y:%d\n\n", (i/d), (j/d));*/
		}
	}
	*width /= d;
	*height /= d;
	return newImage;
}

//provavelmente desnecessario
int nearDivisible(int value, int d){
	int a = value;
	while ((a % d) > 0){
		a -=1;
	}
	return a;
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

		/*maskx[3][3] = {{-2, 0, 2},
					  {-3, 0, 3},
					  {-2, 0, 2}},
		masky[3][3] = {{2, 3, 2},
					  {0, 0, 0},
					  {-2, -3, -2}};*/
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

int** getBinImage(Pixel **image, int width, int height, int threshold){
	int i, j,
		**binImage = calloc (height, sizeof(int));

	for (i = 0; i < height; i++){
		binImage[i] = calloc(width,sizeof(int));
		for (j = 0; j < width; j++){
			if (image[i][j].r >= threshold)
				binImage[i][j] = true;
			else
				binImage[i][j] = false;
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

double getPercentual (int vTotal, int vCurrent){
	double out = (double) (vCurrent * 100)/vTotal;
	return out;
}


//testar essa abordagem caso nao de em nada
//http://laid.delanover.com/hough-transform-circle-detection-and-space-reduction/
Circle findCircle(int **image, int width, int height){
	double pi = 3.14,
		   cosValues[360],
		   sinValues[360];
	int x, y, r, t, a, b,
		rmin = 80,
		rmax = 220,//120,	

		//rmin = 70,
		//rmax = 120,//fmin(width, height)/2,
		rmag = rmax - rmin;
		
	//calcula previamente valores de seno e cosseno em [0;2pi)
	for (t = 0; t < 360; t++){
		cosValues[t] = cos(t*pi/180);
		sinValues[t] = sin(t*pi/180);
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

						
						if ((a-(r+rmin)) >= 0 && (b-(r+rmin)) >= 0 && 	//se o centro forma círculos dentro da imagem
							(a+(r+rmin)) < height && (b+(r+rmin)) < width){ /*&&
							a >= 0 && b >= 0 && 	//se o centro está dentro da imagem
							a < height && b < width){*/
							
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
		printf("\nR:%d (%d) at x:%d y:%d", c.r, A[c.x][c.y][r], c.x, c.y);
		//printf("\n{x:%d y:%d, R:%d}", c.x, c.y, c.r);

	}

	//DEBUG
	//
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			a = 0;
			for (r = 0; r < rmag; r++){
				a += A[x][y][r];	
			}
			image[x][y] = a;
		}
	}

	//printf("\n%d", A[350][568][80]  );
	//printf("\nx:%d y:%d r:%d (%d)\n", c.x, c.y, c.r, aux);

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
			x = (int) c.x + c.r*cos(t*pi/180);
			y = (int) c.y + c.r*sin(t*pi/180);

			image[x][y].r = 255;
			image[x][y].g = 0;
			image[x][y].b = 0;
		}
		c.r++;
	}

	return;
}

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