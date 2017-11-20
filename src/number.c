#include "number.h"

#include "utils.h"

#define MAX_LONG_LENGTH 30

void NumberDestroy(Number *n)
{
	WordDestroy(&(n->repr));
}

Number EmptyNumber()
{
	Number n;
	n.repr = EmptyWord();
	n.isNegative = false;
	return n;
}

/**porównuje liczby pod względem ich wartości bezwzględnej 
 * czyli nie zwracając uwagi na ujemność, tylko na samą reprezentację cyfrową
 */
int CmpNumberAbs(const Number *n1, const Number *n2)
{
	if(WordSize(&(n1->repr)) > WordSize(&(n2->repr)))
	{
		return 1;
	}
	else if(WordSize(&(n1->repr)) < WordSize(&(n2->repr)))
	{
		return (-1);
	}
	else
	{
		WordElem *iter1 = n1->repr.firstWordElem;
		WordElem *iter2 = n2->repr.firstWordElem;
		
		while(iter1 != NULL && iter2 != NULL)
		{
			if(iter1->value > iter2->value)
			{
				return 1;
			}
			else if(iter1->value < iter2->value)
			{
				return (-1);
			}
			
			iter1 = iter1->next;
			iter2 = iter2->next;
		}
		return 0;
	}
}

int CmpNumber(const Number *n1, const Number *n2)
{
	if(n1->isNegative)
	{
		if(!n2->isNegative)
		{
			return (-1);
		}
		else
		{
			return (-1) * CmpNumberAbs(n1, n2);
		}
	}
	else
	{
		if(n2->isNegative)
		{
			return 1;
		}
		else
		{
			return CmpNumberAbs(n1, n2);
		}
	}
}

int CmpNumberLong(const Number *n, const long l)
{
	Number n2 = LongToNumber(l);
	int res = CmpNumber(n, &n2);
	NumberDestroy(&n2);
	return res;
}

Number LongToNumber(long l)
{
	Number res = EmptyNumber();
	
	if(l == LONG_MIN)
	{
		Number tmp = LongToNumber(l + 1);
		tmp.repr.lastWordElem->value++;
		return tmp;
	}
	
	if(l < 0)
	{
		res.isNegative = true;
		l = (-l);
	}

	char tab[MAX_LONG_LENGTH];
	int currIndex = 0;
	
	while(l != 0)
	{
		tab[currIndex] = (l % 10l) + '0';
		l /= 10;
		currIndex++;
	}
	currIndex--;
	while(currIndex >= 0)
	{
		NumberAppend(&res, tab[currIndex]);
		currIndex--;
	}
	return res;
}

long NumberToLong(const Number *n)
{
	long res = 0;
	WordElem *iter = n->repr.firstWordElem;
	
	while(iter != NULL)
	{
		res *= 10l;
		res += iter->value - '0';
		iter = iter->next;
	}
	
	if(n->isNegative)
	{
		res = (-res);
	}
	
	return res;
}
