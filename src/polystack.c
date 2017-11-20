#include "polystack.h"

#include "utils.h"

PolyStack EmptyPolyStack()
{
	PolyStack pStack;
	pStack.topElem = NULL;
	return pStack;
}
bool PolyStackHasEnoughElements(PolyStack *pStack, long numOfElems)
{
	if(numOfElems <= 0)
	{
		return true;
	}
	
	PolyStackElem *iter = pStack->topElem;
	int elems = 0;
	
	while(iter != NULL)
	{
		elems++;
		if(elems >= numOfElems)
		{
			return true;
		}
		iter = iter->prev;
	}
	return false;
}
void PolyStackPush(PolyStack *pStack, Poly *p)
{
	PolyStackElem *newElem = (PolyStackElem*)malloc(sizeof(PolyStackElem));
	assert(newElem != NULL);
	newElem->p = *p;
	newElem->prev = pStack->topElem;
	pStack->topElem = newElem;
}
void PolyStackPop(PolyStack *pStack)
{
	PolyDestroy(&(pStack->topElem->p));
	PolyStackElem *elemToRemove = pStack->topElem;
	pStack->topElem = pStack->topElem->prev;
	free(elemToRemove);
}
void DestroyStack(PolyStack *pStack)
{
	while(!PolyStackIsEmpty(pStack))
	{
		PolyStackPop(pStack);
	}
}
