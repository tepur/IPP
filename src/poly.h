/** @file
   Interfejs klasy wielomianów

   @author Jakub Pawlewicz <pan@mimuw.edu.pl>, Michał Tepper <mt386430@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-13
*/

#ifndef __POLY_H__
#define __POLY_H__

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/** Typ współczynników wielomianu */
typedef long poly_coeff_t;

/** Typ wykładników wielomianu */
typedef int poly_exp_t;

typedef struct Mono Mono;

/**
 * Struktura przechowująca listę (dwukierunkową) jednomianów
 * składa się z wskaźnika na pierwszy i ostatni jednomian.
 */
typedef struct MonoList
{
	Mono *first; ///< wskaźnik na pierwszy element w liście
	Mono *last; ///< wskaźnik na ostatni element w liście 
}MonoList;

/**
 * Struktura przechowująca wielomian
 * Wielomian jest listą jednomianów lub pojedynczą liczbą, 
 * jeśli jest wielomianem stałym.
 */
typedef struct Poly
{
	/** Lista jednomianów.
	 *  Przyjęte konwencje: 
	 *  1) monomiany w liście są posortowane po wykładnikach w kolejności rosnącej
	 *  2) każdy dodany jednomian powstał w wyniku malloc'a.
	 */
	MonoList ml; 
	/** Stała (w przypadku, gdy wielomian jest stałą). 
	 *  W przeciwnym razie jest to dowolna liczba.
	 */
	poly_coeff_t c; 
} Poly;

/**
  * Struktura przechowująca jednomian
  * Jednomian ma postać `p * x^e`.
  * Współczynnik `p` może też być wielomianem.
  * Będzie on traktowany jako wielomian nad kolejną zmienną (nie nad x).
  * 
  * Jednomian, jako część wielomianu (a tym samym część jego listy jednomianów), 
  * ma wskaźniki na poprzedni i następny element w tej liście jednomianów tego wielomianu 
  * (jeśli nie jest w żadnej liście, wartości są dowolne).
  */
typedef struct Mono
{
    Poly p; ///< współczynnik 
    poly_exp_t exp; ///< wykładnik
    
	Mono *prev; ///< wskaźnik na poprzedni element w liście
	Mono *next; ///< wskaźnik na następny element w liście 
} Mono;

/**
 * Tworzy pustą listę jednomianów
 * @return Lista jednomianów
 */
static inline MonoList EmptyMonoList()
{
	return (MonoList) {.first = NULL, .last = NULL};
}
/**
 * Sprawdza, czy lista jednomianów jest pusta
 * @param[in] ml : lista jednomianów
 * @return Czy lista jednomianów jest pusta
 */
static inline bool MonoListIsEmpty(const MonoList *ml)
{
	return (ml->first == NULL);
}

/**
 * Przekształca listę jednomianów @p ml na tablicę jednomianów 
 * i zwraca wynik wykonanej na niej funkcji PolyAddMonos().
 * Funkcja zakłada, że taka operacja jest możliwa.
 * @param[in] ml : lista jednomianów
 * @return wynik funkcji PolyAddMonos()
 */
Poly PolyAddMonosFromMonoList(MonoList *ml);

/**
 * Dodaje do listy jednomianów @p ml dynamicznie zaalokowaną głęboką kopię jednomianu @p m.
 * @param[in] ml : lista
 * @param[in] m : jednomian
 */
void MonoListCopyAndAppend(MonoList *ml, Mono *m);

/**
 * Dodaje jednomian @p m na koniec listy jednomianów @p ml.
 * Przejmuje jednomian @m na własność.
 * @param[in] ml : lista
 * @param[in] m : jednomian
 */
void MonoListAppendMono(MonoList *ml, Mono *m);

/**
 * Tworzy wielomian, który jest współczynnikiem
 * @param[in] c : wartość współczynnika
 * @return wielomian
 */
static inline Poly PolyFromCoeff(poly_coeff_t c)
{
	return (Poly) {.ml.first = NULL, .ml.last = NULL, .c = c};
}

/**
 * Tworzy wielomian tożsamościowo równy zeru.
 * @return wielomian
 */
static inline Poly PolyZero()
{
    return PolyFromCoeff(0);
}

/**
 * Tworzy jednomian `p * x^e`.
 * Przejmuje na własność zawartość struktury wskazywanej przez @p p.
 * @param[in] p : wielomian - współczynnik jednomianu
 * @param[in] e : wykładnik
 * @return jednomian `p * x^e`
 */
static inline Mono MonoFromPoly(const Poly *p, poly_exp_t e)
{
    return (Mono) {.p = *p, .exp = e, .prev = NULL, .next = NULL};
}

/**
 * Sprawdza, czy wielomian jest współczynnikiem.
 * @param[in] p : wielomian
 * @return Czy wielomian jest współczynnikiem?
 */
