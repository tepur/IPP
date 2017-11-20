/** @file
   Interfejs klasy stosów wielomianów
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/
#ifndef __POLYSTACK_H__
#define __POLYSTACK_H__

#include <stdbool.h>
#include <assert.h>
#include "poly.h"

typedef struct PolyStackElem PolyStackElem;

/**
 * Struktura reprezentująca element stosu wielomianów (typu PolyStack)
 */
typedef struct PolyStackElem
{
	Poly p; ///< przechowywany wielomian
	/**
	 * wskaźnik na element będący bezpośrednio pod danym elementem
	 * NULL w przypadku, gdy nie ma już elementów poniżej danego
	 */
	PolyStackElem *prev;
} PolyStackElem;

/**
 * Struktura reprezentująca stos wielomianów
 */
typedef struct PolyStack
{
	PolyStackElem *topElem; ///< element stosu będący na jego szczycie
} PolyStack;


/**
 * Zwraca pusty stos wielomianów
 * @return Pusty stos wielomianów
 */
PolyStack EmptyPolyStack();
/**
 * Sprawdza, czy stos wielomianów @p pStack ma przynajmniej @p numOfElems elementów
 * @param[in] pStack : stos wielomianów
 * @param[in] numOfElems : wymagana ilość
 * @return Czy @p pStack ma przynajmniej @p numOfElems elementów
 */
bool PolyStackHasEnoughElements(PolyStack *pStack, long numOfElems);

/**
 * Zwraca wielomian będący na szczycie stosu wielomianów
 * @param[in] pStack : stos wielomianów
 * @return Wielomian będący na szczycie stosu wielomianów
 */
static inline Poly PolyStackTop(const PolyStack *pStack)
{
	return pStack->topElem->p;
}
/**
 * Zwraca wielomian będący bezpośrednio pod wielomianem będącym na szczycie stosu wielomianów
 * @param[in] pStack : stos wielomianów
 * @return Wielomian będący bezpośrednio pod wielomianem będącym na szczycie stosu wielomianów
 */
static inline Poly PolyStackNextAfterTop(const PolyStack *pStack)
{
	return pStack->topElem->prev->p;
}
/**
 * Sprawdza, czy stos wielomianów jest pusty
 * @param[in] pStack : stos wielomianów
 * @return Czy stos wielomianów jest pusty
 */
static inline bool PolyStackIsEmpty(const PolyStack *pStack)
{
	return (pStack->topElem == NULL);
}
/**
 * Usuwa wielomian ze szczytu stosu wielomianów.
 * W szczególności, usuwa ten wielomian z pamięci
 * @param[in] pStack : stos wielomianów
 */
void PolyStackPop(PolyStack *pStack);

/**
 * Dodaje wielomian na szczyt stosu wielomianów
 * @param[in] pStack : stos wielomianów
 * @param[in] p : dodawany wielomian
 */
void PolyStackPush(PolyStack *pStack, Poly *p);

/**
 * Usuwa stos wielomianów z pamięci
 * @param[in] pStack : stos wielomianów
 */
void DestroyStack(PolyStack *pStack);

#endif /* __POLYSTACK_H__ */
