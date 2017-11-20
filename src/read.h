/** @file
   Interfejs funkcji służących do wczytywania i parsowania linii
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#ifndef __READ_H__
#define __READ_H__

#include <stdbool.h>
#include <limits.h>
#include "poly.h"
#include "number.h"
#include "error.h"
#include "polystack.h"
#include "operation.h"

#define MIN_COEFF LONG_MIN
#define MAX_COEFF LONG_MAX

#define MIN_EXP 0
#define MAX_EXP INT_MAX

/**
 * Wczytuje ze standardowego wejścia wielomian, "przesuwa" numer kolumny
 * i zgłasza błąd, gdy takowy napotka
 * Kończy wczytywanie, gdy następny znak nie może być "przedłużeniem" wielomianu
 * @param[in] lineNumber : aktualny numer linii
 * @param[in] columnNumber : aktualny numer kolumny (numer kolumny, w której wczytywany wielomian się zaczyna)
 * @param[in] hasError : czy w danym wierszu wystąpił już błąd
 * @return Wczytany wielomian
 */
Poly ReadPoly(int lineNumber, int *columnNumber, bool *hasError);

/**
 * Wczytuje ze standardowego wejścia jednomian, "przesuwa" numer kolumny
 * i zgłasza błąd, gdy takowy napotka
 * Kończy wczytywanie, gdy następny znak nie może być "przedłużeniem" jednomianu
 * @param[in] lineNumber : aktualny numer linii
 * @param[in] columnNumber : aktualny numer kolumny (numer kolumny, w której wczytywany monomian się zaczyna)
 * @param[in] hasError : czy w danym wierszu wystąpił już błąd
 * @return Wczytana liczba
 */
Mono ReadMono(int lineNumber, int *columnNumber, bool *hasError);

/**
 * Wczytuje ze standardowego wejścia nazwę polecenia
 * Kończy wczytywanie, gdy następny znak nie może być "przedłużeniem" nazwy
 * (np. jest znakiem końca linii lub spacją)
 * @return Wczytana nazwa polecenia
 */
Word ReadCommandName();

/**
 * Wczytuje ze standardowego wejścia liczbę (typu Number)
 * Kończy wczytywanie, gdy następny znak nie może być "przedłużeniem" liczby
 * Zgłasza błąd, gdy jego wartość przekracza podane limity
 * @param[in] lineNumber : aktualny numer linii
 * @param[in] columnNumber : aktualny numer kolumny (numer kolumny, w której wczytywana liczba się zaczyna)
 * @param[in] hasError : czy w danym wierszu wystąpił już błąd
 * @param[in] minValue : minimalna wartość liczby
 * @param[in] maxValue : maksymalna wartość liczby
 * @return Wczytana liczba, a w przypadku przekroczenia limitu jej najdłuższy prawidłowy prefiks
 */
Number ReadNumberForParse(int lineNumber, int *columnNumber, bool *hasError, long minValue, long maxValue);

/**
 * Wczytuje ze standardowego wejścia liczbę (typu Number)
 * Kończy wczytywanie, gdy następny znak nie może być "przedłużeniem" liczby
 * @return Wczytana liczba
 */
Number ReadNumber();

/**
 * Wczytuje wiersz
 * w zależności od pierwszego znaku, wykonuje operację kalkulatora lub parsowanie wielomianu
 * parsowany wielomian jest dodany do kalkulatorowego stosu wielomianów, jeśli nie ma błędów
 * @param[in] pStack : stos wielomianów (część kalkulatora)
 * @param[in] lineNumber : aktualny numer wiersza
 * @param[in] operation : bezargumentowe operacje udostępnione przez kalkulator
 * @param[in] opWithArg : jednoargumentowe operacje udostępnione przez kalkulator
 * @return false, jeśli pierwszy znak wiersza to EOF, true w przeciwnym wypadku
 */
bool ReadLine(PolyStack *pStack, int lineNumber, Operation operation[], OperationWithArg opWithArg[]);

/**
 * Sprawdza, czy znak jest cyfrą
 * @param[in] c : znak
 * @return Czy znak jest cyfrą
 */
static inline bool IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}

/**
 * Sprawdza, czy znak jest literą
 * @param[in] c : znak
 * @return Czy znak jest literą
 */
static inline bool IsLetter(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));   
}
/**
 * Sprawdza, czy znak jest poprawnym znakiem polecenia
 * @param[in] c : znak
 * @return Czy znak jest poprawnym znakiem polecenia
 */
static inline bool IsCorrectCommandChar(char c)
{
	return (IsLetter(c) || c == '_');
}
/**
 * Wczytuje i wykonuje polecenie kalkulatora
 * w przypadku, gdy wykonanie nie jest możliwe, zwraca odpowiedni błąd
 * @param[in] pStack : stos wielomianów (część kalkulatora)
 * @param[in] lineNumber : aktualny numer wiersza
 * @param[in] operation : bezargumentowe operacje udostępnione przez kalkulator
 * @param[in] opWithArg : jednoargumentowe operacje udostępnione przez kalkulator
 */
void ReadAndExecuteCommand(PolyStack *pStack, int lineNumber, Operation operation[], OperationWithArg opWithArg[]);

#endif /* __READ_H__ */
