/** @file
   Interfejs klasy operacji na kalkulatorze wielomianów
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/
#ifndef __OPERATION_H__
#define __OPERATION_H__

#include <stdio.h>
#include <limits.h>
#include "poly.h"
#include "polystack.h"
#include "number.h"
#include "error.h"

#define AT "AT"
#define DEG_BY "DEG_BY"
#define ZERO "ZERO"
#define IS_COEFF "IS_COEFF"
#define IS_ZERO "IS_ZERO"
#define CLONE "CLONE"
#define ADD "ADD"
#define MUL "MUL"
#define NEG "NEG"
#define SUB "SUB"
#define IS_EQ "IS_EQ"
#define DEG "DEG"
#define DEG_BY "DEG_BY"
#define AT "AT"
#define PRINT "PRINT"
#define POP "POP"
#define COMPOSE "COMPOSE"

#define OPER_WITHOUT_ARG_AMOUNT 12
#define OPER_WITH_ARG_AMOUNT 3

/**
 * Struktura przechowująca polecenie kalkulatora, które nie wymaga żadnego argumentu
 */
typedef struct Operation
{
	char* name; ///< nazwa polecenia
	long requiredStackSize; ///< wymagany przez polecenie rozmiar stosu
	void (*execute)(PolyStack *); ///< funkcja będąca wykonaniem danego polecenia
} Operation;
/**
 * Struktura przechowująca polecenie kalkulatora, które wymaga dokładnie jednego argumentu
 */
typedef struct OperationWithArg
{
	char* name; ///< nazwa polecenia
	long (*requiredStackSize)(Number *); ///< funkcja zwracająca wymagany przez polecenie rozmiar stosu
	void (*execute)(PolyStack *, Number *); ///< funkcja będąca wykonaniem danego polecenia
	/**
	 * typ błędu zwracany w przypadku, gdy argument operacji jest niewłaściwy
	 */
	char* argErrorType;
	long argMinValue; ///< minimalna wartość argumentu
	long argMaxValue; ///< maksymalna wartość argumentu
} OperationWithArg;

/**
 * Inicjalizuje tablice poleceń kalkulatora standardowymi operacjami
 * @param[in] operation : tablica operacji bezargumentowych
 * @param[in] opWithArg : tablica operacji jednoargumentowych
 */
void InitStandardOperations(Operation operation[], OperationWithArg opWithArg[]);

#endif /* __OPERATION_H__ */
