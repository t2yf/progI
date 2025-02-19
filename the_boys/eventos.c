#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "mundo.h"
#include "conjunto.h"
#include "eventos.h"

#define TRES_DIAS 4320
#define AMANHA 1440

/*Criar e destruir "item" da LEF ===============================================================*/
struct evento_t *cria_info_evento(int id_heroi, int id_base, int id_missao)
{
    struct evento_t *ev;
    
    if(!(ev = malloc(sizeof(struct evento_t))))
        return NULL;

    ev->id_heroi = id_heroi;
    ev->id_base = id_base;
    ev->id_missao = id_missao;

    return ev;
}

struct evento_t *destroi_info_evento(struct evento_t *ev)
{
    free(ev);
    ev = NULL;

    return ev;
}

/*Funcoes auxiliares ==========================================================================*/

/*Pelas bases ser um vetor pequeno, será utilizado o Select Sort*/
void selectSort(struct dist_base_missao_t v[], int tam)
{
    int min, aux, aux_id;

    for(int i = 0; i < tam; i++){
        min = i;
        for(int j=i+1; j<=tam; j++){
            if(v[j].dist < v[min].dist)
            {
                min = j;
            }
        }

        /*Trocar id e distancia*/
        aux = v[min].dist;
        aux_id = v[min].id_base;

        v[min].dist = v[i].dist;
        v[min].id_base = v[i].id_base;

        v[i].dist = aux;
        v[i].id_base = aux_id;
    }
}
//int calcula_distancia()

/*Eventos ======================================================================================*/

void evento_viaja(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b)
{
    int dist, cordx, cordy, duracao, chegada;
    struct evento_t *ev;

    /*Calculo da distancia cartesiana entre base atual e a base destino*/
    cordx = abs(m.bases[h->id_base].local.x - b->local.x);
    cordy = abs(m.bases[h->id_base].local.y - b->local.y);
    dist = sqrt(cordx*cordx + cordy*cordy);

    /*Calculo da duracao da viajem*/
    duracao = dist/h->vel; 

    /*Tempo de chegada do heroi na base*/
    chegada = tempo + duracao;

    printf("%6d: VIAJA   HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
        tempo, 
        h->id,
        m.bases[h->id_base].id,
        b->id,
        dist,
        h->vel, 
        chegada);
    
    /*Inserir evento CHEGA na LEF*/
    ev = cria_info_evento(h->id, b->id, 0);
    fprio_insere(m.lef, ev, CHEGA, chegada);
}

void evento_sai(struct mundo_t m, int tempo,  struct heroi_t *h, struct base_t *b)
{
    int base_aleat;
    struct evento_t *ev;

    /*Retirar heroi do conjunto de herois presentes*/ 
    cjto_retira(b->heroi_pres, h->id);

    /*Base destino aleatoria*/
    base_aleat = aleat(0, m.num_bases -1);

    printf("%6d: SAI     HEROI %2d BASE %d (%2d/%2d)\n", 
        tempo, 
        h->id, 
        b->id, 
        cjto_card(b->heroi_pres), 
        b->lotacao);

    /*Inserir evento VIAJA na LEF*/
    ev = cria_info_evento(h->id, base_aleat, 0);
    fprio_insere(m.lef, ev, VIAJA, tempo);

    /*Inserir evento AVISA na LEF*/
    ev = cria_info_evento(0, b->id, 0);
    fprio_insere(m.lef, ev, AVISA, tempo);
}

void evento_entra(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b) 
{
    int permanencia, saida;
    struct evento_t *ev;

    permanencia = 15 + h->pac*aleat(1, 20);
    saida = tempo + permanencia;

    printf("%6d: ENTRA   HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
        tempo, 
        h->id, 
        b->id, 
        cjto_card(b->heroi_pres), 
        b->lotacao, saida);

    /*Inserir evento SAI na LEF*/
    ev = cria_info_evento(h->id, b->id, 0);
    fprio_insere(m.lef, ev, SAI, saida);
}

void evento_avisa(struct mundo_t m, int tempo, struct base_t *b) /*struct heroi_t *h,*/
{
    int heroi_fila;
    struct evento_t *ev;
    
    printf("%6d: AVISA   PORTEIRO BASE %d (%2d/%2d)", 
        tempo, 
        b->id, 
        cjto_card(b->heroi_pres), 
        b->lotacao);
    printf(" FILA [ ");
    fila_imprime(b->espera);
    printf(" ]\n");

    /*Nao atingiu a lotacao e a fila nao esta vazia*/
    while(cjto_card(b->heroi_pres) < b->lotacao && fila_tamanho(b->espera) != 0)
    {
        /*Retira heroi da fila e coloca no conjunto dos herois presentes*/
        dequeue(b->espera, &heroi_fila);
        cjto_insere(b->heroi_pres, heroi_fila);
        printf("%6d: AVISA   PORTEIRO BASE %d ADMITE %2d\n", tempo, b->id, heroi_fila);

        /*Inserir evento Entra na LEF*/
        ev = cria_info_evento(heroi_fila, b->id, 0);
        fprio_insere(m.lef, ev, ENTRA, tempo);
    }
}

