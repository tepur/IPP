#include "poly.h"
#include "utils.h"

void MonoDestroy(Mono *m)
{
	if(m != NULL)
	{
		PolyDestroy(&(m->p));
	}
}

/**
 * Usuwa jednomian @p m z pamięci i zwalnia pamięć, 
 * która została zaalokowana na sam wskaźnik.
 * @param[in] m : jednomian
 */
void MonoDestroyMalloced(Mono *m)
{
	if(m != NULL)
	{
		MonoDestroy(m);
		free(m);
	}
}

/**
 * Zwraca długość listy jednomianów @p ml.
 * @param[in] ml : lista
 * @return długość listy @p ml
 */
int MonoListLength(const MonoList *ml)
{
	int length = 0;
	Mono *iter = ml->first;
	while(iter != NULL)
	{
		length++;
		iter = iter->next;
	}
	return length;
}

/**
 * Tworzy wskaźnik na zaalokowany dynamicznie jednomian zerowy.
 * @return wskaźnik na wielomian zerowy
 */
Mono *MonoMallocEmpty()
{
	Mono *newMono = (Mono*)malloc(sizeof(Mono));
	assert(newMono != NULL);
	newMono->next = NULL;
	newMono->prev = NULL;
	newMono->exp = 0;
	newMono->p = PolyZero();
	return newMono;
}

void MonoListAppendMono(MonoList *ml, Mono *m)
{
	if(ml->first == NULL)
	{
		ml->first = m;
		ml->last = ml->first;
	
		ml->first->next = NULL;
		ml->first->prev = NULL;
	}
	else
	{
		ml->last->next = m;
		m->prev = ml->last;
		ml->last = m;
	}
}

void MonoListCopyAndAppend(MonoList *ml, Mono *m)
{
	Mono *newMono = MonoMallocEmpty();
	*newMono = MonoClone(m);
	
	MonoListAppendMono(ml, newMono);
}

void PolyDestroy(Poly *p)
{
	if(p != NULL)
	{
		Mono *prev = p->ml.last;
		while(p->ml.last != NULL)
		{
			prev = p->ml.last->prev;
			MonoDestroyMalloced(p->ml.last);
			p->ml.last = prev;
		}
	
		p->ml.first = NULL;
		p->ml.last = NULL;
		p->c = 0;
	}
}

Poly PolyClone(const Poly *p)
{
	Poly pClone = PolyFromCoeff(p->c);
	
	if(!PolyIsCoeff(p))
	{
		Mono *iter = p->ml.first;
		
		while(iter != NULL)
		{
			Mono *mClone = MonoMallocEmpty();
			*mClone = MonoClone(iter);
				
			MonoListAppendMono(&(pClone.ml), mClone);
			
			iter = iter->next;
		}
	}
	return pClone;
}

/**
 * Porównuje jednomiany względem ich wykładników.
 * @param[in] fst : jednomian
 * @param[in] snd : jednomian
 * @return liczba < 0, jeśli @p fst < @p snd, 0, jeśli @p fst == @p snd, a > 0, jeśli @p fst > @p snd.
 */
int MonoCmp(const void *fst, const void *snd)
{
	int expFst = ((Mono*)fst)->exp;
	int expSnd = ((Mono*)snd)->exp;
	return (expFst - expSnd);
}

/**
 * Jeśli @p p jest wielomianem stałym, dodaje mu jeden jednomian tak, by tożsamościowo się nie zmienił.
 * @param[in] p : wielomian
 */
void PolyTransformIfCoeff(Poly *p)
{
	if(PolyIsCoeff(p))
	{
		Mono *m = MonoMallocEmpty();
		m->p = PolyClone(p);
		
		MonoListAppendMono(&(p->ml), m);
	}
}
/**
 * Sprawdza, czy wielomian @p p składa się z dokładnie jednego jednomianu.
 * @param[in] p : wielomian
 */
bool PolyHasOnlyOneMono(const Poly *p)
{
	return ((p->ml.first != NULL) && (p->ml.first == p->ml.last));
}

/**
 * Przekształca wielomian zawierający jeden jednomian na wielomian stały.
 * Funkcja zakłada, że taka operacja jest możliwa.
 * @param[in] p : wielomian
 */
void PolyTurnToCoeff(Poly *p)
{
	p->c = p->ml.last->p.c;
	MonoDestroyMalloced(p->ml.last);
	p->ml.first = NULL;
	p->ml.last = NULL;
}

/**
 * Dodaje (w sensie matematycznym) jednomian @p m do wielomianu @p p (czyli @p p := @p p + @p m)
 * @param[in] p : wielomian
 * @param[in] m : jednomian
 */
