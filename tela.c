// inclui as definicoes
#include "tela.h"
#include <stdio.h>

// Os includes do allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// tempo entre telas (em segundos)
// depende da itera��o entre o SO e o allegro e suas configura��es.
// pode ser que isto n�o funcione.
#define PERIODO (1/30.0)

// fila para receber os eventos do teclado
ALLEGRO_EVENT_QUEUE *tela_eventos_teclado;
void tela_inicializa_teclado(void)
{
  al_install_keyboard();
  // cria e inicializa a fila de eventos do teclado
  tela_eventos_teclado = al_create_event_queue();
  al_register_event_source(tela_eventos_teclado,
                           al_get_keyboard_event_source());
}

void tela_inicializa_janela(float l, float a, char n[])
{
  // cria uma janela
  ALLEGRO_DISPLAY *janela;
  janela = al_create_display(l, a);
  // esconde o cursor do mouse
  al_hide_mouse_cursor(janela);
  al_set_window_title(janela, n);
}

// vetor com as cores
#define NCORES 100 // n�mero m�xim ode cores diferentes
ALLEGRO_COLOR cores[NCORES];
int ncores_predef = 10;  // quantas cores s�o predefinidas
int prox_cor = 10; // numero da proxima cor definida pelo usuario
void tela_inicializa_cores(void)
{
  cores[transparente] = al_map_rgba_f(0, 0, 0, 0);
  cores[azul]         = al_map_rgb_f(0, 0, 1);
  cores[vermelho]     = al_map_rgb_f(1, 0, 0);
  cores[verde]        = al_map_rgb_f(0, 1, 0);
  cores[amarelo]      = al_map_rgb_f(1, 1, 0);
  cores[preto]        = al_map_rgb_f(0, 0, 0);
  cores[laranja]      = al_map_rgb_f(1, 0.65, 0);
  cores[rosa]         = al_map_rgb_f(1, 0, 0.5);
  cores[branco]       = al_map_rgb_f(1, 1, 1);
  cores[marrom]       = al_map_rgb_f(0.58, 0.29, 0);
}

int tela_cria_cor(float vm, float az, float vd)
{
  int cor = prox_cor;
  prox_cor = prox_cor+1;
  if (prox_cor >= NCORES) {
    prox_cor = ncores_predef;
  }
  cores[cor] = al_map_rgb_f(vm, az, vd);
  return cor;
}

void tela_inicio(int largura, int altura, char nome[])
{
  // inicializa os subsistemas do allegro
  al_init();
  al_install_mouse();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();

  // inicializa a tela
  tela_inicializa_janela(largura, altura, nome);
  tela_inicializa_teclado();
  tela_inicializa_cores();
}


void tela_fim(void)
{
  // badabum!
  al_uninstall_system();
}

int conta_telas = 0;
void tela_termina_desenho(void)
{
  conta_telas++;
  double quando_a_tela_deve_ser_mostrada = conta_telas * PERIODO;
  double agora = al_get_time();
  double tempo_ate_mostrar = quando_a_tela_deve_ser_mostrada - agora;
  if (tempo_ate_mostrar > 0) {
    // � muito cedo, d� uma cochilada
    al_rest(tempo_ate_mostrar);
  }
  // faz a magica! -- na verdade, se o allegro t� instalado corretamente,
  // essa fun��o deve esperar pra mostrar na hora certa e o que t� acima
  // pode ser retirado
  al_flip_display();
}


void tela_inicia_desenho(void)
{
  // limpa todo o canvas em preto
  al_clear_to_color(cores[preto]);
}

void tela_circulo(float x, float y, float r, float l, int corl, int corint)
{
  // preenche
  al_draw_filled_circle(x, y, r, cores[corint]);
  // faz o contorno
  al_draw_circle(x, y, r, cores[corl], l);
}

void tela_linha(float x1, float y1, float x2, float y2, float l, int corl)
{
  al_draw_line(x1, y1, x2, y2, cores[corl], l);
}

void tela_retangulo(float x1, float y1, float x2, float y2, float l,
                    int corl, int corint)
{
  al_draw_filled_rectangle(x1, y1, x2, y2, cores[corint]);
  al_draw_rectangle(x1, y1, x2, y2, cores[corl], l);
}

// tem que ter uma fonte para poder escrever
ALLEGRO_FONT *fonte = NULL;
int tamanho_das_letras = 0;

void tela_prepara_fonte(int tam)
{
  // se se quer o mesmo tamanho que antes, usa a mesma
  if (tam == tamanho_das_letras) return;

  // se ja tinha uma fonte carregada, livra-se dela antes de carregar outra
  if (fonte != NULL) {
    al_destroy_font(fonte);
  }

  // carrega uma fonte, para poder escrever na tela
  fonte = al_load_font("DejaVuSans.ttf", tam, 0);
  if (fonte == NULL) {
    al_uninstall_system();
    printf("\n\nERRO FATAL\n");
    printf("ARQUIVO QUE DEFINE DESENHO DAS LETRAS (DejaVuSans.ttf) NAO ENCONTRADO.\n"
           "COPIE ESSE ARQUIVO, OU MUDE tela.c PARA USAR UM ARQUIVO QUE EXISTA.\n\n");
    exit(1);
  }
  tamanho_das_letras = tam;
}

void tela_texto(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y-tam/2, ALLEGRO_ALIGN_CENTRE, t);
}

void tela_texto_esq(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y, ALLEGRO_ALIGN_RIGHT, t);
}

void tela_texto_dir(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y, ALLEGRO_ALIGN_LEFT, t);
}

float tela_rato_x(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  return al_get_mouse_state_axis(&rato, 0);
}

float tela_rato_y(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  return al_get_mouse_state_axis(&rato, 1);
}

bool tela_rato_apertado(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  // s� nos interessa o bot�o da esquerda
  return al_mouse_button_down(&rato, 1);
}

bool tava_apertado = false;
float x_clicado = 0;
float y_clicado = 0;
bool tela_rato_clicado(void)
{
  bool ta_apertado = tela_rato_apertado();
  bool foi_clicado = (!tava_apertado && ta_apertado);
  if (foi_clicado) {
    x_clicado = tela_rato_x();
    y_clicado = tela_rato_y();
  }
  tava_apertado = ta_apertado;
  return foi_clicado;
}

float tela_rato_x_clique(void)
{
  return x_clicado;
}

float tela_rato_y_clique(void)
{
  return y_clicado;
}

char tela_le_tecla(void)
{
  ALLEGRO_EVENT ev;

  while (al_get_next_event(tela_eventos_teclado, &ev)) {
    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
      int c = ev.keyboard.unichar;
      if (c == '\r') c = '\n'; // corrige windowscentrismo
      // s� retorna caracteres imprim�veis, backspace e enter
      if ((c >= ' ' && c <= '~') || c == '\b' || c == '\n') {
        return (char) c;
      }
    }
  }
  // nada foi pressionado (ou foi pressionado algo n�o imprim�vel)
  return '\0';
}