void evento_espera(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b)
{
    struct evento_t *ev;
    
    /*Adicionar heroi na fila*/
    enqueue(b->espera, h->id);

    /*Estatistica*/
    if(fila_tamanho(b->espera) > b->max_fila)
        b->max_fila = fila_tamanho(b->espera);

    /*Imprimir os herois na fila, exceto o atual*/
    printf("%6d: ESPERA  HEROI %2d BASE %d (%2d)\n", 
        tempo, 
        h->id, 
        b->id, 
        fila_tamanho(b->espera) - 1);
    
    /*Evento avisa*/
    ev = cria_info_evento(0, b->id, 0);
    fprio_insere(m.lef, ev, AVISA, tempo);
}

void evento_desiste(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b) 
{
    /*Escolher uma base aleatória -> relacionado ao evento viaja*/
    int base_destino;
    struct evento_t *ev;

    base_destino = aleat(0, m.num_bases -1);

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, h->id, b->id);

    /*Inserir evento VIAJA na LEF*/
    ev = cria_info_evento(h->id, base_destino, 0);
    fprio_insere(m.lef, ev, VIAJA, tempo);
}

void evento_chega(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b)
{
    int espera = 0;
    struct evento_t *ev;

    /*Atualiza base do heroi*/
    h->id_base = b->id;

    /*Analisar de heroi espera ou desiste*/

    /*Se não estiver lotado e não tiver fila, o heroi espera*/
    if(cjto_card(b->heroi_pres) < b->lotacao && fila_tamanho(b->espera) == 0)
        espera = 1;
    else
        /*Se a paciencia do heroi for 10x maior que o tamanho da fila, ele espera*/
        if(h->pac > 10*fila_tamanho(b->espera))
            espera = 1;
    
    printf("%6d: CHEGA   HEROI %2d BASE %d ", tempo, h->id, h->id_base);
    if(espera)
    {
        /*Inserir evento ESPERA na LEF*/
        ev = cria_info_evento(h->id, h->id_base, 0); 
        fprio_insere(m.lef, ev, ESPERA, tempo);
        printf("(%2d/%2d) ESPERA \n", cjto_card(b->heroi_pres), b->lotacao);
    }
    else
    {
        /*Inserir evento DESISTE na LEF*/
        ev = cria_info_evento(h->id, h->id_base , 0); 
        fprio_insere(m.lef, ev, DESISTE, tempo);
        printf("(%2d/%2d) DESISTE\n", cjto_card(b->heroi_pres), b->lotacao);
    }
}

void evento_morre(struct mundo_t m, int tempo, struct heroi_t *h, struct base_t *b, struct missao_t *mi)
{
    struct evento_t *ev;
    
    /*Retirar heroi do conjunto de herois presentes*/
    cjto_retira(b->heroi_pres, h->id);

    /*Mudar estado do heroi para morto*/
    h->vivo = 0;

    /*Add no evento_t id_heroi, id_base, id_missao*/
    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", tempo, h->id, mi->id);

    /*Insere evento AVISA na LEF*/
    ev = cria_info_evento(0, b->id, 0);
    fprio_insere(m.lef, ev, AVISA, tempo);
}

