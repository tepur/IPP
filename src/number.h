/** @file
   Interfejs klasy liczb
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/
#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <stdbool.h>
#include <limits.h>
#include "word.h"

/**
 * Struktura przechowująca liczbę o nieokreślonej z góry maksymalnej długości
 * zbudowana na podstawie struktury Word
 */
typedef struct Number
{
	Word repr; ///< reprezentacja liczby w postaci słowa (samych cyfr)
	/**
	 * Czy liczba jest ujemna
	 * Odpowiada on za "znak minusa" poprzedzający liczbę w przypadku, gdy jest ona ujemna
	 */
	bool isNegative;
} Number;

/**
 * Usuwa liczbę z pamięci
 * @param[in] n : liczba
 */
void NumberDestroy(Number *n);
/**
 * Sprawdza, czy liczba jest pusta (nie zawiera żadnej cyfry)
 * @param[in] n : liczba
 * @return Czy liczba jest pusta
 */
static inline bool NumberIsEmpty(const Number *n)
{
	return WordIsEmpty(&(n->repr));
}
/**
 * Dodaje cyfrę @p c na koniec liczby @p n
 * @param[in] n : liczba
 * @param[in] c : cyfra
 */
static inline void NumberAppend(Number *n, char c)
{
	WordAppend(&(n->repr), c);
}
/**
 * Zwraca długość liczby
 * @param[in] n : liczba
 * @return Długość liczby
 */
static inline int NumberSize(const Number *n)
{
	return WordSize(&(n->repr));
}
/**
 * Zwraca pustą liczbę (bez cyfr, ustawioną jako nieujemna)
 * @return pusta liczba
 */
Number EmptyNumber();
/**
 * Porównuje dwie liczby
 * @param[in] n1 : liczba
 * @param[in] n2 : liczba
 * @return 1, jeśli @p n1 > @p n2, 0, jeśli są równe, -1, jeśli @p n1 < @p n2
 */
int CmpNumber(const Number *n1, const Number *n2);
/**
 * Porównuje liczbę typu Number z liczbą typu long.
 * @param[in] n : liczba (typu Number)
 * @param[in] l : liczba (typu long)
 * @return 1, jeśli @p n1 > @p l, 0, jeśli są równe, -1, jeśli @p n1 < @p l
 */
int CmpNumberLong(const Number *n, const long l);
/**
 * Konwertuje liczbę typu long na liczbę typu Number
 * @param[in] l : liczba (typu long)
 * @return skonwertowana liczba typu Number
 */
Number LongToNumber(long l);
/**
 * Konwertuje liczbę typu Number na liczbę typu long
 * @param[in] n : liczba (typu Number)
 * @return skonwertowana liczba typu long
 */
long NumberToLong(const Number *n);

#endif /* __NUMBER_H__ */
