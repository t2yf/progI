// programa principal do projeto "The Boys - 2024/2"
// Autor: Thamiris Yamate Fischer

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"
#include "fprio.h"
#include "mundo.h"
#include "eventos.h"

int main()
{
    srand(0);

    struct mundo_t mundo;

    /*Criar mundo*/
    mundo = cria_mundo();
    inicializa_mundo(&mundo);

    eventos_iniciais(mundo);
    simulacao(mundo);

    destroi_entidades(mundo);

    return 0;
}