void PolyAppendMono(Poly *p, Mono *m)
{
	if(PolyIsZero(&(m->p)))
	{
		MonoDestroyMalloced(m);
		return;
	}
	
	if(PolyIsZero(p))
	{
		if(m->exp != 0 || (m->exp == 0 && (!PolyIsCoeff(&m->p))))
		{
			MonoListAppendMono(&(p->ml), m);
		}
		else
		{
			p->c = m->p.c;
			MonoDestroyMalloced(m);
		}
	}
	else
	{
		PolyTransformIfCoeff(p);
		if(p->ml.last->exp != m->exp)
		{
			MonoListAppendMono(&(p->ml), m);
		}
		else
		{
			Poly oldP = p->ml.last->p;
			p->ml.last->p = PolyAdd(&(p->ml.last->p), &(m->p));
			PolyDestroy(&oldP);
			
			if(PolyIsCoeff(&(p->ml.last->p)))
			{
				if((PolyHasOnlyOneMono(p)) && (p->ml.last->exp == 0 || PolyIsZero(&(p->ml.last->p))))
				{
					PolyTurnToCoeff(p);
				}
				else if(PolyIsZero(&(p->ml.last->p)))
				{
					p->ml.last = p->ml.last->prev;
					MonoDestroyMalloced(p->ml.last->next);
					p->ml.last->next = NULL;
					
					//jesli zostal taki, co zostal stworzony przez "napraw jesli coeff":
					if((PolyHasOnlyOneMono(p)) && PolyIsCoeff(&(p->ml.first->p)) && p->ml.first->exp == 0)
					{
						PolyTurnToCoeff(p);
					}
				}
			}
			MonoDestroyMalloced(m);
		}
	}
}

Poly PolyAddMonosFromMonoList(MonoList *ml)
{
	Mono *monos = malloc((MonoListLength(ml)) * sizeof(Mono));
	assert(monos != NULL);
	
	Mono *iter = ml->first;
	int index = 0;
	
	while(iter != NULL)
	{
		monos[index] = *iter;
		index++;
		iter = iter->next;
	}
	
	Poly res = PolyAddMonos(MonoListLength(ml), monos);
	free(monos);
	
	iter = ml->first;
	Mono *iter2;
	while(iter != NULL)
	{
		iter2 = iter->next;
		free(iter); //wszystko "głębiej" zostanie usunięte 
					//przez wywołany wcześniej PolyAddMonos
		iter = iter2;
	}
	*ml = EmptyMonoList();
	
	return res;
}

Poly PolyAddMonos(unsigned count, const Mono monos[])
{
	Poly res = PolyZero();
	Mono *monosSorted = (Mono*)malloc(count * sizeof(Mono));
	assert(monosSorted != NULL);
	
	unsigned i;
	for(i = 0; i < count; i++)
	{
		monosSorted[i] = monos[i];
	}
	
	qsort(monosSorted, count, sizeof(Mono), MonoCmp);
	
	for(i = 0; i < count; i++)
	{
		Mono *newMono = MonoMallocEmpty();
		*newMono = MonoClone(&monosSorted[i]);
		
		PolyAppendMono(&res, newMono);
	}
	for(i = 0; i < count; i++)
	{
		MonoDestroy(&monosSorted[i]);
	}
	free(monosSorted);
	return res;
}

Poly PolySub(const Poly *p, const Poly *q)
{
	Poly negQ = PolyNeg(q);
	Poly res = PolyAdd(p, &negQ);
	PolyDestroy(&negQ);
	return res;
}
/**
 * Zwraca większą z liczb @p p i @p q. W przypadku, gdy obie są równe, zwracą @p q.
 * @param[in] p : liczba całkowita
 * @param[in] q : liczba całkowita
 * @return @p p, jeśli @p p > @p q, w przeciwnym przypadku @p q
 */
poly_exp_t Max(poly_exp_t f, poly_exp_t s)
{
	if(f > s)return f;
	return s;
}

poly_exp_t PolyDeg(const Poly *p)
{
	if(PolyIsZero(p))
	{
		return (-1);
	}
	if(PolyIsCoeff(p))
	{
		return 0;
	}
	poly_exp_t maxExp = (-1);
	Mono *iter = p->ml.first;
	
	while(iter != NULL)
	{
		poly_exp_t currPolyDeg = (PolyDeg(&(iter->p)) + iter->exp);
		maxExp = Max(maxExp, currPolyDeg);
		iter = iter->next;
	}
	return maxExp;
}

