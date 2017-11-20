#include "read.h"
#include "utils.h"

/**
 * Tworzy wielomian, który jest współczynnikiem
 * @param[in] number : wartość współczynnika
 * @return wielomian
 */
Poly PolyFromCoeffNumber(Number *number)
{
	poly_coeff_t convertedNumber = (poly_coeff_t)NumberToLong(number);
	return PolyFromCoeff(convertedNumber);
}

Poly ReadPoly(int lineNumber, int *columnNumber, bool *hasError)
{
	char firstChar = getc(stdin);
	ungetc(firstChar, stdin);
	
	if(firstChar == '-' || IsDigit(firstChar))
	{
		Number coeff = ReadNumberForParse(lineNumber, columnNumber, hasError, MIN_COEFF, MAX_COEFF);
		Poly res = PolyZero();
		if(!NumberIsEmpty(&coeff))
		{
			res = PolyFromCoeffNumber(&coeff);
		}
		else
		{
			ErrorParse(lineNumber, *columnNumber, hasError);
		}
		NumberDestroy(&coeff);
		return res;
	}
	else
	{
		MonoList ml = EmptyMonoList();
		bool forcePlus = false;

		while(1)
		{
			char currChar = getc(stdin);
			if(currChar == '+')
			{
				if(forcePlus == true)
				{
					forcePlus = false;
				}
				else
				{
					ErrorParse(lineNumber, *columnNumber, hasError);
				}
			}
			else if(currChar == '(')
			{
				if(forcePlus == false)
				{
					ungetc(currChar, stdin);
					Mono m = ReadMono(lineNumber, columnNumber, hasError);
					if(!(*hasError))
					{
						MonoListCopyAndAppend(&ml, &m);
					}
					MonoDestroy(&m);
					forcePlus = true;
				}
				else
				{
					ErrorParse(lineNumber, *columnNumber, hasError);
				}
			}
			else
			{
				if(forcePlus == false)
				{
					ErrorParse(lineNumber, *columnNumber, hasError);
				}
				ungetc(currChar, stdin);
				break;
			}
			(*columnNumber)++;
		}
		
		if(MonoListIsEmpty(&ml))
		{
			ErrorParse(lineNumber, *columnNumber, hasError);
			return PolyZero();
		}
		else
		{
			Poly res = PolyAddMonosFromMonoList(&ml);
			return res;
		}
	}
}

Mono ReadMono(int lineNumber, int *columnNumber, bool *hasError)
{
	Mono res;
	
	Poly p0 = PolyZero();
	res = MonoFromPoly(&p0, 0);
	
	char currChar = getc(stdin);
	
	if(currChar != '(')
	{
		ErrorParse(lineNumber, (*columnNumber) + 1, hasError);
		ungetc(currChar, stdin);
		return res;
	}
	(*columnNumber)++;
	
	Poly p = ReadPoly(lineNumber, columnNumber, hasError);
	res = MonoFromPoly(&p, 0);
	
	currChar = getc(stdin);
	
	if(currChar != ',')
	{
		ErrorParse(lineNumber, *columnNumber, hasError);
		ungetc(currChar, stdin);
		return res;
	}
	(*columnNumber)++;
	
	Number exp = ReadNumberForParse(lineNumber, columnNumber, hasError, MIN_EXP, MAX_EXP);
	
	if(NumberIsEmpty(&exp))
	{
		ErrorParse(lineNumber, *columnNumber, hasError);
		return res;
	}
	
	res.exp = (poly_exp_t)NumberToLong(&exp);
	
	NumberDestroy(&exp);
	
	currChar = getc(stdin);
	
	if(currChar != ')')
	{
		ErrorParse(lineNumber, *columnNumber, hasError);
		ungetc(currChar, stdin);
	}
	return res;
}

Number ReadNumber()
{
	Number number = EmptyNumber();
	
	while(1)
	{
		char currChar = getc(stdin);
		
		if(currChar == '-')
		{
			if(!number.isNegative && NumberIsEmpty(&number))
			{
				number.isNegative = true;
			}
			else
			{
				ungetc(currChar, stdin);
				break;
			}
		}
		else if(IsDigit(currChar))
		{
			NumberAppend(&number, currChar);
		}
		else
		{
			ungetc(currChar, stdin);
			break;
		}
	}
	return number;
}

