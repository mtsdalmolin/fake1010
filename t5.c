#include "tela.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

const int LARGURA_TELA = 560;
const int ALTURA_TELA = 640;
FILE *arq_records;
char *nomeArq = "pontuacao.txt";
char letra_digitada = ' ';
double x, y;
int linha = 0;
int coluna = 0;
int movePeca = 0;
int geraNovasPecas = 3;
int pontuacao = 0;
int i, j, f, g;
int m[10][10];
int pontAnterior=0;
int MatrizCores[10][10];
int soundPlay=0;
int soundHTP=0;
int soundRank=0;
int soundQuit=0;
int play_sound_yes=0;
int play_sound_no=0;
int play_sound_gameover = 0;
int play_sound_menu = 0;
int play_sound_back = 0;
char texto[100], record[30];
bool play_sound_select_block = true;
bool validade = false, fim = false;
bool var_reinicia_jogo = false;
bool how_to_play = false;
bool ranking = false;
bool varbreak = false;
bool jogaJogo = false;
bool ver_linhas = false;

struct peca
{
	int p[5][5];
	int cor;
	float x, y;
	float lado_peca;
	bool peca_ativa;
};

void desenha_principal(float x_ini, float y_ini, float lado)
{
	int cor;
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (m[i][j] == 0) {
				cor = transparente;
			}
			else {
				cor = MatrizCores[i][j];
			}
			tela_retangulo(x_ini + i*lado, y_ini + j*lado, x_ini + (i + 1)*lado, y_ini + (j + 1)*lado, 2, branco, cor);
		}
	}
}

int valorRandom(int x)
{
	int valor;
	valor = 1 + rand() % x;
	return valor;
}

bool tela_rato_direito(void)
{
	ALLEGRO_MOUSE_STATE rato;
  	al_get_mouse_state(&rato);
  	return al_mouse_button_down(&rato, 2);
}

void somatorioPontuacao(int valorJogada);

void define_tipo_peca(int p[5][5])
{

	int peca, tipo;

	peca = valorRandom(60);

	switch (peca)
	{
	case 1 ... 10:
		p[0][0] = 1;
		break;
	case 11 ... 20:
		tipo = valorRandom(100);
		if (tipo <= 50)
		{
			p[0][0] = 1;
			p[1][0] = 1;
		}
		else {
			p[0][0] = 1;
			p[0][1] = 1;
		}
		break;
	case 21 ... 30:
		tipo = valorRandom(100);
		if (tipo <= 25)
		{
			p[0][0] = 8;
			p[0][1] = p[1][0] = p[1][1] = 1;
		}
		else if (tipo > 25 && tipo <= 50) {
			p[0][0] = p[1][0] = p[1][1] = 1;
		}
		else if (tipo > 50 && tipo <= 75) {
			p[0][0] = p[0][1] = p[1][1] = 1;
		}
		else if (tipo > 75) {
			p[0][0] = p[0][1] = p[1][0] = 1;
		}
		break;
	case 31 ... 40:
		tipo = valorRandom(90);
		if (tipo <= 30)
		{
			p[0][0] = p[0][1] = p[1][0] = p[1][1] = 1;
		}
		else if (tipo > 30 && tipo <= 60) {
			p[0][0] = 1;
			p[0][1] = 1;
			p[0][2] = 1;
			p[0][3] = 1;
		}
		else if (tipo > 60) {
			p[0][0] = 1;
			p[1][0] = 1;
			p[2][0] = 1;
			p[3][0] = 1;
		}
		break;
	case 41 ... 50:
		tipo = valorRandom(120);
		if (tipo <= 20)
		{
			p[0][0] = p[0][1] = p[0][2] = p[1][2] = p[2][2] = 1;
		}
		else if (tipo > 20 && tipo <= 40) {
			p[0][0] = p[0][1] = p[0][2] = p[1][0] = p[2][0] = 1;
		}
		else if (tipo > 40 && tipo <= 60) {
			p[0][0] = p[1][0] = p[2][0] = p[2][1] = p[2][2] = 1;
		}
		else if (tipo > 60 && tipo <= 80) {
			p[0][0] = 9;
			p[0][2] = p[1][2] = p[2][0] = p[2][1] = p[2][2] = 1;
		}
		else if (tipo > 80 && tipo <= 100) {
			p[0][0] = 1;
			p[0][1] = 1;
			p[0][2] = 1;
			p[0][3] = 1;
			p[0][4] = 1;
		}
		else if (tipo > 100) {
			p[0][0] = 1;
			p[1][0] = 1;
			p[2][0] = 1;
			p[3][0] = 1;
			p[4][0] = 1;
		}
		break;
	case 51 ... 60:
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				p[i][j] = 1;
			}
		}
		break;
	}
}

