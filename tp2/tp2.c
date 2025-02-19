/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
*/

#include <stdio.h>
#include "racional.h"
#define VET_TAM 100

void ler_vetor(struct racional r[], int n)
{
   int num, den;
   for (int i=0; i<n; i++)
   {
      scanf("%d %d", &num, &den);
      r[i].num = num;
      r[i].den = den;
   }
}

void imprime_vetor(struct racional r[], int n)
{
   printf("VETOR = ");

   for (int i=0; i<n; i++)
   {
      imprime_r(r[i]);
   }
   printf("\n");
}

void elimina_invalidos(struct racional r[], int *n)
{
   struct racional aux;
   for (int i=0; i<=*n-1; i++)
   {   
      if(!valido_r(r[i]))/*se r[i] for inválido, trocar com um válido do fim do vetor*/
      {
         for(int j=*n-1; j>i; j--)
         {
            if(valido_r(r[j]))
            {
               aux = r[i];
               r[i] = r[j];
               r[j] = aux;
               break;  
            }
         }
      } 
   }

   for(int k=0; k<*n;k++)
   {
      if(!valido_r(r[k])) /*diminuir o tamanho do n, apagando os NaN*/
      {
         *n = k;
      }
   }
}

void insertion_sort(struct racional r[], int n)
{
   for(int i=1; i<n; ++i)
   {
      struct racional chave = r[i];
      int j = i-1;

      while ( j>=0 && compara_r(r[j], chave) == 1) /*r[j] > chave*/
      {
         r[j+1] = r[j];
         j = j-1;
      }
      r[j+1] = chave;
   }
}

int main ()
{
   struct racional r[VET_TAM];
   struct racional somatorio;

   int n;
   scanf("%d", &n);
  
   /*caso n esteja fora dos limites, interromper execucao*/
   if(n<=0 || n>=100)
      return 1;

   ler_vetor(r, n);

   /*imprimir todos os racionais armazenados*/
   imprime_vetor(r, n);

   /*remover invalidos e imprimir*/
   elimina_invalidos(r, &n);
   imprime_vetor(r, n);

   /*ordenar e imprimir vetor*/
   insertion_sort(r, n);
   imprime_vetor(r, n);

   /*calcular soma de todos os elementos e imprimir o resultado*/
   if (!valido_r(*r)) /*vetor vazio, inválido*/
   {
      printf("SOMA = 0\n");
      return 1;
   }
   
   somatorio = r[0];
   for (int i=1; i<n; i++)
   {
      soma_r(somatorio, r[i], &somatorio);
   }
   printf("SOMA = ");
   imprime_r(somatorio);
   printf("\n");

   return (0);
}
