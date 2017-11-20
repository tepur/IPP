/** @file
   Interfejs zgłaszania błędów związanych z działaniem kalkulatora wielomianów
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/
#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>
#include <stdbool.h>

#define WRONG_COMMAND "WRONG COMMAND"
#define WRONG_VALUE "WRONG VALUE"
#define WRONG_VARIABLE "WRONG VARIABLE"
#define STACK_UNDERFLOW "STACK UNDERFLOW"
#define WRONG_COUNT "WRONG COUNT"

/**
 * Wypisuje na standardowy strumień błędów informację o błędzie 
 * związanym z wykonaniem polecenia kalkulatora
 * @param[in] r : numer wiersza, w którym wystąpił błąd
 * @param[in] type : typ błędu
 */
void ErrorCommand(int r, char *type);

/**
 * Wypisuje na standardowy strumień błędów informację o błędzie 
 * związanym z parsowaniem wielomianu
 * Działa tylko w przypadku, gdy dla danego wiersza nie został jeszcze zgłoszony błąd
 * @param[in] r : numer wiersza, w którym wystąpił błąd
 * @param[in] c : numer kolumny, w której wystąpił błąd
 * @param[in] hasError : czy w danym wierszu wystąpił już błąd
 */
void ErrorParse(int r, int c, bool *hasError);

#endif /* __ERROR_H__ */
