/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b*/
/* pelo Metodo de Euclides */
long mdc (long a, long b)
{
   long resp;
   while(a!= 0 && b!= 0)
   {
      if (a>b)
         a=a%b;
      else
         b=b%a;
   }
   resp = a+b;/*a ou b será 0, entao a soma sera o MDC*/

   return resp;  
}

/* Minimo Multiplo Comum entre a e b */
long mmc (long a, long b)
{
   return (a*b)/mdc(a,b);
}

int valido_r (struct racional *r)
{
   /*ponteiro nulo ou divisao por 0*/
   if(r == NULL || r->den == 0) 
      return 0;

   return 1;
}

int simplifica_r (struct racional *r)
{
   /*r invalido*/
   if (!valido_r(r))
      return 0;

   /*ponteiro nulo*/
   if(r == NULL)
      return 0;

   /*Inverter sinais. Caso den seja negativo, jogar o sinal negativo para o num
   * mudando o sinal de ambos. Caso ambos sejam negativos, mudar o sinal de ambos */
   if ((r->den < 0) || (r->num < 0 && r->den < 0))
   {
      r->num *=-1;
      r->den *=-1;
   }

   /*Para simplificar, dividir num e den pelo mdc*/
   int divisor;
   divisor = mdc(labs(r->num), labs(r->den)); /*Trabalhar apenas com numeros positivos
                                              *para achar o mdc*/
   r->num = r->num/divisor;
   r->den = r->den/divisor;

   return 1;	/*sucesso!*/
}


struct racional *cria_r (long numerador, long denominador)
{
   struct racional *prt_r;

   /*caso a alocação não tenha sido feita*/
   if(! (prt_r = malloc(sizeof(struct racional))) ) 
      return NULL; 

   prt_r->num = numerador;
   prt_r->den = denominador;

   simplifica_r(prt_r);

   return prt_r;
}

void destroi_r (struct racional *r)
{
   free(r);
}

void imprime_r (struct racional *r)
{
   /*ponteiro nulo*/
   if(r == NULL)
   {
      printf("NULL ");
      return;
   }

   /*racional invalido*/
   if(!valido_r(r))
   {
      printf("NaN ");
      return;
   }
   
   if(r->num == 0)
   {
      printf("0 ");
      return;
   }

   if(r->num == r->den)
   {
      printf("1 ");
      return;
   }

   /*simplificar o racional*/
   simplifica_r(r);

   if(r->den == 1)
   {
      printf("%ld ", r->num);
      return;
   }

   printf("%ld/%ld ", r->num, r->den);
}

int compara_r (struct racional *r1, struct racional *r2)
{
   /*ponteiros nulos*/
   if(r1 == NULL || r2 == NULL)
      return -2;

   /*racionais inválidos*/
   if(!valido_r(r1) || !valido_r(r2))
      return -2;

   /*comparacao igualando denominadores pelo mmc*/
   int def_mmc, rac1, rac2;
   def_mmc = mmc(r1->den, r2->den);

   rac1 = (def_mmc / r1->den)*r1->num;
   rac2 = (def_mmc / r2->den)*r2->num;

   /*r1>r2*/
   if(rac1 > rac2)
      return 1;

   /*r1=r2*/
   if(rac1 == rac2)
      return 0;

   /*r1<r2*/  
   return -1; 
}

int soma_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
   /*ponteiro nulo*/
   if(r1 == NULL || r2 == NULL)
      return 0;

   /*r1 ou r2 invalido*/
   if(!valido_r(r1) || !valido_r(r2))
      return 0;

   /*operacao de soma*/
   r3->num = (r1->num*r2->den) + (r2->num*r1->den);
   r3->den = r1->den*r2->den;

   /* r3 nulo*/
   if(r3 == NULL)
      return 0;

   simplifica_r(r3);
   return 1;
}

int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
   /*ponteiro nulo*/
   if(r1 == NULL || r2 == NULL)
      return 0;

   /*r1 ou r2 invalido*/
   if(!valido_r(r1) || !valido_r(r2))
      return 0;
   
   /*operacao de subtracao*/
   r3->num = (r1->num*r2->den) - (r2->num*r1->den);
   r3->den = r1->den*r2->den;

   /* r3 nulo*/
   if(r3 == NULL)
      return 0;

   simplifica_r(r3);
   return 1;
}

int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
   /*ponteiro nulo*/
   if(r1 == NULL || r2 == NULL)
      return 0;

   /*r1 ou r2 invalido*/
   if(!valido_r(r1) || !valido_r(r2))
      return 0;
   
   /*operacao de multiplicacao*/
   r3->num = r1->num*r2->num;
   r3->den = r1->den*r2->den;

   /* r3 nulo*/
   if(r3 == NULL)
      return 0;

   simplifica_r(r3);
   return 1;
}

int divide_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
   /*ponteiro nulo*/
   if(r1 == NULL || r2 == NULL)
      return 0;

   /*r1 ou r2 invalido*/
   if(!valido_r(r1) || !valido_r(r2))
      return 0;

   /*operacao de divisao*/
   r3->num = r1->num*r2->den;
   r3->den = r1->den*r2->num;

   /* r3 nulo*/
   if(r3 == NULL)
      return 0;

   simplifica_r(r3);
   return 1;
}