poly_exp_t PolyDegBy(const Poly *p, unsigned var_idx)
{
	if(PolyIsZero(p))
	{
		return -1;
	}
	if(PolyIsCoeff(p))
	{	
		return 0;
	}
	if(var_idx == 0)
	{
		return p->ml.last->exp;
	}
	
	poly_exp_t maxExp = (-1);
	Mono *iter = p->ml.first;
	
	while(iter != NULL)
	{
		poly_exp_t currPolyDeg = PolyDegBy(&(iter->p), var_idx - 1);
		maxExp = Max(maxExp, currPolyDeg);
		iter = iter->next;
	}
	return maxExp;
}

bool PolyIsEq(const Poly *p, const Poly *q)
{
	if(PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return (p->c == q->c);
	}
	Mono *iterP = p->ml.first;
	Mono *iterQ = q->ml.first;
	
	while(iterP != NULL && iterQ != NULL)
	{
		if((!PolyIsEq(&(iterP->p), &(iterQ->p))) || (iterP->exp != iterQ->exp))
		{
			return false;
		}
		iterP = iterP->next;
		iterQ = iterQ->next;
	}
	if((iterP == NULL && iterQ != NULL) || (iterP != NULL && iterQ == NULL))
	{
		return false;
	}
	return true;
}

/**
 * Do listy jednomianów @p ml dodaje głębokie kopie jednomianów zawartych w wielomianie @p p. 
 * Jeśli @p p jest wielomianem zerowym, nie dodaje nic. Jeśli jest wielomianem 
 * stałym różnym od zerowego, dodaje odpowiadający mu jednomian.
 * @param[in] ml : lista jednomianów
 * @param[in] p : wielomian
 */
void MonoListAppendCopiedMonosFromPoly(MonoList *ml, const Poly *p)
{
	if(PolyIsCoeff(p))
	{
		if(!PolyIsZero(p))
		{
			Poly pCopy = PolyClone(p);
			Mono m = MonoFromPoly(&pCopy, 0);
			
			MonoListCopyAndAppend(ml, &m);
			MonoDestroy(&m);
		}
	}
	else
	{
		Mono *iter = p->ml.first;
	
		while(iter != NULL)
		{
			MonoListCopyAndAppend(ml, iter);
			iter = iter->next;
		}
	}
}

Poly PolyAdd(const Poly *p, const Poly *q)
{
	if(PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return PolyFromCoeff(p->c + q->c);
	}
	MonoList ml = EmptyMonoList();
	
	MonoListAppendCopiedMonosFromPoly(&ml, p);
	MonoListAppendCopiedMonosFromPoly(&ml, q);
	
	return PolyAddMonosFromMonoList(&ml);
}
/**
 * Mnoży dwa jednomiany.
 * @param[in] m1 : jednomian
 * @param[in] m2 : jednomian
 * @return `m1 * m2`
 */
Mono MonoMul(const Mono *m1, const Mono *m2)
{
	Poly p = PolyMul(&(m1->p), &(m2->p));
	return MonoFromPoly(&p, (m1->exp + m2->exp));
}

Poly PolyMul(const Poly *p, const Poly *q)
{
	if(PolyIsZero(p) || PolyIsZero(q))
	{
		return PolyZero();
	}
	if(PolyIsCoeff(p) && PolyIsCoeff(q))
	{
		return PolyFromCoeff((p->c) * (q->c));
	}
	if(PolyIsCoeff(q))
	{
		return PolyMul(q, p);
	}
	if(PolyIsCoeff(p))
	{
		Poly pCopy = PolyClone(p);
		PolyTransformIfCoeff(&pCopy);
		
		Poly res = PolyMul(&pCopy, q);
		PolyDestroy(&pCopy);
		
		return res;
	}
	
	MonoList res = EmptyMonoList();
	
	Mono *iterP = p->ml.first, *iterQ = q->ml.first;
	while(iterP != NULL)
	{
		while(iterQ != NULL)
		{
			Mono mulMono = MonoMul(iterP, iterQ);
			if(!PolyIsZero(&(mulMono.p)))
			{
				MonoListCopyAndAppend(&res, &mulMono);
			}
			MonoDestroy(&mulMono);
			
			iterQ = iterQ->next;
		}
		iterQ = q->ml.first;
		iterP = iterP->next;
	}
	return PolyAddMonosFromMonoList(&res);
}

/**
 * Wielomianowi @p p rekurencyjnie ustawia wartości "wewnętrznych" stałych na przeciwne.
 * @param[in] p : wielomian
 */
void PolySetInverseCoeffs(Poly *p)
{
	if(PolyIsCoeff(p))
	{
		p->c = -(p->c);
	}
	else
	{
		Mono *iter = p->ml.first;
		
		while(iter != NULL)
		{
			PolySetInverseCoeffs(&(iter->p));
			iter = iter->next;
		}
	}
}

