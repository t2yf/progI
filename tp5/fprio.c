// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"


struct fprio_t *fprio_cria ()
{
    struct fprio_t *f;

    if(!(f = malloc(sizeof(struct fprio_t))))
        return NULL;

    f->prim = NULL;
    f->num = 0;

    return f;
}

struct fprio_t *fprio_destroi (struct fprio_t *f)
{
    struct fpnodo_t *aux, *remov;
    aux = f->prim;

    /*Destruir fila vazia*/
    if(f == NULL)
    {
        free(f);
        return NULL;
    }

    while(aux != NULL)
    {
        remov = aux;
        aux = remov->prox;
        free(remov->item);
        free(remov);
    }
    free(f);

    return NULL;
}


int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
    struct fpnodo_t *novo, *aux, *auxAnt;

    /*Erro: Fila não alocada*/
    if(f == NULL)
        return -1;

    /*Erro: Inserir item nulo*/
    if(item == NULL)
        return -1;

    /*Erro: Não foi feita a alocação*/
    if(!(novo = malloc(sizeof(struct fpnodo_t))))
        return -1;

    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;
    
    f->num++;

    /*Fila inicialmente vazia*/
    if(f->prim == NULL)
    {
        f->prim = novo;
        novo->prox = NULL;

        return f->num;
    }

    /*Inserir ordenado por prioridade*/
    aux = f->prim;

    /*Inserir no início da fila*/
    /*Exemplo: Se aux->prio = 2 e novo->prio = 1*/
    if(aux->prio > novo->prio)
    {
        /*Inserir mesmo item*/
        if(aux->item == novo->item)
        {
            free(novo);
            return -1;
        }
        novo->prox = aux;
        f->prim = novo;

        return f->num;  
    }

    while(aux != NULL)
    {
        /*Inserir mesmo item*/
        if(aux->item == novo->item)
        {
            free(novo);
            return -1;
        }
        
        if(aux->prio > novo->prio)
        {
            novo->prox = aux;
            auxAnt->prox = novo;
            return f->num;
        }
        
        auxAnt = aux;
        aux = aux->prox;
    }

    /*Ultima prioridade, ou seja, inserir no final*/
    auxAnt->prox = novo;
    novo->prox = NULL;

    return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    struct fpnodo_t *aux;
    void *prt_item;

    /*Erro: retirar de fila vazia*/
    if(f == NULL)
    {
        tipo = NULL;
        prio = NULL;
        return NULL;
    }

    /*Erro: tipo ou prio serem nulos*/
    if(tipo == NULL || prio == NULL)
    {
        return NULL;
    }
    
    aux = f->prim;
    *tipo = aux->tipo;
    *prio = aux->prio;
    prt_item = aux->item;

    f->prim = aux->prox;
    free(aux);
    f->num--;

    return prt_item;
    
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f)
{
    /*Erro: Fila vazia*/
    if(f == NULL)
        return -1;
    
    return f->num;
}

void fprio_imprime (struct fprio_t *f)
{
    struct fpnodo_t *atual;
    atual = f->prim;

    /*Lista vazia*/
    if (atual == NULL)
        return;

    for(int i=0; i< f->num -1; i++)
    {
        printf("(%d %d) ", atual->tipo, atual->prio);
        atual = atual->prox;
    }

    printf("(%d %d)", atual->tipo, atual->prio);
}