Number ReadNumberForParse(int lineNumber, int *columnNumber, bool *hasError, long minValue, long maxValue)
{
	Number number = EmptyNumber();
	
	while(1)
	{
		char currChar = getc(stdin);
		
		if(currChar == '-')
		{
			if(!number.isNegative && NumberIsEmpty(&number))
			{
				number.isNegative = true;
				
				if(minValue > 0L)
				{
					ErrorParse(lineNumber, *columnNumber, hasError);
					break;
				}
			}
			else
			{
				ErrorParse(lineNumber, *columnNumber, hasError);
				ungetc(currChar, stdin);
				break;
			}
		}
		else if(IsDigit(currChar))
		{
			NumberAppend(&number, currChar);
			
			if((CmpNumberLong(&number, minValue) < 0) || (CmpNumberLong(&number, maxValue) > 0))
			{
				ErrorParse(lineNumber, *columnNumber, hasError);
				ungetc(currChar, stdin);
				break;
			}
		}
		else
		{
			ungetc(currChar, stdin);
			break;
		}
		(*columnNumber)++;
	}
	return number;
}

Word ReadCommandName()
{
	Word commandName = EmptyWord();
	
	while(1)
	{
		char currChar = getc(stdin);
		if(IsCorrectCommandChar(currChar))
		{
			WordAppend(&commandName, currChar);
		}
		else
		{
			ungetc(currChar, stdin);
			break;
		}
	}
	return commandName;
}

bool ReadLine(PolyStack *pStack, int lineNumber, Operation operation[], OperationWithArg opWithArg[])
{
    char firstChar = getc(stdin);
    
    if(firstChar == EOF)return false;
    
    ungetc(firstChar, stdin);
    
    if(IsLetter(firstChar))
    {
        ReadAndExecuteCommand(pStack, lineNumber, operation, opWithArg);
    }
    else
    {
    	bool hasError = false;
    	int columnNumber = 1;
    	
        Poly p = ReadPoly(lineNumber, &columnNumber, &hasError);
        
        char currChar = getc(stdin);
        ungetc(currChar, stdin);
        
        if(currChar != '\n' && currChar != EOF)
        {
			ErrorParse(lineNumber, columnNumber, &hasError);
			PolyDestroy(&p);
        }
        else if(hasError == false)
        {
        	PolyStackPush(pStack, &p);
        }
        else
        {
        	PolyDestroy(&p);
        }
    }
    
    char currChar;
    while(1)
    {
    	currChar = getc(stdin);
    	if(currChar == EOF || currChar == '\n')
    	{
    		break;
    	}
    }
    return true;
}

void ReadAndExecuteCommand(PolyStack *pStack, int lineNumber, Operation operation[], OperationWithArg opWithArg[])
{
	Word commandName = ReadCommandName();
	
	bool nameFound = false;
	
	for(int i = 0; i < OPER_WITH_ARG_AMOUNT; i++)
	{
		if(WordEquals(&commandName, opWithArg[i].name))
		{
			nameFound = true;
			
			char currChar = getc(stdin);
			if(currChar != ' ')
			{
				if(currChar == '\n' || currChar == EOF)
				{
					ErrorCommand(lineNumber, opWithArg[i].argErrorType);
				}
				else{
					ErrorCommand(lineNumber, WRONG_COMMAND);
				}
				ungetc(currChar, stdin);
			}
			else
			{
				Number arg = ReadNumber();
				
				currChar = getc(stdin);
				ungetc(currChar, stdin);
				
				if((currChar != '\n' && currChar != EOF) || NumberIsEmpty(&arg))
				{
					ErrorCommand(lineNumber, opWithArg[i].argErrorType);
				}
				else
				{
					if((CmpNumberLong(&arg, opWithArg[i].argMinValue) >= 0) && 
						(CmpNumberLong(&arg, opWithArg[i].argMaxValue) <= 0))
					{
						if(PolyStackHasEnoughElements(pStack, opWithArg[i].requiredStackSize(&arg)))
						{
							opWithArg[i].execute(pStack, &arg);
						}
						else
						{
							ErrorCommand(lineNumber, STACK_UNDERFLOW);
						}
					}
					else
					{
						ErrorCommand(lineNumber, opWithArg[i].argErrorType);
					}
				
				}
				NumberDestroy(&arg);
			}
		}
	}
	if(nameFound == false)
	{
		char currChar = getc(stdin);
		ungetc(currChar, stdin);
		if(currChar != '\n' && currChar != EOF)
		{
			ErrorCommand(lineNumber, WRONG_COMMAND);
		}
		else
		{
			for(int i = 0; i < OPER_WITHOUT_ARG_AMOUNT; i++)
			{
				if(WordEquals(&commandName, operation[i].name))
				{
					nameFound = true;
					if(PolyStackHasEnoughElements(pStack, operation[i].requiredStackSize))
					{
						operation[i].execute(pStack);
					}
					else
					{
						ErrorCommand(lineNumber, STACK_UNDERFLOW);
					}
					break;
				}
			}
			if(!nameFound)
			{
				ErrorCommand(lineNumber, WRONG_COMMAND);
			}
		}
	}
	WordDestroy(&commandName);
}

