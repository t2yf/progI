#ifndef MUNDO
#define MUNDO

#include "fila.h"
#include "fprio.h"

/*Defines gerais ============================================================================*/
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 

/*Defines do mundo ===========================================================================*/
#define N_TAMANHO_MUNDO 20000 
#define N_HABILIDADES  10 
#define N_HEROIS  N_HABILIDADES*5 
#define N_BASES  N_HEROIS/5 
#define N_MISSOES T_FIM_DO_MUNDO/100 

/*Structs auxiliares ========================================================================*/

/*Coordenadas do mundo*/
struct coordenadas_t
{
    int x;
    int y;
};

/*Declaração das Entidades ==================================================================*/

/*Atributos dos herois*/
struct heroi_t
{
    int id;
    int xp; /*experiencia*/
    int pac; /*paciencia*/
    int vel; /*velocidade*/
    struct cjto_t *hab; /*conjunto de habilidades do heroi*/
    int id_base; /*base atual do heroi*/ 

    int vivo; /*estado do heroi: vivo ou morto*/
};

/*Atributos das bases*/
struct base_t
{
    int id;
    int lotacao; /*numero maximo de herois possivel*/
    struct cjto_t *heroi_pres; /*conjunto dos herois presentes: equipes para as missoes*/
    struct lista_t *espera; /*fila de espera para entrar na base*/
    struct coordenadas_t local; /*localizacao*/

    /*Estatisticas*/
    int max_fila; /*qual o maximo de herois que ficou na fila de espera*/
    int part_missao; /*quantas vezes essa base participou de uma missao*/
};

/*Atributos das missoes*/
struct missao_t
{
    int id;
    struct cjto_t *hab_requerida; /*conjunto das habilidades requeridas*/
    int perigo; /*nivel de perigo da missao*/
    struct coordenadas_t local_missao; /*localizacao da missao*/

    /*Estatisticas*/
    int tentativa; /*quantidade de tentativas para cumprir a missao*/
    int cumprida; /*missao cumprida ou nao*/
};

/*Atributos do mundo*/
struct mundo_t
{
    int num_herois;
    struct heroi_t herois[N_HEROIS]; /*vetor de herois*/

    int num_bases;
    struct base_t bases[N_BASES]; /*vetor de bases*/

    int num_missoes;
    struct missao_t missoes[N_MISSOES]; /*vetor de missoes*/

    struct coordenadas_t tam_mundo; /*dimensao do mundo*/
    int fim_do_mundo; /*tempo maximo da duracao do mundo*/
    int relogio; /*tempo atual do mundo*/
    struct fprio_t *lef; /*Lista de Eventos Futuros*/

    /*Estatisticas*/
    int eventos_tratados; /*quantos eventos foram tratados ao longo da execucao*/
    int qtde_missoes_cumpridas; /*quantas missoes*/
};

/*Funcoes auxiliares ========================================================================*/

/*
* Retorna um numero aleatorio entre min e max
*/
int aleat(int min, int max);

/*Inicializar Mundo e Entidades =============================================================*/

/*
* Cria o mundo e inicializa seus atributos
* Retorna uma struct 
*/
struct mundo_t cria_mundo();

/*
* Recebe um ponteiro para o mundo e inicializa os atributos de cada entidade dos
* vetores herois, bases e missoes
*/
void inicializa_mundo(struct mundo_t *m);

/*
* Destroi os campos alocados dinamicamente das entidades dos vetores
* herois, bases e missoes. Alem de destruir a LEF
*/
void destroi_entidades(struct mundo_t m);

#endif