void define_cor_peca(struct peca *ptrpeca)
{
	int defCor;

	defCor = valorRandom(70);

	switch (defCor) {
	case 1 ... 10:
		ptrpeca->cor = azul;
		break;
	case 11 ... 20:
		ptrpeca->cor = vermelho;
		break;
	case 21 ... 30:
		ptrpeca->cor = verde;
		break;
	case 31 ... 40:
		ptrpeca->cor = amarelo;
		break;
	case 41 ... 50:
		ptrpeca->cor = laranja;
		break;
	case 51 ... 60:
		ptrpeca->cor = rosa;
		break;
	case 61 ... 70:
		ptrpeca->cor = marrom;
		break;
	}
}

void setaLinha(double x, double y)
{
	if ((y >= ALTURA_TELA*0.2) && (y <= (ALTURA_TELA*0.2 + 10 * LARGURA_TELA*0.6 / 10))) {
		if ((x >= LARGURA_TELA*0.2) && (x <= (LARGURA_TELA*0.2 + LARGURA_TELA*0.6 / 10))) {
			coluna = 0;
		}
		else if ((x > (LARGURA_TELA*0.2 + LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 2 * LARGURA_TELA*0.6 / 10))) {
			coluna = 1;
		}
		else if ((x > (LARGURA_TELA*0.2 + 2 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 3 * LARGURA_TELA*0.6 / 10))) {
			coluna = 2;
		}
		else if ((x > (LARGURA_TELA*0.2 + 3 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 4 * LARGURA_TELA*0.6 / 10))) {
			coluna = 3;
		}
		else if ((x > (LARGURA_TELA*0.2 + 4 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 5 * LARGURA_TELA*0.6 / 10))) {
			coluna = 4;
		}
		else if ((x > (LARGURA_TELA*0.2 + 5 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 6 * LARGURA_TELA*0.6 / 10))) {
			coluna = 5;
		}
		else if ((x > (LARGURA_TELA*0.2 + 6 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 7 * LARGURA_TELA*0.6 / 10))) {
			coluna = 6;
		}
		else if ((x > (LARGURA_TELA*0.2 + 7 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 8 * LARGURA_TELA*0.6 / 10))) {
			coluna = 7;
		}
		else if ((x > (LARGURA_TELA*0.2 + 8 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 9 * LARGURA_TELA*0.6 / 10))) {
			coluna = 8;
		}
		else if ((x > (LARGURA_TELA*0.2 + 9 * LARGURA_TELA*0.6 / 10)) && (x <= (LARGURA_TELA*0.2 + 10 * LARGURA_TELA*0.6 / 10))) {
			coluna = 9;
		}
	}
}

void setaColuna(double x, double y)
{
	if ((x >= LARGURA_TELA*0.2) && (x <= (LARGURA_TELA*0.2 + 10 * LARGURA_TELA*0.6 / 10))) {
		if ((y >= ALTURA_TELA*0.2) && (y <= (ALTURA_TELA*0.2 + LARGURA_TELA*0.6 / 10))) {
			linha = 0;
		}
		else if ((y > (ALTURA_TELA*0.2 + LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 2 * LARGURA_TELA*0.6 / 10))) {
			linha = 1;
		}
		else if ((y > (ALTURA_TELA*0.2 + 2 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 3 * LARGURA_TELA*0.6 / 10))) {
			linha = 2;
		}
		else if ((y > (ALTURA_TELA*0.2 + 3 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 4 * LARGURA_TELA*0.6 / 10))) {
			linha = 3;
		}
		else if ((y > (ALTURA_TELA*0.2 + 4 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 5 * LARGURA_TELA*0.6 / 10))) {
			linha = 4;
		}
		else if ((y > (ALTURA_TELA*0.2 + 5 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 6 * LARGURA_TELA*0.6 / 10))) {
			linha = 5;
		}
		else if ((y > (ALTURA_TELA*0.2 + 6 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 7 * LARGURA_TELA*0.6 / 10))) {
			linha = 6;
		}
		else if ((y > (ALTURA_TELA*0.2 + 7 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 8 * LARGURA_TELA*0.6 / 10))) {
			linha = 7;
		}
		else if ((y > (ALTURA_TELA*0.2 + 8 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 9 * LARGURA_TELA*0.6 / 10))) {
			linha = 8;
		}
		else if ((y > (ALTURA_TELA*0.2 + 9 * LARGURA_TELA*0.6 / 10)) && (y <= (ALTURA_TELA*0.2 + 10 * LARGURA_TELA*0.6 / 10))) {
			linha = 9;
		}
	}
}

void desenha_peca(struct peca *pecaX, float x_ini, float y_ini, float lado)
{
	int i, j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (pecaX->p[i][j] == 0 || pecaX->p[i][j] == 8 || pecaX->p[i][j] == 9) {
				continue;
			}
			tela_retangulo(x_ini + i*lado, y_ini + j*lado, x_ini + (i + 1)*lado, y_ini + (j + 1)*lado, 1, branco, pecaX->cor);
		}
	}
}