static inline bool PolyIsCoeff(const Poly *p)
{
    return (p->ml.first == NULL);
}

/**
 * Sprawdza, czy wielomian jest tożsamościowo równy zeru.
 * @param[in] p : wielomian
 * @return Czy wielomian jest równy zero?
 */
static inline bool PolyIsZero(const Poly *p)
{
    return (PolyIsCoeff(p) && p->c == 0);
}

/**
 * Usuwa wielomian z pamięci.
 * @param[in] p : wielomian
 */
void PolyDestroy(Poly *p);

/**
 * Usuwa jednomian z pamięci.
 * @param[in] m : jednomian
 */
void MonoDestroy(Mono *m);

/**
 * Robi pełną, głęboką kopię wielomianu.
 * @param[in] p : wielomian
 * @return skopiowany wielomian
 */
Poly PolyClone(const Poly *p);

/**
 * Robi pełną, głęboką kopię jednomianu.
 * @param[in] m : jednomian
 * @return skopiowany jednomian
 */
Mono MonoClone(const Mono *m);

/**
 * Dodaje dwa wielomiany.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p + q`
 */
Poly PolyAdd(const Poly *p, const Poly *q);

/**
 * Sumuje listę jednomianów i tworzy z nich wielomian.
 * Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return wielomian będący sumą jednomianów
 */
Poly PolyAddMonos(unsigned count, const Mono monos[]);

/**
 * Mnoży dwa wielomiany.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p * q`
 */
Poly PolyMul(const Poly *p, const Poly *q);

/**
 * Zwraca przeciwny wielomian.
 * @param[in] p : wielomian
 * @return `-p`
 */
Poly PolyNeg(const Poly *p);

/**
 * Odejmuje wielomian od wielomianu.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p - q`
 */
Poly PolySub(const Poly *p, const Poly *q);

/**
 * Zwraca przeciwny wielomian.
 * @param[in] p : wielomian
 * @return `-p`
 */
Poly PolyNeg(const Poly *p);

/**
 * Odejmuje wielomian od wielomianu.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p - q`
 */
Poly PolySub(const Poly *p, const Poly *q);

/**
 * Zwraca stopień wielomianu ze względu na zadaną zmienną (-1 dla wielomianu
 * tożsamościowo równego zeru).
 * Zmienne indeksowane są od 0.
 * Zmienna o indeksie 0 oznacza zmienną główną tego wielomianu.
 * Większe indeksy oznaczają zmienne wielomianów znajdujących się
 * we współczynnikach.
 * @param[in] p : wielomian
 * @param[in] var_idx : indeks zmiennej
 * @return stopień wielomianu @p p z względu na zmienną o indeksie @p var_idx
 */
poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx);

/**
 * Zwraca stopień wielomianu (-1 dla wielomianu tożsamościowo równego zeru).
 * @param[in] p : wielomian
 * @return stopień wielomianu @p p
 */
poly_exp_t PolyDeg(const Poly *p);

/**
 * Sprawdza równość dwóch wielomianów.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return `p = q`
 */
bool PolyIsEq(const Poly *p, const Poly *q);

/**
 * Wylicza wartość wielomianu w punkcie @p x.
 * Wstawia pod pierwszą zmienną wielomianu wartość @p x.
 * W wyniku może powstać wielomian, jeśli współczynniki są wielomianem
 * i zmniejszane są indeksy zmiennych w takim wielomianie o jeden.
 * Formalnie dla wielomianu @f$p(x_0, x_1, x_2, \ldots)@f$ wynikiem jest
 * wielomian @f$p(x, x_0, x_1, \ldots)@f$.
 * @param[in] p
 * @param[in] x
 * @return @f$p(x, x_0, x_1, \ldots)@f$
 */
Poly PolyAt(const Poly *p, poly_coeff_t x);

/**
 * Wypisuje na standardowe wyjście wielomian
 * @param[in] p : wielomian
 */
void PrintPoly(const Poly *p);


/**
 * Wypisuje na standardowe wyjście monomian
 * @param[in] m : monomian
 */
void PrintMono(const Mono *m);

/**
 * Zwraca wielomian, w którym pod zmienną x_i podstawiony jest wielomian @p x[i].
 * Jeśli count = 0, zwraca wielomian zerowy.
 * Jeśli count < k + 1, gdzie k - największy indeks zmiennej występujący w jednomianach 
 * o wykładniku większym od zera danego wielomianu, to x_i = 0, dla i = count, ..., k
 * @param[in] p : wielomian, na którym wykonywana jest operacja
 * @param[in] count : rozmiar tablicy @p x
 * @param[in] x : tablica wielomianów podstawianych pod zmienne wielomianu @p p
 * @return przekształcony wielomian
 */
Poly PolyCompose(const Poly *p, unsigned count, const Poly x[]);

#endif /* __POLY_H__ */