void evento_missao(struct mundo_t *m, int tempo, struct missao_t *mi) //int tempo,
{
    int dist, cordx, cordy, bmp, contem, risco, risco_rand, amanha; //, dist_inic, ;
    struct dist_base_missao_t *dist_bm;
    struct cjto_t *hab_base;
    struct cjto_t *hab_base_aux;

    struct evento_t *ev;

    /*Alocar um vetor para armazenar as distancias*/
    if(!(dist_bm = malloc(sizeof(struct dist_base_missao_t)*m->num_bases)))
        return;
    
    /*Calcular a distancia entre a base do heroi e a missao*/
    for(int i = 0; i < m->num_bases; i ++)
    {
        cordx = abs(m->bases[i].local.x - mi->local_missao.x);
        cordy = abs(m->bases[i].local.y - mi->local_missao.y);

        dist = sqrt(cordx*cordx + cordy*cordy);

        /*Guardar no vetor*/
        dist_bm[i].id_base = i;
        dist_bm[i].dist = dist;
    } 

    /*Ordenar o vetor em relacao a distancia*/
    selectSort(dist_bm, m->num_bases -1);

    /*Inicializacao*/
    bmp = -1; 

    /*Incrementa o n de tentativas da missao, tentativas começam em 0*/    
    mi->tentativa++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, mi->id, mi->tentativa);
    cjto_imprime(mi->hab_requerida);
    printf(" ]\n");

    /*Testar se as hab dos herois da base correspondem ao exigido pela missao*/
    for(int i = 0; i < m->num_bases; i++)
    {
        hab_base = cjto_cria(N_HABILIDADES); /*conjunto vazio*/

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", 
            tempo, 
            mi->id,
            m->bases[dist_bm[i].id_base].id,
            dist_bm[i].dist);
        cjto_imprime(m->bases[dist_bm[i].id_base].heroi_pres);
        printf(" ]\n");

        /*Unir todos os conjuntos de habilidades dos herois*/
        for(int j=0; j<m->num_herois; j++)
        {
            /*Se heroi pertence a base*/
            if (cjto_pertence(m->bases[dist_bm[i].id_base].heroi_pres, m->herois[j].id) == 1)
            {
                /*
                *Usar um conjunto auxiliar para garantir que toda a memoria alocada pelo cjto_uniao a cada iteracao
                *seja liberado
                */

                printf("%6d: MISSAO %d HAB HEROI %2d: [ ",
                     tempo, 
                     mi->id, 
                     m->herois[j].id);
                cjto_imprime(m->herois[j].hab);
                printf(" ]\n");

                hab_base_aux = cjto_uniao(hab_base, m->herois[j].hab); 
                cjto_destroi(hab_base);
                hab_base = hab_base_aux;
            }
        }


        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", 
            tempo,
            mi->id, 
            dist_bm[i].id_base);
        cjto_imprime(hab_base);
        printf(" ]\n");


        /*Verificar se base contem o conjunto requerido*/
        contem = cjto_contem(hab_base, mi->hab_requerida);

        /*Se contem e eh a base com a menor distancia*/
        if(contem)
        {
            /*Como o vetor com as distancias está ordenado o primeiro que atender o requerido será o mais próximo*/
            bmp = dist_bm[i].id_base;
            
            /*Copiar o hab_base, imprimir e liberar*/
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", 
                tempo, 
                mi->id, 
                bmp);
            cjto_imprime(hab_base);
            printf(" ]\n");

            /*Liberar memoria*/
            cjto_destroi(hab_base);
            break;
        }

        /*Liberar memoria*/
        cjto_destroi(hab_base);
    }
   
    if(bmp != -1)
    {
        /*Estatistica: base bmp participou dessa missao*/
        m->bases[bmp].part_missao++;

        /*Missao cumprida*/
        mi->cumprida = 1;

        /*Estatisticas*/
        m->qtde_missoes_cumpridas++;

        /*Verificar se cada heroi morre ou não*/
        for(int i = 0; i < m->num_herois; i++)
        {
            if(cjto_pertence(m->bases[bmp].heroi_pres, m->herois[i].id) == 1)
            {
                risco = mi->perigo / (m->herois[i].pac + m->herois[i].xp +1);
                risco_rand = aleat(0, 30);

                if(risco > risco_rand)
                {
                    /*Insere evento MORRE na LEF*/
                    ev = cria_info_evento(m->herois[i].id, m->bases[bmp].id, mi->id);
                    fprio_insere(m->lef, ev, MORRE, tempo);
                }
                else
                {
                    /*Incrementa o xp*/
                    m->herois[i].xp++;
                }
            }
        }

    }
    else
    {
        printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, mi->id);

        /*Insere evento MISSAO na LEF*/
        amanha = tempo + AMANHA; 
        ev = cria_info_evento(0, 0, mi->id);
        fprio_insere(m->lef, ev, MISSAO, amanha);
    }
    free(dist_bm);
    dist_bm = NULL;
}

void evento_fim(struct mundo_t m, int tempo)
{
    float pcto_missoes, taxa_mort, media_missao;
    int min_tent_missao, max_tent_missao, soma_missao, mortos;
    mortos = 0;

    printf("%6d: FIM\n", tempo);

    /*Estatísticas*/
    /*Para cada heroi*/
    for(int i = 0; i < m.num_herois; i++)
    {
        if(m.herois[i].vivo)
            printf("HEROI %2d VIVO ", m.herois[i].id);
        else
        {
            printf("HEROI %2d MORTO", m.herois[i].id);
            mortos++;
        }        
        printf("  PAC %3d VEL %4d EXP %4d HABS [ ",
            m.herois[i].pac,
            m.herois[i].vel,
            m.herois[i].xp
            );
        cjto_imprime(m.herois[i].hab);
        printf(" ]\n");
    }

    /*Para cada base*/
    for(int i = 0; i < m.num_bases; i++)
    {
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n",
            m.bases[i].id,
            m.bases[i].lotacao,
            m.bases[i].max_fila,
            m.bases[i].part_missao);
    }

    /*Estatisticas gerais*/
    printf("EVENTOS TRATADOS: %d\n", m.eventos_tratados);

    pcto_missoes = ((float)m.qtde_missoes_cumpridas/(float)m.num_missoes)*100;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n",
        m.qtde_missoes_cumpridas,
        m.num_missoes,
        pcto_missoes);

    /*Inicializar com os dados da primeira missao*/
    min_tent_missao = m.missoes[0].tentativa;
    max_tent_missao = m.missoes[0].tentativa; 
    soma_missao = 0;
    for(int i = 1; i < m.num_missoes -1; i++)
    {
        if(m.missoes[i].tentativa < min_tent_missao)
            min_tent_missao = m.missoes[i].tentativa;
    
        if(m.missoes[i].tentativa > max_tent_missao)
            max_tent_missao = m.missoes[i].tentativa;
        
        soma_missao += m.missoes[i].tentativa;
    }
    media_missao = (float)soma_missao / (float)m.num_missoes;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", 
        min_tent_missao, 
        max_tent_missao, 
        media_missao);

    taxa_mort = ((float)mortos/(float)m.num_herois)*100;   
    printf("TAXA MORTALIDADE: %.1f%%\n",taxa_mort);
}

