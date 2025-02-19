#ifndef EVENTOS
#define EVENTOS

#include "mundo.h"

/*Tipos dos Eventos ===========================================================================*/

#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

/*Structs ======================================================================================*/

/*Informacoes necessarias para cada evento*/
struct evento_t
{
    int id_heroi; /*qual heroi estah participando do evento*/
    int id_base; /*qual base estah participando do evento*/
    int id_missao; /*qual missao estah participando do evento*/
};

/*Informacoes para distancia entre base e missao*/
struct dist_base_missao_t
{
    int id_base; /*qual base esta sendo analisado*/
    int dist; /*qual a distancia ate a missao*/
};

/*Eventos ======================================================================================*/

/*
* Armazena as informacoes necessarias para cada evento
* Retorna um ponteiro com essas informacoes ou NULL em caso de falha
*/
struct evento_t *cria_info_evento(int id_heroi, int id_base, int id_missao);

/*
* Destroi o ponteiro para as informacoes
*/
struct evento_t *destroi_info_evento(struct evento_t *ev);

/*
* Heroi indicado VIAJA da base atual para a base indicada.
* O tempo de chegada eh calculado a depender da distancia entre as bases e da velocidade do heroi.
* Eh inserido o evento CHEGA na lef.
*/
void evento_viaja(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b);

/*
* Heroi indicado SAI da base indicada, no tempo indicado
* O evento VIAJA eh inserido na lef, com uma base aleatoria
* O evento AVISA eh inserido na lef, para ser possivel entrar o proximo heroi que aguarda
* na fila
*/
void evento_sai(struct mundo_t m, int tempo,  struct heroi_t *h, struct base_t *b);

/*
* Heroi indicado ENTRA na base indicada, no tempo indicado.
* Eh calculado o tempo de permanecia na base, o heroi saira apos ela;
* O evento SAI eh inserido na lef. 
*/
void evento_entra(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b); 

/*
* O porteiro da base indicada eh avisado que o heroi indicado estah na fila de espera, no tempo 
* indicado, caso haja espaco na base, eh admitido a entrada do heroi na base.
* O evento ENTRA eh inserido na lef.
*/
void evento_avisa(struct mundo_t m, int tempo,  struct base_t *b);

/*
* Heroi indicado ESPERA na base indicada, no tempo indicado.
* O heroi eh inserido na fila de espera da base.
* O evento AVISA eh inserido na lef.
*/
void evento_espera(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b);

/*
* Heroi indicado DESISTE de ficar na base indicada, no tempo indicado.
* O evento ENTRA eh inserido na lef, com uma base aleatoria.
*/
void evento_desiste(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b);

/*
* Heroi indicado CHEGA na base indicada, no tempo indicado.
* Com base na paciencia do heroi, tamanho da fila e lotacao da base eh decidido se o heroi ESPERA .
* ou DESISTE de entrar na base.
* O evento ESPERA eh inserido na lef, caso o heroi espere
* O evento DESISTE eh inserido na lef, caso o heroi desista
*/
void evento_chega(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b);

/*
* O mundo acaba, nenhum novo evento pode ser executado
* Eh apresentado as estatisticas de cada base: lotacao, fila maxima, qtde de missoes que 
* participou
* Eh apresentado as estatisticas de cada heroi: paciencia, velocidade, experiencia e habilidades que 
* possui
* Eh apresentado as estatisticas gerais: qtde de eventos tratados, missoes cumpridas,
* relacao tentativas/missao e taxa de mortalidade.
*/
void evento_fim(struct mundo_t m, int tempo);

/*
* Heroi indicado MORRE no tempo indicado na missao indicada
* O status do heroi muda para morto e ele eh retirado da base em que estava
* O evento AVISA eh inserido na lef
*/
void evento_morre(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b, struct missao_t *mi);

/*
* A missao indicada sera cumprida pela base mais proxima que contenha um conjunto de habilidades
* que satisfaca o requerido pela missao. Caso isso nao ocorra, a missao sera adiada em um dia.
* Caso ocorra, eh avaliado se os herois envolvidos morrem ou nao, se nao morrerem, seu xp aumenta
* O evento MISSAO eh inserido na lef, se nao for resolvida
* O evento MORRE eh inserido na lef, caso algum heroi morra
*/
void evento_missao(struct mundo_t *m, int tempo, struct missao_t *mi);

/*
* Inclui na lef o evento FIM com o tempo do final do mundo.
* Cada heroi CHEGA em alguma base nos tres primeiros dias (ate tempo == 4320).
* Cada missao eh agendada para algum momento entre 0 - final do mundo.
*/
void eventos_iniciais(struct mundo_t m);

/*
* Extrai os dados da lef e executa o evento relacionado.
* Sao destruidas apenas os eventos ate o fim do mundo, ou seja, eh necessario destruir.
* os eventos da lef que estao alem do fim do mundo.
*/
void simulacao(struct mundo_t m);

#endif