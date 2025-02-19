#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


struct lista_t *lista_cria ()
{
    struct lista_t *lst;

    if(!(lst = malloc(sizeof(struct lista_t)))){
        return NULL;
    }

    /*Inicializar campos*/
    lst->prim = NULL;
    lst->ult = NULL;
    lst->tamanho = 0;

    return lst;
}

struct lista_t *lista_destroi (struct lista_t *lst)
{
    struct item_t *aux, *remov;
    aux = lst->prim;

    /*Lista vazia*/
    if(lst->prim == NULL)
    {
        free(lst);
        return NULL;
    }

    while(aux != NULL)
    {
        remov = aux;
        aux = remov->prox;
        free(remov);
    }
    free(lst);
    
    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
    struct item_t *novo, *aux, *auxAnt;

    /*Lista não alocada*/
    if(lst == NULL)
        return -1;

    if(!(novo = malloc(sizeof(struct item_t))))
        return -1;

    novo->valor = item;
    lst->tamanho++; 

    /*Lista inicialmente vazia*/
    if(lst->prim == NULL)
    {
        /*É o primeiro e único elemento da lista*/
        novo->ant = NULL;
        novo->prox = NULL;
        lst->prim = novo;
        lst->ult = novo;
        
        return lst->tamanho;
    }
       
    /*Inserir na primeira posição*/
    if(pos == 0)
    {
        novo->prox = lst->prim;
        novo->ant = NULL;
        lst->prim->ant = novo;
        lst->prim = novo; 
       
        return lst->tamanho;
    }

    /*Inserir na última posição (pos > tam-1 || pos == -1)*/
    if(pos >= lst->tamanho || pos == -1)
    {
        novo->prox = NULL;
        aux = lst->ult;
        aux->prox = novo;
        novo->ant = aux;
        lst->ult = novo;

        return lst->tamanho;
    }

    /*Inserir na posição*/

    /*Chegar na posição*/
    int i = 0;
    aux = lst->prim;
    while(i != pos)
    {
        i++;
        auxAnt = aux;
        aux = aux->prox;
    }

    /*Novo toma o lugar de aux*/
    novo->prox = aux;
    aux->ant = novo;
    novo->ant = auxAnt;
    auxAnt->prox = novo;    

    return lst->tamanho;
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux, *auxAnt;

    /*Lista não alocada ou item nulo*/
    if(lst == NULL || item == NULL)
        return -1;

    /*Erro, acessando posição inválida*/
    if(pos >= lst->tamanho)
        return -1;
    
    /*Tentar tirar de lista vazia*/
    if(lst->prim == NULL)
        return -1;

    lst->tamanho--;

    /*Retirar o único elemento de uma lista*/
    if(lst->prim->prox == NULL)
    {
        *item = lst->prim->valor;

        aux = lst->prim;
        free(aux);

        lst->prim = NULL;
        lst->ult = NULL;

        return lst->tamanho;
    }

    /*Retirar primeiro elemento*/
    if(pos == 0)
    {
        *item = lst->prim->valor;

        aux = lst->prim->prox;
        free(lst->prim);

        aux->ant = NULL;
        lst->prim = aux;

        return lst->tamanho;
    }

    /*Retirar último elemento ou inexistente (pos >= tam-1 || pos == -1)*/
    if(pos == lst->tamanho || pos ==-1)
    {
        struct item_t *aux;

        *item = lst->ult->valor;

        aux = lst->ult->ant;
        free(lst->ult);

        aux->prox = NULL;
        lst->ult = aux;

        return lst->tamanho;
    }

    /*Retirar elemento indicado*/

    /*Chegar na posição*/
    int i = 0;
    aux = lst->prim;

    while(i != pos)
    {
        i++;
        auxAnt = aux;
        aux = aux->prox;
    }

    /*Ajeitar os ponteiros do anterior e do sucessor do que eu quero tirar*/
    *item = aux->valor;

    auxAnt->prox = aux->prox;
    aux->prox->ant = auxAnt;
    free(aux);

    return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;

    /*Lista não alocada*/
    if(lst == NULL)
        return -1;

    /*Erro, acessando posição inválida*/
    if(pos >= lst->tamanho)
        return -1;

    /*Acessar item da última posição*/
    if(pos == lst->tamanho || pos == -1)
    {
        *item = lst->ult->valor;
        return lst->tamanho;
    }

    /*Chegar na posição*/
    int i = 0;
    aux = lst->prim;
    
    while(i != pos)
    {
        i++;
        aux = aux->prox;
    }

    *item = aux->valor;

    return lst->tamanho;

}

int lista_procura (struct lista_t *lst, int valor)
{
    struct item_t *aux;

    /*Lista não alocada*/
    if(lst == NULL)
        return -1;

    aux = lst->prim;

    for(int i=0; i<lst->tamanho; i++)
    {
        if(valor == aux->valor)
            return i;
        aux = aux->prox;
    }

    return -1;
}

int lista_tamanho (struct lista_t *lst)
{
    /*Lista não alocada*/
    if(lst == NULL)
        return -1;

    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
    struct item_t *atual;
    atual = lst->prim;

    /*Lista vazia*/
    if (atual == NULL)
        return;

    for(int i=0; i< lst->tamanho -1; i++)
    {
        printf("%d ", atual->valor);
        atual = atual->prox;
    }

    printf("%d", atual->valor);
}