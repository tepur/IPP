#include "polystack.h"
#include "operation.h"
#include "read.h"

#include "utils.h"

int main()
{
	PolyStack polyStack = EmptyPolyStack(); //stos kalkulatora
	Operation operation[OPER_WITHOUT_ARG_AMOUNT]; //bezargumentowe operacje kalkulatora
	OperationWithArg operWithArg[OPER_WITH_ARG_AMOUNT]; //jednoargumentowe operacje kalkulatora
	
	InitStandardOperations(operation, operWithArg);
	
	int currLine = 1;
	
	while(ReadLine(&polyStack, currLine, operation, operWithArg))
	{
		currLine++;
	}
	DestroyStack(&polyStack);
	   
    return 0;
}