void eventos_iniciais(struct mundo_t m)
{
    int base, tempo_heroi, tempo_missao;
    struct evento_t *ev;

    /*Adicionar fim do mundo na LEF*/
    ev = cria_info_evento(0, 0, 0);
    fprio_insere(m.lef, ev, FIM, m.fim_do_mundo);

    /*Heroi chegar em alguma base aleatoria nos tres primeiros dias*/
    for(int i = 0; i < m.num_herois; i ++)
    {
        /*Limites dos valores aleatorios conforme enunciado*/
        base = aleat(0, m.num_bases -1);
        tempo_heroi = aleat(0, TRES_DIAS);

        /*Inserir evento CHEGA na LEF*/
        ev = cria_info_evento(m.herois[i].id, m.bases[base].id, 0);        
        fprio_insere(m.lef, ev, CHEGA, tempo_heroi);
    }

    /*Agendar missões*/
    for(int i = 0; i < m.num_missoes; i++)
    {
        tempo_missao = aleat(0, m.fim_do_mundo);
        
        /*Inserir evento MISSAO na LEF */
        ev = cria_info_evento(0, 0, m.missoes[i].id);
        fprio_insere(m.lef, ev, MISSAO, tempo_missao);
    }
}

void simulacao(struct mundo_t m)
{
    int fim_sim = 0;
    int tipo, tempo;
    struct evento_t *ev;

    while(!fim_sim)
    {
        /*Extrair os dados principais do evento*/
        ev = fprio_retira(m.lef, &tipo, &tempo);

        /*Atualizar o relogio*/
        m.relogio = tempo;

        /*Verificar se heroi está vivo*/
        int estado;
        estado = m.herois[ev->id_heroi].vivo;

        switch (tipo)
        {
            case CHEGA:
            {
                if(estado)
                {
                    /*O evento so ocorre se o heroi estiver vivo, logo conta como um evento tratado*/
                    m.eventos_tratados++;
                    evento_chega(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                /*Destruicao dos dados do evento ocorre independente do heroi estar vivo ou morto*/
                destroi_info_evento(ev);
                break;
            }
            case ESPERA:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_espera(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                destroi_info_evento(ev);
                break;
            }
            case DESISTE:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_desiste(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                destroi_info_evento(ev);
                break;
            }
            case AVISA:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_avisa(m, m.relogio, &m.bases[ev->id_base]);  
                }
                destroi_info_evento(ev);
                break;
            }
            case ENTRA:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_entra(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                destroi_info_evento(ev);
                break;
            }
            case SAI:
            {
                if (estado)
                {
                    m.eventos_tratados++;
                    evento_sai(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                destroi_info_evento(ev);
                break;
            }
            case VIAJA:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_viaja(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base]);
                }
                destroi_info_evento(ev);
                break;
            }
            case MISSAO:
            {
                if(estado)
                {
                    m.eventos_tratados++;
                    evento_missao(&m,m.relogio, &m.missoes[ev->id_missao]); 
                }
                destroi_info_evento(ev);
                break;
            }
            case MORRE:
            {
                m.eventos_tratados++;
                evento_morre(m, m.relogio, &m.herois[ev->id_heroi], &m.bases[ev->id_base], &m.missoes[ev->id_missao]);
                destroi_info_evento(ev);
                break;
            }
            case FIM:
            {
                evento_fim(m, m.relogio);
                destroi_info_evento(ev);
                /*
                *Sair do loop, os eventos que poderiam ter passado do tempo maximo
                *serao destruidos juntos com a destruicao da lef
                */
                fim_sim = 1;
                break;
            }
            default:
            {
                printf("Evento desconhecido\n");
                break;
            }
        }   
    }
}


