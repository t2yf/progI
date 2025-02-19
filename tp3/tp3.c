/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

void ler_vetor(struct racional **vet, int n)
{
   int num, den;
   for (int i=0; i<n; i++)
   {
      scanf("%d %d", &num, &den);
      vet[i] = cria_r(num, den);
   }
}

void imprime_vetor(struct racional **vet, int n)
{
   printf("VETOR = ");

   for (int i=0; i<n; i++)
   {
      imprime_r(vet[i]);
   }
   printf("\n");
}

void elimina_invalidos(struct racional **vet, int *n)
{
   struct racional *aux;
   for (int i=0; i<=*n-1; i++)
   {   
      if(!valido_r(vet[i]))/*se r[i] for inválido, trocar com um válido do fim do vetor*/
      {
         for(int j=*n-1; j>i; j--)
         {
            if(valido_r(vet[j]))
            {
               aux = vet[i];
               vet[i] = vet[j];
               vet[j] = aux;
               break;  
            }
         }
      } 
   }

   for(int k=*n-1; k>=0;k--)
   {
      if(!valido_r(vet[k])) /*se racional for inválido, liberar ponteiro e diminuir vetor*/
      {
         destroi_r(vet[k]);
         vet[k] = NULL;
         *n = k;
      }
   }
}

void insertion_sort(struct racional **vet, int n)
{
   for(int i=1; i<n; ++i)
   {
      struct racional *chave = vet[i];
      int j = i-1;

      while ( j>=0 && compara_r(vet[j], chave) == 1) /*r[j] > chave*/
      {
         vet[j+1] = vet[j];
         j = j-1;
      }
      vet[j+1] = chave;
   }
}

void liberar_racionais(struct racional **vet, int n)
{
   for (int i=0; i<n; i++)
   {
      destroi_r(vet[i]);
      vet[i] = NULL;
   }  
}

struct racional *somatorio(struct racional **vet, int n)
{
   struct racional *soma_vetor;
   soma_vetor = cria_r(0,1); /*receber inicialmente um valor inválido*/
   
   
   if(!valido_r(vet[0])) /*caso o primeiro elemento não exista, não há vetor a ser somado*/
   {
      return soma_vetor; /*retornar valor inválido*/
   }

   for(int i=0; i<n; i++)
      soma_r(soma_vetor, vet[i], soma_vetor);
   
   return soma_vetor;
}

void imprime_somatorio(struct racional *soma_vetor)
{
   printf("SOMA = ");
   imprime_r(soma_vetor);
   printf("\n");
   destroi_r(soma_vetor);
}


int main ()
{
   int n;
   struct racional **prt_vet;
   struct racional *soma_vetor;
   scanf("%d", &n);
   
   /*n invalido*/
   if(n<=0 || n>=100)
      return 0;
   
   /*alocar dinamicamente vetor de ponteiros para racionais*/
   prt_vet = malloc(n*sizeof(struct racional *));
   
   /*caso não tenha alocado*/
   if(prt_vet == NULL)
      return 0; 
    
   ler_vetor(prt_vet, n);
   imprime_vetor(prt_vet, n);

   elimina_invalidos(prt_vet, &n);
   imprime_vetor(prt_vet, n);

   /*ordenar*/
   insertion_sort(prt_vet, n);
   imprime_vetor(prt_vet, n);

   /*calcular a soma de todos os elementos do vetor*/
   soma_vetor= somatorio(prt_vet, n);
   imprime_somatorio(soma_vetor);

   /*liberar racionais do vetor de ponteiros*/
   liberar_racionais(prt_vet, n);
   imprime_vetor(prt_vet, n);

   /*liberar vetor de ponteiros*/
   free(prt_vet);
   prt_vet = NULL;

   return (0) ;
}