void desenha_pecas(struct peca *peca1, struct peca *peca2, struct peca *peca3)
{
	desenha_peca(peca1, peca1->x, peca1->y, peca1->lado_peca);
	desenha_peca(peca2, peca2->x, peca2->y, peca2->lado_peca);
	desenha_peca(peca3, peca3->x, peca3->y, peca3->lado_peca);
}

 void displayImagens()
 {
 	ALLEGRO_BITMAP *imagem = NULL;
    	imagem = al_load_bitmap("Trophy.png");
    	al_draw_scaled_bitmap(imagem, 0, 0, 180, 179, 241.5, 28, 80, 79, 0);
    	al_destroy_bitmap(imagem);
 }

void zeraEstruturas(int peca1[5][5], int peca2[5][5], int peca3[5][5])
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			peca1[i][j] = 0;
			peca2[i][j] = 0;
			peca3[i][j] = 0;
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			m[i][j] = 0;
			MatrizCores[i][j] = 0;
		}
	}
}

void menu_iniciar()
{
	tela_inicia_desenho();

	tela_retangulo(100, 140, 270, 300, 1, branco, verde);
	tela_texto(182, 217, 40, branco, "Play");

	tela_retangulo(290, 140, 460, 300, 1, branco, azul);
	tela_texto(375, 217, 27.8, branco, "How to Play");

	tela_retangulo(100, 320, 270, 480, 1, branco, laranja);
	tela_texto(182, 397, 30, branco, "Ranking");

	tela_retangulo(290, 320, 460, 480, 1, branco, vermelho);
	tela_texto(372, 397, 40, branco, "Quit");
}

void consomeLCCheia(int linhasCheias[10], int colunasCheias[10], ALLEGRO_SAMPLE *clearFull)
{
	int i, j, k = 0, l = 0, o;
	for (j = 0; j < 10; j++) {
		for (i = 0; i < 10; i++) {
			if (i == linhasCheias[k]) {
          				al_play_sample(clearFull, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				for (o = 0; o < 10; o++) {
					m[o][i] = 0;
				}
				k++;
				somatorioPontuacao(10);
			}
			if (i == colunasCheias[l]) {
          				al_play_sample(clearFull, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				for (o = 0; o < 10; o++) {
					m[i][o] = 0;
				}
				l++;
				somatorioPontuacao(10);
			}
		}
	}
}

void verificaLCCheia(ALLEGRO_SAMPLE *clearFull)
{
	int i, j, k = 0, l = 0, linha_cheia = 0, vet_linhasCheias[10], coluna_cheia = 0, vet_colunasCheias[10];
	for (i = 0; i < 10; i++) {
		vet_linhasCheias[i] = 999;
	}
	for (j = 0; j < 10; j++) {
		for (i = 0; i < 10; i++) {
			if (m[i][j] == 1) {
				linha_cheia++;
			}
		}
		if (linha_cheia == 10) {
			vet_linhasCheias[k] = j;
			k++;
		}
		linha_cheia = 0;
	}

	for (i = 0; i < 10; i++) {
		vet_colunasCheias[i] = 888;
	}
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (m[i][j] == 1) {
				coluna_cheia++;
			}
		}
		if (coluna_cheia == 10) {
			vet_colunasCheias[l] = i;
			l++;
		}
		coluna_cheia = 0;
	}
	consomeLCCheia(vet_linhasCheias, vet_colunasCheias, clearFull);
}

int diferenca_x(struct peca *pecaX)
{

	int dif;

	if (pecaX->p[0][0] == 8) {
		dif = 10;
	}
	else if (pecaX->p[0][0] == 9) {
		dif = 10;
	}
	else {
		dif = 10;
	}

	return dif;
}

struct coord
{
	int x;
	int y;
};

bool PecaEncaixa(struct peca *pecaX, int xo, int yo);

bool PecaEncaixaNaTabuleiro(struct peca *pecaX)
{
	for (int xTabuleiro = 0; xTabuleiro < 10; xTabuleiro++)
	{
		for (int yTabuleiro = 0; yTabuleiro < 10; yTabuleiro++)
		{
			if (PecaEncaixa(pecaX, xTabuleiro, yTabuleiro))
				return true;
		}
	}
	return false;
}



bool PecaEncaixa(struct peca *pecaX, int xo, int yo)
{
	struct coord posicoesOcupadas[25];
	int cont = 0;
	for (int xPeca = 0; xPeca < 5; xPeca++)
	{
		for (int yPeca = 0; yPeca < 5; yPeca++)
		{

			if (pecaX->p[xPeca][yPeca] == 1)
			{
				if ((xo + xPeca >= 0) && (xo+ xPeca <= 9) && (yo + yPeca >= 0) && (yo + yPeca <= 9))
				{

					posicoesOcupadas[cont].x = xPeca;
					posicoesOcupadas[cont].y = yPeca;
					cont++;

				} else {

					return false;

				}
			}
		}
	}

	for (i = 0; i < cont; i++)
	{
		if (m[xo + posicoesOcupadas[i].x][yo + posicoesOcupadas[i].y] == 1)
			return false;

	}

	return true;
}

