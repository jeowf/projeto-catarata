//#include "ioput.h"
#include "filters.h"
#include <stdio.h>

/*
Para otimizar a detecção da pupila (tanto deixar mais rápida quanto diminuir as chances de erro),
optamos por adicionar alguns processos extras além dos exigidos no documento do projeto.

O exigido, em resumo, seria:
	- Ler a imagem .ppm
	- Aplicar uma série de filtros
	- Detectar a pupila através de Hough
	- Gerar o diagnóstico

Nós, porém, tivemos alguns problemas para detectar o círculo usando as mesmas proporções de 
raio mínimo e máximo nas imagens fornecidas, sendo necessário conhecer os raios para cada imagem.
Como achamos que essa abordagem não era a melhor, propomos o seguinte:
	- Ler a imagem .ppm
	- Encontrar região da íris
		1 - Aplica-se o filtro de escala cinza à imagem
		2 - Cria-se duas imagens binárias (a primeira, com um threshold igual ao valor de intensidade 
		máxima de pixel na imagem, e a segunda com um threshold capaz de isolar as regiões mais escuras
		da image)
		3 - Encontra-se o flash na imagem usando a primeira imagem binária
		4 - Usa-se a coordenada do flash para estimar o centro e raio da íris (levando em consideração a 
		existência de um flash dentro da íris) com base na segunda imagem binária 
		5 - Recorta-se, da imagem original, a região da íris
	- Aplicar os demais filtros (conforme a abordagem padrão)
	- Detectar a pupila através de Hough
	- Gerar o diagnóstico

A vantagem desse processo é poder automatizar a escolha dos raios com base na proporção do raio da íris 
com o raio da pupila, mesmo que esses não sejam concêntricos. Além disso, a transformada de Hough encontra
círculos de forma mais rápida já que busca círculos em uma imagem menor com um intervalo de raios menor.

Mesmo assim, percebeu-se que essa abordagem possui alguns problemas que a impede de ser funcional para qualquer
imagem de olho:
	- Caso a imagem do olho possua muita incidência de luz em outras regiões fora da íris, é possível que
	a estimativa do flash seja errada (da forma como está implementado o código)
	- Por não existir um threholding automático para a binarização da região da íris, é possível que essa
	região não seja 100% fechada, comprometendo o preenchimento dessa.

No mais, o algoritmo funciona muito bem para as imagens dadas e consegue dar o diagnóstico esperado para
cada qual.
*/
int main(int argc, char *argv[]){
	//pega informações da imagem 
	ObjectImage *objectImage = newObjectImage(argc, argv);
	
	//lê a imagem
	Pixel **image = readImage(objectImage); // <------------COMENTE EM CASO DE ERRO

	/*
	ATENÇÃO: 
	Dependendo do sistema operaional ou versão do GCC, pode ocorrer alguns problemas na leitura da imagem que
	irão ocasionar na falha da alocação da imagem.
	Nesse caso, comente a linha de código acima e descomente a de baixo.
	*/
	//Pixel **image = readImageLegacy(objectImage); //<--------- DESCOMENTE EM CASO DE ERRO

	//converte imagem para escala cinza
	applyGrayScale(image, objectImage->width, objectImage->height);

	//faz copia da imagem para ser usada posteriormente
	Pixel **imageOrig = copyImage(image, objectImage->width, objectImage->height);
	
	//aplica gaussiano na imagem a ser tratada
	applyGaussian(image, objectImage->width, objectImage->height);


	//------- INICIO: DETECÇÃO DA REGIÃO QUE CONTÉM A ÍRIS -------

	//calula a intensidade máxima de pixel
	int maxIntensity = getMaxPixelIntensity(image, objectImage->width, objectImage->height);

	//binariza a imagem com um threshold alto para captar áreas de alta luminosidade, ou seja, possíveis flashs
	int **binSpecImage = getBinImage(image, objectImage->width, objectImage->height, maxIntensity);

	//binariza a imagem passando um threshold (negatico para inverter os valores) suficiente para pegar regiões de íris
	int **binAreaImage = getBinImage(image, objectImage->width, objectImage->height, -70);

	//estima o centro da iris com base n a posição do flash, já que esse, necessariamente, está contido na iris
	Circle estimatedCenter = estimateCenter(binSpecImage, objectImage->width, objectImage->height);

	//Caso o olho possua catarata, a região da íris possuirá um 'buraco' que irá atrapalhar no 
	//cálculo do centro da iris, então é necessário preencher esse 'buraco'
	fill (binAreaImage, estimatedCenter.x, estimatedCenter.y, 0, 1);

	//calcula o centro da iris
	Circle irisCenter = fastFindCircle(binAreaImage, objectImage->width, objectImage->height, estimatedCenter);

	//recorta da imagem original a regiao em que a iris está contida
	Pixel **irisRegion = cropImage(imageOrig, &objectImage->width, &objectImage->height, irisCenter);

	//------- FIM: DETECÇÃO DA REGIÃO QUE CONTÉM A ÍRIS -------


	//faz uma cópia da região da iris para futuros calculos
	Pixel **imageFinal = copyImage(irisRegion, objectImage->width, objectImage->height);

	//aplica o filtro sobel na região da iris para iniciar a detecção da pupila
	applySobel(irisRegion, objectImage->width, objectImage->height);

	//cria uma versão binária da imagem (região da íris) com sobel
	int **edges = getBinImage(irisRegion, objectImage->width, objectImage->height, 10);

	//cria uma versão binária da imagem (região da íris) com sobel, porém com um threshold maior, para
	//obter as arestas dos círculos que irão compor o flash
	int **flashRegion = getBinImage(irisRegion, objectImage->width, objectImage->height, 60);
	//printf("%d\n", flashRegion[98][1]);

	//escreve a imagem enquanto essa será processada (apenas um debug útil)
	writeImage(objectImage, imageFinal);

	//calcula quantidade de pixels brancos e pretos para estabelecer uma relação com o raio máximo dos círculos
	//internos à iris
	int whitePixels = countPixels(edges, objectImage->width, objectImage->height, 1);
	int blackPixels = objectImage->width*objectImage->height - whitePixels;

	//calcula as contantes multiplicativas dos raios máximo e mínimo da pupila com abse no raio da íris
	double kRmax = 1 - fmax(whitePixels - blackPixels, 0)/(objectImage->width*objectImage->height),
		   kRmin = kRmax * 0.5;

	//procura a pupila
	Circle pupil = findCircle(edges, objectImage->width, objectImage->height, kRmin *  irisCenter.r, kRmax * irisCenter.r);

	//exclui os pixels fora da pupila para evitar erros na detecção do flash
	flashRegion = excludeOutsideCircle(flashRegion, objectImage->width, objectImage->height, pupil);

	//procura o flash
	Circle flash = findCircle(flashRegion, objectImage->width, objectImage->height, kRmin * 0.16 * irisCenter.r, kRmin * 0.5 * irisCenter.r);

	//escreve o diagnóstico
	writeDiagnosis(*objectImage, cataractDiagnosis(imageFinal, pupil, flash), DIAGNOSIS_CATARACT_THRESHOLD);

	//desenha os círculos em volta da pupila e do flash
	drawCircle(imageFinal, objectImage->width, objectImage->height, pupil, 1, 0.1);
	drawCircle(imageFinal, objectImage->width, objectImage->height, flash, 1, 1);

	//escreve a imagem
	writeImage(objectImage, imageFinal);

	return 0;
}
