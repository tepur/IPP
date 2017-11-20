#include "word.h"
#include "utils.h"

bool WordIsEmpty(const Word *w)
{
	return (w->firstWordElem == NULL);
}

void WordAppend(Word *w, const char c)
{
	WordElem *l = (WordElem*)malloc(sizeof(WordElem));
	assert(l != NULL);
	l->value = c;
	l->next = NULL;
	if(WordIsEmpty(w))
	{
		w->firstWordElem = l;
		w->lastWordElem = l;
	}
	else
	{
		w->lastWordElem->next = l;
		w->lastWordElem = l;
	}
}
Word EmptyWord()
{
	Word w;
	w.firstWordElem = NULL;
	w.lastWordElem = NULL;
	return w;
}
unsigned WordSize(const Word *w)
{
	WordElem *iter = w->firstWordElem;
	unsigned size = 0;
	
	while(iter != NULL)
	{
		size++;
		iter = iter->next;
	}
	return size;
}
bool WordEquals(const Word *w, const char *text)
{
	if(WordSize(w) != strlen(text))
	{
		return false;
	}
	
	int currIndex = 0;
	WordElem *iter = w->firstWordElem;
	
	while(iter != NULL)
	{
		if(iter->value != text[currIndex])
		{
			return false;
		}
		iter = iter->next;
		currIndex++;
	}
	return true;
}

void WordDestroy(Word *w)
{
	WordElem *iter = w->firstWordElem;
	WordElem *nextIter;
	
	while(iter != NULL)
	{
		nextIter = iter->next;
		free(iter);
		iter = nextIter;
	}
}


