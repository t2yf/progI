#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "mundo.h"
#include "conjunto.h"

/*Funcoes auxiliares ============================================================================*/

int aleat(int min, int max)
{
    int num_aleat;
    num_aleat = rand()%(max - min +1) + min;
    return num_aleat;
}

/*Funcoes principais ============================================================================*/

struct mundo_t cria_mundo()
{
    struct mundo_t m;

    m.num_herois = N_HEROIS;
    m.num_bases = N_BASES;
    m.num_missoes = N_MISSOES;
    m.tam_mundo.x = N_TAMANHO_MUNDO;
    m.tam_mundo.y = N_TAMANHO_MUNDO;
    m.fim_do_mundo = T_FIM_DO_MUNDO;
    m.relogio = T_INICIO;
    m.lef = fprio_cria();

    /*Estatisticas*/
    m.eventos_tratados = 0;
    m.qtde_missoes_cumpridas = 0;
    
    return m;
}

void inicializa_heroi(struct heroi_t h[])
{
    int tam_hab;

    for(int i = 0; i < N_HEROIS; i++)
    {
        h[i].id = i;
        h[i].xp = 0;

        /*Limites dos valores aleatorios conforme enunciado*/
        h[i].pac = aleat(0, 100);
        h[i].vel = aleat(50, 5000);
        tam_hab = aleat(1, 3);
        h[i].hab = cjto_aleat(tam_hab, N_HABILIDADES);

        h[i].vivo = 1;
    }
}

void inicializa_base(struct base_t b[])
{
    int x_aleat, y_aleat;  

    for(int i = 0; i < N_BASES; i++)
    {
        /*Limites dos valores aleatorios conforme enunciado*/
        x_aleat = aleat(0, N_TAMANHO_MUNDO -1);
        y_aleat = aleat(0, N_TAMANHO_MUNDO -1);

        b[i].id = i;
        b[i].local.x = x_aleat;
        b[i].local.y = y_aleat;
        b[i].lotacao = aleat(3, 10);
        b[i].heroi_pres = cjto_cria(N_HEROIS); 
        b[i].espera = fila_cria();

        /*Estatisticas*/
        b[i].max_fila = 0;
        b[i].part_missao = 0;
    }
}

void inicializa_missao(struct missao_t mi[])
{
    int x_aleat, y_aleat, hab; 

    for(int i = 0; i < N_MISSOES; i++)
    {
        /*Limites dos valores aleatorios conforme enunciado*/
        x_aleat = aleat(0, N_TAMANHO_MUNDO -1);
        y_aleat = aleat(0, N_TAMANHO_MUNDO -1);

        
        mi[i].id = i;
        mi[i].local_missao.x = x_aleat;
        mi[i].local_missao.y = y_aleat;
        hab = aleat(6, 10);
        mi[i].hab_requerida = cjto_aleat(hab, N_HABILIDADES);
        mi[i].perigo = aleat(0, 100);

        /*Estatisticas*/
        mi[i].tentativa = 0;
        mi[i].cumprida = 0;
    }
}

void inicializa_mundo(struct mundo_t *m)
{
    inicializa_base(m->bases);
    inicializa_heroi(m->herois);
    inicializa_missao(m->missoes);
}

void destroi_heroi(struct heroi_t h[])
{
    for(int i = 0; i < N_HEROIS; i++)
    {
        cjto_destroi(h[i].hab);
    } 
}

void destroi_base(struct base_t b[])
{
    for(int i = 0; i < N_BASES; i++)
    {
        cjto_destroi(b[i].heroi_pres);
        fila_destroi(b[i].espera);
    }
}

void destroi_missao(struct missao_t mi[])
{
    for(int i = 0; i < N_MISSOES; i++)
    {
        cjto_destroi(mi[i].hab_requerida);
    }
}

void destroi_entidades(struct mundo_t m)
{
    destroi_base(m.bases);
    destroi_heroi(m.herois);
    destroi_missao(m.missoes);
    
    /*Destruir a LEF*/
    fprio_destroi(m.lef);
}

