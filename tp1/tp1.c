/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */

#include <stdio.h>
#include <stdlib.h> /*permite incluir srand() e rand()*/
#include "racional.h" 

/* programa principal */
int main ()
{
    srand (0); /* inicializar semente randomica */

    int n, max, min;
    scanf("%d %d", &n, &max);
    
    /*Testar entrada n*/
    if (n <= 0 || n >= 100)
    {
        return 1;
    }
    /*Testar entrada max*/
    if (max <= 0 || max >= 30)
    {
        return 1;
    }

    min = -max; /*o intervalo para sortear um numero eh de -max a max */
    
    for (int i=1; i<=n; i++){
        struct racional r1, r2, r_soma, r_subtrai, r_multi, r_div;

        r1 = sorteia_r(min, max);
        r2 = sorteia_r(min, max);
        
        /*Imprimir r1 e r2*/
        printf("%d: ", i);
        imprime_r(r1);
        imprime_r(r2);

        /*Se r1 ou r2 forem INVALIDOS, interromper execucao*/
        if (!valido_r(r1) || !valido_r(r2))
        {
            printf("NUMERO INVALIDO \n");
            return 1;
        }
        
        /*Calcular operacoes*/
        r_soma = soma_r(r1, r2);
        r_subtrai = subtrai_r(r1, r2);
        r_multi = multiplica_r(r1, r2);
        r_div = divide_r(r1, r2);

        /*Como divisao por 0 eh invalido, se r2 for igual a 0, interromper execucao*/
        if (r2.num == 0)
        {
            printf("DIVISAO INVALIDA \n");
            return 1;
        }
            
        /*Imprimir operacoes*/
        imprime_r(r_soma);
        imprime_r(r_subtrai);
        imprime_r(r_multi);
        imprime_r(r_div);
        printf("\n");
    }

    return (0);
}