Poly PolyNeg(const Poly *p)
{
	Poly neg = PolyClone(p);
	PolySetInverseCoeffs(&neg);
	return neg;
}

/**
 * Zwraca @p x podniesiony do potęgi @p exp.
 * @param[in] x : liczba całkowita
 * @param[in] exp : liczba całkowita
 * @return @p x ^ @p exp.
 */
poly_coeff_t PowI(poly_coeff_t x, poly_exp_t exp)
{
	if(exp == 0)return 1;
	
	if(exp % 2 == 1)
	{
		return x * PowI(x, exp - 1);
	}
	poly_coeff_t a = PowI(x, exp / 2);
	return a * a;
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
	if(PolyIsCoeff(p))
	{
		Poly res = PolyClone(p);
		return res;
	}
	
	Mono *iter = p->ml.first;
	MonoList ml = EmptyMonoList();
	
	while(iter != NULL)
	{
		Poly coeff = PolyFromCoeff(PowI(x, iter->exp));
		Poly mul = PolyMul(&iter->p, &coeff);
		if(PolyIsCoeff(&mul))
		{
			if(!PolyIsZero(&mul))
			{
				Mono nm = MonoFromPoly(&mul,0);
				MonoListCopyAndAppend(&ml, &nm);
				MonoDestroy(&nm);
			}
			else
			{
				PolyDestroy(&mul);
			}
		}
		else
		{
			MonoListAppendCopiedMonosFromPoly(&ml, &mul);
			PolyDestroy(&mul);
		}
		PolyDestroy(&coeff);
		iter = iter->next;
	}
	return PolyAddMonosFromMonoList(&ml);
}

Mono MonoClone(const Mono *m)
{
	Mono mClone;
	mClone.exp = m->exp;
	mClone.p = PolyClone(&(m->p));
	mClone.next = NULL;
	mClone.prev = NULL;
	return mClone;
}
void PrintPoly(const Poly *p)
{
	if(PolyIsCoeff(p))
	{
		printf("%ld", p->c);
	}
	else
	{
		Mono *iter = p->ml.first;
		
		while(iter != NULL)
		{
			PrintMono(iter);
			if(iter->next != NULL)
			{
				printf("+");
			}
			iter = iter->next;
		}
	}
}

void PrintMono(const Mono *m)
{
	printf("(");
	PrintPoly(&(m->p));
	printf(",%d)",m->exp);
}
/*
Mono MonoZero()
{
	Poly p = PolyZero();
	Mono m = MonoFromPoly(&p, 0);
	return m;
}*/

Poly PolyPow(const Poly *p, poly_exp_t exp)
{
	if(exp == 0)return PolyFromCoeff(1);
	Poly res = PolyZero();
	Poly p2;
	if(exp % 2 == 1)
	{
		p2 = PolyPow(p, exp - 1);
		res = PolyMul(p, &p2);
	}
	else{
		p2 = PolyPow(p, exp / 2);
		res = PolyMul(&p2, &p2);
	}
	PolyDestroy(&p2);
	return res;
}

Poly PolyComposeExecute(const Poly *p, unsigned count, const Poly x[], unsigned currLevel);

Poly MonoComposeExecute(Mono *m, unsigned count, const Poly x[], unsigned currLevel)
{
	if(currLevel >= count)
	{
		return PolyZero();
	}
	Poly furtherPart = PolyComposeExecute(&(m->p), count, x, currLevel + 1);
	if(PolyIsZero(&furtherPart))
	{
		return PolyZero();
	}
	Poly exchangedPart = PolyPow(&(x[currLevel]), m->exp);
	Poly res = PolyMul(&furtherPart, &exchangedPart);
	PolyDestroy(&exchangedPart);
	PolyDestroy(&furtherPart);
	return res;
}

Poly PolyComposeExecute(const Poly *p, unsigned count, const Poly x[], unsigned currLevel)
{
	if(PolyIsCoeff(p))
	{
		return PolyFromCoeff(p->c);
	}
	
	Poly res = PolyZero();
	Mono *iter = p->ml.first;
	while(iter != NULL)
	{
		Poly iterPoly = MonoComposeExecute(iter, count, x, currLevel);
		
		Poly newRes = PolyAdd(&res, &iterPoly);
		PolyDestroy(&res);
		res = newRes;
		
		PolyDestroy(&iterPoly);
		
		iter = iter->next;
	}
	return res;
}

Poly PolyCompose(const Poly *p, unsigned count, const Poly x[])
{
	if(count == 0)
	{
		return PolyZero();
	}
	
	return PolyComposeExecute(p, count, x, 0);
}
