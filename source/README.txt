AUTORES
 - Alison Hedigliranes da Silva
 - Felipe Morais da Silva



SOBRE O PROGRAMA

O programa foi testado nos sistemas:
 - Windows (gcc 6.3.0)
 - Ubuntu  (gcc 4.8.4 e 5.4.0)

Para executar o programa, basta navegar, pelo terminal, até a pasta do projeto e digitar os seguintes comandos:

Para compilar o código e obter o executável "catarata":
 $ make
Para recompilar o código (caso necessário):
 $ make clean; make
Para executar o programa (a ordem dos parâmetros não ocasiona em erros)
 $ ./catarata -i <caminho/imagem.formato> -f <formato> -o <caminho/diagnostico.txt>



O QUE FOI FEITO (FUNCIONALIDADES)

Todas as funcionalidades foram feitas por ambos integrantes da dupla (Alison Hedigliranes e Felipe Morais)

[Legenda: * Funcionalidades extras (ou possíveis extras)]

Modularização do código:
 - main    (contém as chamadas dos métodos necessários para a detecção da catarata)
 - ioput   (contém todos os métodos que envolvem input e output)
 - filters (contém todos os métodos relativos aos filtros, segmentação e diagnóstico)

Leituras de imagens:
 - PGM P2* e P5*
 - PPM P3 e P6*
 - BMP* (24 bits e sem compressão)

Escrita de imagens (PPM P3)

Aplicação de filtros na imagem
 - Escala cinza
 - Gaussiano
 - Sobel
 - Binarização por threshold fixo

Otimização da transformada de Hough*
 - Estima a região da íris e procura a pupila somente nessa
 - Utiliza algumas regras de proporção para encontrar os raios mínimo e máximo com base no raio aproximado da íris

Segmentação da pupila

Segmentação do flash*

Tratamento de erros

Desenho do contorno da pupila*

Detecção da catarata



COMO O ALGORITMO DETECTA CATARATA (RESUMIDAMENTE)

A ordem de execução dos passos para detectar a catarata foi um pouco diferente da esperada, que, em resumo, seria:
 - Ler a imagem .ppm
 - Aplicar uma série de filtros
 - Detectar a pupila através de Hough
 - Gerar o diagnóstico

Nós, porém, fizemos da seguinte forma:
 - Ler a imagem .ppm
 - Encontrar região da íris
    1 - Aplica-se o filtro de escala cinza à imagem
	2 - Cria-se duas imagens binárias (a primeira, com um threshold igual ao valor de intensidade máxima de pixel na imagem, e a segunda com um threshold capaz de isolar as regiões mais escuras da imagem)
	3 - Encontra-se o flash na imagem usando a primeira imagem binária
	4 - Usa-se a coordenada do flash para estimar o centro e raio da íris (levando em consideração a existência de um flash dentro da íris) com base na segunda imagem binária 
	5 - Recorta-se, da imagem original, a região da íris
 - Aplicar os demais filtros (conforme a abordagem padrão)
 - Detectar a pupila através de Hough
 - Gerar o diagnóstico

A vantagem desse processo é poder automatizar a escolha dos raios com base na proporção do raio da íris 
com o raio da pupila, mesmo que esses não sejam concêntricos. Além disso, a transformada de Hough encontra
círculos de forma mais rápida já que busca círculos em uma imagem menor com um intervalo de raios menor.



O QUE SERIA IMPLEMENTADO DE FORMA DIFERENTE

A abordagem que realizamos possui alguns problemas por não utilizar thresholds automáticos, então, quando o algoritmo tenta preencher a região do flash, é possível que a área da íris (onde o flash está contido) possua aberturas, o que ocasionaria no preenchimento do restante da imagem. De qualquer forma, a abordagem funciona bem para as imagens dadas, porém, não garantimos que funcionará para qualquer outra devido isso.

Além disso, há outras formas mais sofisticadas de detectar a íris conhecendo-se as proporções do olho, de forma geral, e certamente essa seria outra funcionalidade que queríamos ter feito.