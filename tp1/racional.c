/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */

#include <stdio.h>
#include <stdlib.h>

/*
 * Implemente aqui as funcoes definidas no racionais.h; caso precise,
 * pode definir aqui funcoes auxiliares adicionais, que devem ser usadas
 * somente neste arquivo.
*/

/*Abaixo esta implementado todas as funcoes necessarias para criar e imprimir um racional valido*/

struct racional {
  long num;          /* numerador   */
  long den;          /* denominador */
};


/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max)
{
    long num_aleat;
    num_aleat = (rand()%(2*max))+min; /*Gera um numero pseudo aleatorio no intervalo 
	                                   *-max==min a max */

    return num_aleat;
}

struct racional cria_r(long numerador, long denominador)
{
	struct racional r;
	r.num = numerador;
	r.den = denominador;
	
	return r;
}

struct racional sorteia_r (long min, long max)
{
 	long num, den;
	num = aleat(min, max);
	den = aleat(min, max);

	return cria_r(num, den); /*"Une" num com den e devolve para quem chamou*/
}


/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
long mdc (long a, long b)
{
	long resp;
    while(a!= 0 && b!= 0){
		if (a>b)
			a=a%b;
		else
			b=b%a;
	}
	resp = a+b;/*a ou b será 0, entao a soma sera o MDC*/

    return resp;  
}

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  return (a*b)/mdc(a,b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r)
{
	/*r invalido*/
	if (r.den == 0)
        return r;

	/*Inverter sinais. Caso den seja negativo, jogar o sinal negativo para o num
	 * mudando o sinal de ambos. Caso ambos sejam negativos, mudar o sinal de ambos */
	if ((r.den <0) || (r.num < 0 && r.den <0))
	{
		r.num *=-1;
		r.den *=-1;
	}

	/*Para simplificar, dividir num e den pelo mdc*/
	int divisor;
	divisor = mdc(labs(r.num), labs(r.den)); /*Trabalhar apenas com numeros positivos
											  *para achar o mdc*/
	r.num = r.num/divisor;
	r.den = r.den/divisor;

	return r;		
}

int valido_r (struct racional r)
{
	if (r.den == 0)
		return 0; /*Racional invalido*/

	return 1;
}


void imprime_r (struct racional r)
{
    struct racional r_aux;
	r_aux = simplifica_r(r); /*r_aux recebe as alteracoes simplificadas em r para as*/ 
	                        /* comparacoes abaixo*/
	
	if (!valido_r(r_aux))
		printf("INVALIDO ");

	else if (r_aux.num == 0)
		printf("0 ");

	else if (r_aux.den == 1)
		printf("%ld ",r_aux.num);

	else if (r_aux.den == r_aux.num)
		printf("1 ");

	else{
		printf("%ld/%ld ", r_aux.num, r_aux.den);
	} 
}

/*Abaixo está implementado todas as operacoes com os racionais previstas em racional.h*/

struct racional soma_r (struct racional r1, struct racional r2)
{
	/*Se r1 invalido, retornar r1 ou se r2 invalido, retornar r2*/
	if (!valido_r(r1))
		return r1;

	if (!valido_r(r2))
		return r2;

	struct racional r_soma;

    r_soma.num= (r1.num*r2.den) + (r2.num*r1.den);
    r_soma.den= r1.den*r2.den;

    return r_soma;
}

struct racional subtrai_r (struct racional r1, struct racional r2)
{
    /*Se r1 invalido, retornar r1 ou se r2 invalido, retornar r2*/
	if (!valido_r(r1))
		return r1;

	if (!valido_r(r2))
		return r2;
 
	struct racional r_subtrai;

    r_subtrai.num = (r1.num*r2.den) - (r2.num*r1.den);
    r_subtrai.den = r1.den*r2.den;

    return r_subtrai;
}

struct racional multiplica_r (struct racional r1, struct racional r2)
{
    /*Se r1 invalido, retornar r1 ou se r2 invalido, retornar r2*/
	if (!valido_r(r1))
		return r1;

	if (!valido_r(r2))
		return r2;
	
	struct racional r_multi;

    r_multi.num = r1.num*r2.num;
    r_multi.den = r1.den*r2.den;

    return r_multi;
}

struct racional divide_r (struct racional r1, struct racional r2)
{	
	/*Se r1 invalido, retornar r1 ou se r2 invalido, retornar r2*/
	if (!valido_r(r1))
		return r1;

	if (!valido_r(r2))
		return r2;
	
	/*racional invalido 0/0*/
	struct racional r_invalido;
	r_invalido = cria_r(0,0);

	struct racional r_div;
    r_div.num = r1.num*r2.den;
    r_div.den = r1.den*r2.num;

	/*Se r_div inválido, retornar r_invalido*/
	if (!valido_r(r_div))
		return r_invalido;

    return r_div;
}
