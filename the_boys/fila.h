#ifndef FILA
#define FILA

#include "lista.h"

/*
* Esta implementacao de fila eh uma especificacao da implementacao da lista generica
*/

/*
* Cria uma fila vazia.
* Retorno: ponteiro p/ a fila ou NULL em erro.
*/
struct lista_t *fila_cria ();

/*
* Remove todos os itens da fila e libera a memória.
*/
struct lista_t *fila_destroi (struct lista_t *fila);

/*
* Insere o item na fila respeitando a política FIFO;
* Retorno: 1 em caso de sucesso, -1 caso contrário
*/
int enqueue (struct lista_t *fila, int item);

/*
* Retira o item da fila respeitando a política FIFO.
* Retorno: 1 em caso de sucesso, -1 caso contrário
*/
int dequeue (struct lista_t *fila, int *item);

/*
* Informa o tamanho da fila(o número de itens presentes nela).
* Retorno: número de itens na fila ou -1 em erro.
*/
int fila_tamanho (struct lista_t *fila);
 
/*
* Imprime o conteúdo da fila do inicio ao fim no formato "item item ...",
* com um espaço entre itens, sem espaços antes/depois, sem newline.
*/
void fila_imprime (struct lista_t *fila);

#endif 