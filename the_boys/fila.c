#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "lista.h"

struct lista_t *fila_cria ()
{
    return lista_cria();
}

struct lista_t *fila_destroi (struct lista_t *fila)
{
    return lista_destroi(fila);
}

int enqueue (struct lista_t *fila, int item)
{
    int sucesso;
    /*pos = -1, pois sempre irá inserir no final*/
    sucesso = lista_insere(fila, item, -1);

    if(sucesso == -1)
        return sucesso;
    
    return 1;
}

int dequeue (struct lista_t *fila, int *item)
{
    int sucesso;
    /*pos = 0, pois sempre irá tirar no início*/
    sucesso = lista_retira(fila, item, 0);
    
    if(sucesso == -1)
        return sucesso;
    
    return 1;
}

int fila_tamanho (struct lista_t *fila)
{
    return lista_tamanho(fila);
}

void fila_imprime (struct lista_t *fila)
{
    lista_imprime(fila);
}