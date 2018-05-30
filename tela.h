#ifndef _tela_h_
#define _tela_h_

//
// tela.h
// ------
//
// funções de acesso à tela
//
// para uso na disciplina Laboratório de Programação I - 2o sem. 2017 - UFSM
//


#include <stdbool.h>

// inicialização da tela
// cria uma janela com o tamanho dado em pixels
// deve ser executada antes do uso de qualquer outra função da tela
void tela_inicio(int largura, int altura, char nome[]);


// finalização da tela
// deve ser chamada no final da utilização da tela, nenhuma outra função da
// tela deve ser chamada depois desta.
void tela_fim(void);


// inicia o desenho de uma imagem
// prepara o necessário para que se possa desenhar uma nova imagem.
// as funções de desenho só devem ser chamadas após esta função.
// o que for desenhado será mantido em memória, e só será mostrado na tela
// quando for chamada a função tela_termina_desenho().
void tela_inicia_desenho(void);


// atualiza a tela
// faz com o que foi desenhado na tela desde a última chamada à 
// função tela_inicia_desenho() realmente apareça.
// caso necessário, espera até que a imagem anterior tenha ficado
// na tela pelo tempo adequado.
// antes da chamada a esta função a imagem anterior fica sendo exibida, 
// o conteúdo da nova imagem fica só na memória.
// após esta função ser chamada, as funções de desenho não devem ser usadas
// enquanto a função tela_inicia_desenho() não for chamada novamente.
void tela_termina_desenho(void);


// DESENHO

// desenha um círculo
// x e y são as coordenadas do centro do círculo, r é o raio
// l é a largura da linha, corl a cor da linha e corint é a cor
// do interior do círculo
void tela_circulo(float x, float y, float r, float l, int corl, int corint);

// desenha uma reta
// x1, y1 e x2, y2 são as coordenadas das pontas, l a largura e corl a cor
void tela_linha(float x1, float y1, float x2, float y2, float l, int corl);

// desenha um retangulo
// x1, y1 são as coordenadas do canto superior esquerdo, 
// x2, y2 as do inferior direito
// l é a largura da linha, corl a cor da linha e corint é a cor
// do interior do retangulo
void tela_retangulo(float x1, float y1, float x2, float y2, float l,
                    int corl, int corint);


// desenha texto centrado
// x,y coordenadas do meio do texto, tam tamanho das letras, c cor, t texto
void tela_texto(float x, float y, int tam, int c, char t[]);

// desenha texto à esquerda
// x,y coordenadas do fim do texto, tam tamanho das letras, c cor, t texto
void tela_texto_esq(float x, float y, int tam, int c, char t[]);

// desenha texto à direita
// x, y coordenadas do inicio do texto, tam tamanho das letras, c cor, t texto
void tela_texto_dir(float x, float y, int tam, int c, char t[]);



// CORES

// valores para representar cores pré-definidas
#define transparente 0
#define azul 1
#define vermelho 2
#define verde 3
#define amarelo 4
#define preto 5
#define laranja 6
#define rosa 7
#define branco 8
#define marrom 9

// cria uma nova cor
// vm, az, vd componentes vermelho, azul e verde (entre 0 e 1) da nova cor
// retorna um valor inteiro que deve ser usado nas funções de desenho
int tela_cria_cor(float vm, float az, float vd);



// ENTRADA DE DADOS

// retorna a posição x do mouse
float tela_rato_x(void);

// retorna a posição y do mouse
float tela_rato_y(void);

// retorna se o botão do mouse está apertado
bool tela_rato_apertado(void);

// retorna se o botão do mouse foi clicado (apertado e solto) desde a última
// chamada a esta função
bool tela_rato_clicado(void);

// retorna a posição x do mouse na última vez que ele foi clicado
float tela_rato_x_clique(void);

// retorna a posição y do mouse na última vez que ele foi clicado
float tela_rato_y_clique(void);

// retorna uma tecla digitada
// pode retornar um caractere imprimível ou '\b' para backspace ou '\n' para
// enter ou '\0' para qualquer outra coisa ou se não tiver sido digitado nada.
char tela_le_tecla(void);

#endif
