/** @file
   Interfejs klasy słów
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/
#ifndef __WORD_H__
#define __WORD_H__

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "poly.h"

typedef struct WordElem WordElem;

/**
 * Element listy typu Word, przechowujący pojedynczy znak.
 */
typedef struct WordElem
{
	char value; ///< przechowywana wartość
	WordElem *next; ///< wskaźnik na kolejny element listy (NULL jeśli jest to ostatni element)
} WordElem;

/**
 * Struktura przechowująca ciąg znaków o nieokreślonej z góry maksymalnej długości
 * jest ona zbudowana na zasadzie listy jednokierunkowej, której elementy są typu WordElem
 */
typedef struct Word
{
	WordElem *firstWordElem; ///< wskaźnik na pierwszy element listy
	WordElem *lastWordElem; ///< wskaźnik na ostatni element listy
} Word;

/**
 * Sprawdza, czy słowo jest puste (nie zawiera żadnego znaku)
 * @param[in] w : słowo
 * @return Czy słowo jest puste
 */
bool WordIsEmpty(const Word *w);

/**
 * Dodaje literę @p c na koniec słowa @p w
 * @param[in] w : słowo
 * @param[in] c : litera
 */
void WordAppend(Word *w, char c);

/**
 * Zwraca długość słowa
 * @param[in] w : słowo
 * @return Długość słowa
 */
unsigned WordSize(const Word *w);

/**
 * Zwraca puste słowo
 * @return Puste słowo
 */
Word EmptyWord();

/**
 * Sprawdza, czy słowo @p w (typu Word) jest równe ciągowi znaków @p text (typu char*)
 * @param[in] w : słowo
 * @param[in] text : ciąg znaków
 * @return Czy są równe
 */
bool WordEquals(const Word *w, const char *text);
/**
 * Usuwa słowo z pamięci
 * @param[in] w : słowo
 */
void WordDestroy(Word *w);



#endif /* __WORD_H__ */
