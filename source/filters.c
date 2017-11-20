#include "filters.h"
#include <math.h>

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

/*
void findCircle(int **image, int width, int height){
	double pi = 3.14;
	int x, y, r, t, a, b,
		rmin = 20,
		rmax = 200,
		***A = calloc (height, sizeof(int**));

	//iniciando matrix 3d inteira
	for (i = 0; i < height; i++){
		A[i] = calloc(height, sizeof(int*));
		for (j = 0; j < width; j++){
			A[i][j] = calloc(rmax-rmin, sizeof(int));
		}
	}
	

	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){

			if (image[x][y] == true){
				for (r = 0; r < rmax - rmin; r++){
					for (t = 0; t <= 360; t++){
						a = x - r*cos(t*pi/180);
						b = y - r*sin(t*pi/180);
						A[a][b][r] += 1;
					}
				}
			}

		}
	}

	//encontrar posicao do centro de circulo onde há maior raio
	int aux[1][1][1] = {0};
	for (x = 0; x < height; x++){
		for (y = 0; y < width; y++){
			for (r = 0; r < rmax-rmin; r++){
				if (A[x][y][r] > aux[0][0][0])
					aux[0][0][0] = A[x][y][r];

			}
		}
	}

	printf("x: %d y: %d = %d", );



	return ;
}*/