int pontos_por_pecas(struct peca *pecaX){
	int lugares_ativos=0;
	for(i=0; i<5; i++){
		for(j=0; j<5; j++){
			if(pecaX->p[i][j]==1){
				lugares_ativos++;
			}
		}
	}
	return lugares_ativos;
}

bool gameOver(struct peca *peca1, struct peca *peca2, struct peca *peca3)
{
	if(peca1->peca_ativa==false && peca2->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca3)))
		{
			return true;
		}

	} else if(peca2->peca_ativa==false && peca3->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca1)))
		{
			return true;
		}
	}else if(peca1->peca_ativa==false && peca3->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca2)))
		{
			return true;
		}

	}else if(peca1->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca2) || PecaEncaixaNaTabuleiro(peca3)))
		{
			return true;
		}

	}else if(peca2->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca1) || PecaEncaixaNaTabuleiro(peca3)))
		{
			return true;
		}

	}else if(peca3->peca_ativa==false)
	{

		if(!(PecaEncaixaNaTabuleiro(peca1) || PecaEncaixaNaTabuleiro(peca2)))
		{
			return true;
		}

	}else
	{
		if(!(PecaEncaixaNaTabuleiro(peca1) || PecaEncaixaNaTabuleiro(peca2) || PecaEncaixaNaTabuleiro(peca3)))
		{
			return true;
		}
	}

	return false;

}


void displayPontuacao()
{
	sprintf(texto, "%d", pontuacao);

	arq_records = fopen(nomeArq, "r");
	if (arq_records != NULL) {
		fscanf(arq_records, "%d", &pontAnterior);
	}
	fclose(arq_records);

	sprintf(record, "%d", pontAnterior);

	tela_texto(180, 65, 40, branco, texto);
	tela_texto(380, 65, 40, laranja, record);
}

void somatorioPontuacao(int valorJogada){
  pontuacao += valorJogada;
}

void reiniciaJogo(struct peca *peca1, struct peca *peca2, struct peca *peca3, ALLEGRO_SAMPLE *soundEffect){
	if(var_reinicia_jogo){

		tela_retangulo(180, 195, 385, 400, 5, vermelho, branco);

		tela_retangulo(220, 345, 270, 375, 1, branco, verde);
		tela_texto(242.5, 358, 15, branco, "Yes");
		tela_retangulo(290, 345, 340, 375, 1, branco, vermelho);
		tela_texto(312.5, 358, 15, branco, "No");

		tela_texto(285, 245, 25, vermelho, "Game Over");
		tela_texto(285, 305, 20, vermelho, "Try Again?");
		if(tela_rato_x()>=220 && tela_rato_x()<=270 && tela_rato_y()>=345 && tela_rato_y()<=375){
			tela_retangulo(220, 345, 270, 375, 1, verde, branco);
			tela_texto(242.5, 358, 15, verde, "Yes");
			play_sound_yes++;
			if(play_sound_yes==1){
				al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			if(tela_rato_apertado()){
				tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
				zeraEstruturas(peca1->p, peca2->p, peca3->p);
				geraNovasPecas = 3;
				pontuacao = 0;
				play_sound_gameover = 0;
				var_reinicia_jogo = false;
			}
		}else if(tela_rato_x()>=290 && tela_rato_x()<=340 && tela_rato_y()>=345 && tela_rato_y()<=375){
			tela_retangulo(290, 345, 340, 375, 1, vermelho, branco);
			tela_texto(312.5, 358, 15, vermelho, "No");
			play_sound_no++;
			if(play_sound_no==1){
				al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			if(tela_rato_apertado()){
				tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
				zeraEstruturas(peca1->p, peca2->p, peca3->p);
				geraNovasPecas = 3;
				pontuacao = 0;
				play_sound_gameover = 0;
				var_reinicia_jogo = false;
				jogaJogo = false;
			} else {
				tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, marrom, transparente);
			}
		}else {
			play_sound_yes = 0;
			play_sound_no = 0;
		}
	}
}

void tela_help(ALLEGRO_SAMPLE *soundEffect)
{
	tela_inicia_desenho();

	tela_retangulo(480, 25, 540, 60, 1, branco, azul);
	tela_texto(508, 40, 20, branco, "Menu");

	if((tela_rato_x()>=480 && tela_rato_x()<=540) && (tela_rato_y()>=25 && tela_rato_y()<=60)){
		play_sound_back++;
		if(play_sound_back==1){
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		tela_retangulo(480, 25, 540, 60, 1, azul, branco);
		tela_texto(508, 40, 20, azul, "Menu");
		if(tela_rato_clicado())
		{
			how_to_play = false;
		}
	}else {
		play_sound_back = 0;
	}

	tela_retangulo(70, 70, 500, 500, 7, azul, branco);
	tela_texto(282, 32, 30, azul, "Como jogar?");
	tela_texto(280, 30, 30, branco, "Como jogar?");
	tela_texto(280, 95, 18, azul, "O objetivo do jogo é completar linhas e colu-");
	tela_texto(280, 115, 18, azul, "nas. As peças são geradas a aleatoriamente   ");
	tela_texto(280, 135, 18, azul, "cada vez que forem colocadas todas as peças ");
	tela_texto(280, 155, 18, azul, "peças no tabuleiro.                                       ");
	tela_texto(280, 185, 18, azul, "Você perde quando não conseguir encaixar    ");
	tela_texto(280, 205, 18, azul, "uma ou mais peças no tabuleiro.                    ");
	tela_texto(280, 235, 18, azul, "Os pontos são acrescidos da seguinte maneira:");
	tela_texto(280, 255, 18, azul, "1 ponto para cada tamanho da peça colocada ");
	tela_texto(280, 275, 18, azul, "no tabuleiro, ou seja, se você colocou uma pe-");
	tela_texto(280, 295, 18, azul, "ça de tamanho 9, ganhará 9 pontos. Caso uma");
	tela_texto(280, 315, 18, azul, "linha ou coluna seja concluída, são acrescidos  ");
	tela_texto(280, 335, 18, azul, "10 pontos extras.                                          ");
	tela_texto(280, 365, 18, azul, "Clique nas peças para pegar e clique na posi- ");
	tela_texto(280, 385, 18, azul, "ção desejada na matriz para largar. Se quiser ");
	tela_texto(280, 405, 18, azul, "largar a peça para selecionar outra, pressione");
	tela_texto(280, 425, 18, azul, "o botão direito, ela voltará para o seu lugar   ");
	tela_texto(280, 445, 18, azul, "de origem.                                                    ");

             letra_digitada = tela_le_tecla();
             if (letra_digitada == 'h') {
                          how_to_play= false;
                          jogaJogo = true;
             }

	if (tela_rato_clicado()) {
		tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
	} else {
		tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, marrom, transparente);
	}
}

void tela_ranking(ALLEGRO_SAMPLE *soundEffect)
{
	tela_inicia_desenho();
	tela_retangulo(480, 25, 540, 60, 1, branco, azul);
	tela_texto(508, 40, 20, branco, "Back");

	if((tela_rato_x()>=480 && tela_rato_x()<=540) && (tela_rato_y()>=25 && tela_rato_y()<=60)){
		play_sound_back++;
		if(play_sound_back==1){
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		tela_retangulo(480, 25, 540, 60, 1, azul, branco);
		tela_texto(508, 40, 20, azul, "Back");
		if(tela_rato_clicado())
		{
			ranking = false;
		}
	}else {
		play_sound_back = 0;
	}
	tela_retangulo(140, 350, 420, 500, 15, laranja, branco);

	arq_records = fopen(nomeArq, "r");
	if (arq_records != NULL) {
		fscanf(arq_records, "%d", &pontAnterior);
	}
	fclose(arq_records);

	sprintf(record, "%d", pontAnterior);
	tela_texto(280, 425, 80, laranja, record);
	tela_texto(282, 152, 50, laranja, "Maior pontuação");
	tela_texto(282, 202, 50, laranja, "feita até agora:");
	tela_texto(280, 150, 50, branco, "Maior pontuação");
	tela_texto(280, 200, 50, branco, "feita até agora:");

	if (tela_rato_clicado()) {
		tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
	} else {
		tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, marrom, transparente);
	}
}

void desenha_tela(struct peca *peca1, struct peca *peca2, struct peca *peca3, ALLEGRO_SAMPLE *soundEffect, ALLEGRO_SAMPLE *clearFull, ALLEGRO_SAMPLE *endGame, ALLEGRO_SAMPLE *dontFit, ALLEGRO_SAMPLE *selectBlock)
{

	tela_inicia_desenho();

	displayImagens();
	displayPontuacao();
	desenha_principal(LARGURA_TELA*0.2, ALTURA_TELA*0.2, LARGURA_TELA*0.6 / 10);
	desenha_pecas(peca1, peca2, peca3);
	tela_retangulo(480, 25, 540, 60, 1, branco, azul);
	tela_texto(508, 40, 20, branco, "Menu");
             tela_texto(150, 10, 15, branco, "Pressione H para acessar as instruções");

	if (geraNovasPecas == 3) {

		peca1->lado_peca = 0.2*LARGURA_TELA / 5;
		peca2->lado_peca = 0.2*LARGURA_TELA / 5;
		peca3->lado_peca = 0.2*LARGURA_TELA / 5;

		define_tipo_peca(peca1->p);
		define_cor_peca(peca1);

		define_tipo_peca(peca2->p);
		define_cor_peca(peca2);

		define_tipo_peca(peca3->p);
		define_cor_peca(peca3);

		peca1->peca_ativa = true;
		peca2->peca_ativa = true;
		peca3->peca_ativa = true;

		geraNovasPecas = 0;
	}

	if ((tela_rato_x() >= 480 && tela_rato_x() <= 540) && (tela_rato_y() >= 25 && tela_rato_y() <= 60)) {
		play_sound_menu++;
		if(play_sound_menu==1){
			al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		tela_retangulo(480, 25, 540, 60, 1, branco, branco);
		tela_texto(508, 40, 20, azul, "Menu");
	} else {
		play_sound_menu = 0;
	}

             letra_digitada = tela_le_tecla();
             if (letra_digitada == 'h') {
                          how_to_play = true;
                          jogaJogo = false;
             }
             if(how_to_play){
                          tela_help(soundEffect);
             }


	if (tela_rato_clicado()) {
		tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
		if ((x >= 480 && x <= 540) && (y >= 25 && y <= 60)) {
			jogaJogo = false;
		}
		else if ((tela_rato_x() >= peca1->x && tela_rato_x() <= peca1->x + 100) && (tela_rato_y() >= peca1->y && tela_rato_y() <= peca1->y + 100)) {
			movePeca = 1;
			peca1->lado_peca = 0.23*LARGURA_TELA / 4.15;
			al_play_sample(selectBlock, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		else if ((tela_rato_x() >= peca2->x && tela_rato_x() <= peca2->x + 100) && (tela_rato_y() >= peca2->y && tela_rato_y() <= peca2->y + 100)) {
			movePeca = 2;
			peca2->lado_peca = 0.23*LARGURA_TELA / 4.15;
			al_play_sample(selectBlock, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
		else if ((tela_rato_x() >= peca3->x && tela_rato_x() <= peca3->x + 100) && (tela_rato_y() >= peca3->y && tela_rato_y() <= peca3->y + 100)) {
			movePeca = 3;
			peca3->lado_peca = 0.23*LARGURA_TELA / 4.15;
			al_play_sample(selectBlock, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}
	}
	else {
		tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, marrom, transparente);
	}

	if(gameOver(peca1, peca2, peca3))
	{
		var_reinicia_jogo = true;

		arq_records = fopen(nomeArq, "r");
		if (arq_records != NULL) {
			fscanf(arq_records, "%d", &pontAnterior);
		}
		fclose(arq_records);

		if (pontuacao > pontAnterior) {
			arq_records = fopen(nomeArq, "w");
			fprintf(arq_records, "%d", pontuacao);
			fclose(arq_records);
		}

		play_sound_gameover++;
		if(play_sound_gameover==1){
			al_play_sample(endGame, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		}

		reiniciaJogo(peca1, peca2, peca3, soundEffect);
	}

	x = tela_rato_x();
	y = tela_rato_y();
	coluna = x;
	linha = y;
	setaLinha(x, y);
	setaColuna(x, y);

	if (movePeca == 1) {
		peca1->x = tela_rato_x() - diferenca_x(peca1);
		peca1->y = tela_rato_y() - 15;
		if (tela_rato_apertado()) {
			if ((tela_rato_x() >= LARGURA_TELA*0.2 && tela_rato_x() <= LARGURA_TELA*0.2 + 335) && (tela_rato_y() >= ALTURA_TELA*0.2 && tela_rato_y() <= ALTURA_TELA*0.2 + 335)) {
				movePeca = 0;
				peca1->x = 0.1*LARGURA_TELA;
				peca1->y = 0.8*ALTURA_TELA;
				setaLinha(x - diferenca_x(peca1), y);
				setaColuna(x - diferenca_x(peca1), y);

				if (PecaEncaixa(peca1, coluna, linha)) {
					for (i = 0; i < 5; i++) {
						for (j = 0; j < 5; j++) {
							if (peca1->p[i][j] == 1) {
								m[coluna + i][linha + j] = 1;
								MatrizCores[coluna + i][linha + j] = peca1->cor;
								ver_linhas = true;
							}
						}
					}

					somatorioPontuacao(pontos_por_pecas(peca1));
					peca1->peca_ativa = false;

					for (int f = 0; f < 5; f++) {
						for (int g = 0; g < 5; g++) {
							peca1->p[f][g] = 0;
						}
					}
					geraNovasPecas++;
				} else {
					al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					peca1->lado_peca = 0.2*LARGURA_TELA / 5;
				}

			}
		}
		else if(tela_rato_direito()){
			al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			movePeca = 0;
			peca1->lado_peca = 0.2*LARGURA_TELA / 5;
			peca1->x = 0.1*LARGURA_TELA;
			peca1->y = 0.8*ALTURA_TELA;
		}
	}
	else if (movePeca == 2) {
		peca2->x = tela_rato_x() - diferenca_x(peca2);
		peca2->y = tela_rato_y() - 15;
		if (tela_rato_apertado()) {
			if ((tela_rato_x() >= LARGURA_TELA*0.2 && tela_rato_x() <= LARGURA_TELA*0.2 + 335) && (tela_rato_y() >= ALTURA_TELA*0.2 && tela_rato_y() <= ALTURA_TELA*0.2 + 335)) {
				movePeca = 0;
				peca2->x = 0.4*LARGURA_TELA;
				peca2->y = 0.8*ALTURA_TELA;
				setaLinha(x - diferenca_x(peca2), y);
				setaColuna(x - diferenca_x(peca2), y);

				if (PecaEncaixa(peca2, coluna, linha)) {
					for (i = 0; i < 5; i++) {
						for (j = 0; j < 5; j++) {
							if (peca2->p[i][j] == 1) {
								m[coluna + i][linha + j] = 1;
								MatrizCores[coluna + i][linha + j] = peca2->cor;
								ver_linhas = true;
							}
						}
					}

					somatorioPontuacao(pontos_por_pecas(peca2));
					peca2->peca_ativa = false;

					for (int f = 0; f < 5; f++) {
						for (int g = 0; g < 5; g++) {
							peca2->p[f][g] = 0;
						}
					}
					geraNovasPecas++;
				} else {
					al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					peca2->lado_peca = 0.2*LARGURA_TELA / 5;
				}
			}
		}
		else if(tela_rato_direito()){
			al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			movePeca = 0;
			peca2->lado_peca = 0.2*LARGURA_TELA / 5;
			peca2->x = 0.4*LARGURA_TELA;
			peca2->y = 0.8*ALTURA_TELA;
		}
	}
	else if (movePeca == 3) {
		peca3->x = tela_rato_x() - diferenca_x(peca3);
		peca3->y = tela_rato_y() - 15;
		if (tela_rato_apertado()) {
			if ((tela_rato_x() >= LARGURA_TELA*0.2 && tela_rato_x() <= LARGURA_TELA*0.2 + 335) && (tela_rato_y() >= ALTURA_TELA*0.2 && tela_rato_y() <= ALTURA_TELA*0.2 + 335)) {
				movePeca = 0;
				peca3->x = 0.7*LARGURA_TELA;
				peca3->y = 0.8*ALTURA_TELA;
				setaLinha(x - diferenca_x(peca3), y);
				setaColuna(x - diferenca_x(peca3), y);

				if (PecaEncaixa(peca3, coluna, linha)) {
					for (i = 0; i < 5; i++) {
						for (j = 0; j < 5; j++) {
							if (peca3->p[i][j] == 1) {
								m[coluna + i][linha + j] = 1;
								MatrizCores[coluna + i][linha + j] = peca3->cor;
								ver_linhas = true;
							}
						}
					}

					somatorioPontuacao(pontos_por_pecas(peca3));
					peca3->peca_ativa = false;

					for (int f = 0; f < 5; f++) {
						for (int g = 0; g < 5; g++) {
							peca3->p[f][g] = 0;
						}
					}
					geraNovasPecas++;
				} else {
					al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					peca3->lado_peca = 0.2*LARGURA_TELA / 5;
				}
			}
		}
		else if(tela_rato_direito()){
			al_play_sample(dontFit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			movePeca = 0;
			peca3->lado_peca = 0.2*LARGURA_TELA / 5;
			peca3->x = 0.7*LARGURA_TELA;
			peca3->y = 0.8*ALTURA_TELA;
		}
	}

	if (ver_linhas) {
		verificaLCCheia(clearFull);
		ver_linhas = false;
	}

	tela_termina_desenho();
}

void funcional(struct peca *peca1, struct peca *peca2, struct peca *peca3, ALLEGRO_SAMPLE *soundEffect, ALLEGRO_SAMPLE *clearFull, ALLEGRO_SAMPLE *endGame, ALLEGRO_SAMPLE *dontFit, ALLEGRO_SAMPLE *selectBlock)
{
	if (jogaJogo == true) {
		desenha_tela(peca1, peca2, peca3, soundEffect, clearFull, endGame, dontFit, selectBlock);
	} else if(how_to_play){
		tela_help(soundEffect);
	} else if(ranking){
		tela_ranking(soundEffect);
	} else {
		menu_iniciar();
		if ((tela_rato_x() >= 100 && tela_rato_x() <= 270) && (tela_rato_y() >= 140 && tela_rato_y() <= 300)) {
        			soundPlay++;
			tela_retangulo(100, 140, 270, 300, 1, branco, branco);
			tela_texto(182, 217, 40, verde, "Play");
		        	if(soundPlay==1){
		         		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		        	}
		}
		else if ((tela_rato_x() >= 290 && tela_rato_x() <= 460) && (tela_rato_y() >= 140 && tela_rato_y() <= 300)) {
        			soundHTP++;
			tela_retangulo(290, 140, 460, 300, 1, branco, branco);
			tela_texto(375, 217, 27.8, azul, "How to Play");
		        	if(soundHTP==1){
		         		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		        	}
		}
		else if ((tela_rato_x() >= 100 && tela_rato_x() <= 270) && (tela_rato_y() >= 320 && tela_rato_y() <= 480)) {
			tela_retangulo(100, 320, 270, 480, 1, branco, branco);
			tela_texto(182, 397, 30, laranja, "Ranking");
        			soundRank++;
		        	if(soundRank==1){
		         		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		        	}
		}
		else if ((tela_rato_x() >= 290 && tela_rato_x() <= 460) && (tela_rato_y() >= 320 && tela_rato_y() <= 480)) {
			tela_retangulo(290, 320, 460, 480, 1, branco, branco);
			tela_texto(372, 397, 40, vermelho, "Quit");
        			soundQuit++;
		        	if(soundQuit==1){
		         		al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
		        	}
		}

		if(soundPlay!=0){
			if((tela_rato_x()<=100 || tela_rato_x()>=270) || (tela_rato_y()<=140 || tela_rato_y()>= 300)){
				soundPlay=0;
			}
		}


		if(soundHTP!=0){
			if((tela_rato_x()<=290 || tela_rato_x()>=460) || (tela_rato_y()<=140 || tela_rato_y()>= 300)){
				soundHTP=0;
			}
		}

		if(soundRank!=0){
			if((tela_rato_x()<=100 || tela_rato_x()>=270) || (tela_rato_y()<=320 || tela_rato_y()>= 480)){
				soundRank=0;
			}
		}

		if(soundQuit!=0){
			if((tela_rato_x()<=290 || tela_rato_x()>=460) || (tela_rato_y()<=320 || tela_rato_y()>= 480)){
				soundQuit=0;
			}
		}

		if (tela_rato_clicado()) {
			if ((tela_rato_x() >= 100 && tela_rato_x() <= 270) && (tela_rato_y() >= 140 && tela_rato_y() <= 300)) {
				jogaJogo = true;
			}
			else if ((tela_rato_x() >= 290 && tela_rato_x() <= 460) && (tela_rato_y() >= 320 && tela_rato_y() <= 480)) {
				varbreak = true;
			}
			else if ((tela_rato_x() >= 290 && tela_rato_x() <= 460) && (tela_rato_y() >= 140 && tela_rato_y() <= 300)) {
				how_to_play = true;
			}
			else if ((tela_rato_x() >= 100 && tela_rato_x() <= 270) && (tela_rato_y() >= 320 && tela_rato_y() <= 480)) {
				ranking = true;
			}
			tela_circulo(tela_rato_x(), tela_rato_y(), 5, 2, amarelo, transparente);
		} else {

			tela_circulo(tela_rato_x(), tela_rato_y(), 3, 1, marrom, transparente);

		}
	}

	tela_termina_desenho();
}

int main(void)
{
	srand(time(NULL));

	al_init_image_addon();

	al_install_audio();
  	al_init_acodec_addon();

	ALLEGRO_SAMPLE *backgroundSound = al_load_sample("./sound/background.wav");
	ALLEGRO_SAMPLE *soundEffect = al_load_sample("./sound/select.wav");
	ALLEGRO_SAMPLE *clearFull = al_load_sample("./sound/clearFull.wav");
	ALLEGRO_SAMPLE *endGame = al_load_sample("./sound/gameover.wav");
	ALLEGRO_SAMPLE *dontFit = al_load_sample("./sound/dontfit.wav");
	ALLEGRO_SAMPLE *selectBlock = al_load_sample("./sound/selectBlock.wav");

	al_reserve_samples(6);

	tela_inicio(LARGURA_TELA, ALTURA_TELA, "tsinc");

	struct peca a, b, c;
	struct peca *peca1 = &a;
	struct peca *peca2 = &b;
	struct peca *peca3 = &c;

	peca1->x = 0.1*LARGURA_TELA;
	peca1->y = 0.8*ALTURA_TELA;
	peca2->x = 0.4*LARGURA_TELA;
	peca2->y = 0.8*ALTURA_TELA;
	peca3->x = 0.7*LARGURA_TELA;
	peca3->y = 0.8*ALTURA_TELA;

	zeraEstruturas(peca1->p, peca2->p, peca3->p);

	al_play_sample(backgroundSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, 0);

	while (!varbreak) {
		funcional(peca1, peca2, peca3, soundEffect, clearFull, endGame, dontFit, selectBlock);
	}

  	al_destroy_sample(backgroundSound);
  	al_destroy_sample(soundEffect);
  	al_destroy_sample(clearFull);
  	al_destroy_sample(endGame);
  	al_destroy_sample(dontFit);
  	al_destroy_sample(selectBlock);

	tela_fim();

	return 0;
}