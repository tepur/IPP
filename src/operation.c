/** @file
   Operacje na kalkulatorze wielomianów
	
   @author Michał Tepper <mt386430@students.mimuw.edu.pl>, Jakub Pawlewicz <pan@mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-06-03
*/
#include "operation.h"

#include "utils.h"

/// @private
void Execute2ArgArithmeticOper(Poly (*op)(const Poly *, const Poly *), PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	Poly top2 = PolyStackNextAfterTop(pStack);
	Poly opRes = op(&top, &top2);
	PolyStackPop(pStack);
	PolyStackPop(pStack);
	PolyStackPush(pStack, &opRes);
}
/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem – wypisuje 
 * na standardowe wyjście 0 lub 1
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void IsCoeffExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	printf("%d\n", PolyIsCoeff(&top));
}
/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru – wypisuje 
 * na standardowe wyjście 0 lub 1
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void IsZeroExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	printf("%d\n", PolyIsZero(&top)); 
}
/**
 * Wstawia na stos kopię wielomianu z wierzchołka
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void CloneExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	Poly p2 = PolyClone(&top);
	PolyStackPush(pStack, &p2);
}
/**
 * Dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void AddExecute(PolyStack *pStack)
{
	Execute2ArgArithmeticOper(PolyAdd, pStack);
}
/**
 * Mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void MulExecute(PolyStack *pStack)
{
	Execute2ArgArithmeticOper(PolyMul, pStack);
}
/**
 * Neguje wielomian na wierzchołku stosu
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void NegExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	Poly neg = PolyNeg(&top);
	PolyStackPop(pStack);
	PolyStackPush(pStack, &neg);
}
/**
 * Odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia 
 * na wierzchołek stosu różnicę
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void SubExecute(PolyStack *pStack)
{
	Execute2ArgArithmeticOper(PolySub, pStack);
}
/**
 * Sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje 
 * na standardowe wyjście 0 lub 1
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void IsEqExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	Poly top2 = PolyStackNextAfterTop(pStack);
	printf("%d\n", PolyIsEq(&top, &top2));
}
/**
 * Wypisuje na standardowe wyjście stopień wielomianu (−1 dla wielomianu 
 * tożsamościowo równego zeru)
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void DegExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	printf("%d\n", PolyDeg(&top));
}
/**
 * Usuwa wielomian z wierzchołka stosu
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void PopExecute(PolyStack *pStack)
{
	PolyStackPop(pStack);
}
/**
 * Wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void ZeroExecute(PolyStack *pStack)
{
	Poly p = PolyZero();
	PolyStackPush(pStack, &p);
}
/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu w formacie akceptowanym przez parser
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 */
void PrintExecute(PolyStack *pStack)
{
	Poly top = PolyStackTop(pStack);
	PrintPoly(&top);
	printf("\n");
}
/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze @p arg 
 * (−1 dla wielomianu tożsamościowo równego zeru)
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 * @param[in] arg : numer zmiennej
 */
void DegByExecute(PolyStack *pStack, Number *arg)
{
	Poly top = PolyStackTop(pStack);
	printf("%d\n", PolyDegBy(&top, (unsigned)NumberToLong(arg)));
}
/**
 * Wylicza wartość wielomianu w punkcie @p arg, usuwa wielomian z wierzchołka 
 * i wstawia na stos wynik operacji
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 * @param[in] arg : punkt, w którym wyliczana jest wartość wielomianu
 */
void AtExecute(PolyStack *pStack, Number *arg)
{
	Poly top = PolyStackTop(pStack);
	Poly at = PolyAt(&top, (poly_coeff_t)NumberToLong(arg));
	PolyStackPop(pStack);
	PolyStackPush(pStack, &at);
}
/**
 * Zdejmuje z wierzchołka stosu najpierw wielomian p, a potem kolejno wielomiany 
 * x[0], x[1], …, x[@p arg - 1] i umieszcza na stosie wynik funkcji PolyCompose.
 * @param[in] pStack : stos, na którym wykonywana jest operacja
 * @param[in] arg : rozmiar tablicy x - ilość zdjętych wielomianów (nie licząc p)
 */
void ComposeExecute(PolyStack *pStack, Number *arg)
{
	Poly top_tmp = PolyStackTop(pStack);
	Poly top = PolyClone(&top_tmp); /* PolyStackPop usuwa cały wielomian, 
		więc potrzebujemy jego głębokiej kopii */
	PolyStackPop(pStack);
	unsigned count = (unsigned)NumberToLong(arg);
	Poly *x = malloc(sizeof(Poly) * count);
	assert(x != NULL);
	for(unsigned i = 0; i < count; i++)
	{
		Poly tmp = PolyStackTop(pStack);
		x[i] = PolyClone(&tmp);
		PolyStackPop(pStack);
	}
	Poly composed = PolyCompose(&top, count, x);
	for(unsigned i = 0; i < count; i++)
	{
		PolyDestroy(&(x[i]));
	}
	PolyDestroy(&top);
	free(x);
	PolyStackPush(pStack, &composed);
}
/// @private
long ConstantRequiredStackSize(Number *arg){
	return 1;
}
/// @private
long ArgDependentRequiredStackSize(Number *arg){
	return (long)NumberToLong(arg) + 1;
}

void InitStandardOperations(Operation operation[], OperationWithArg opWithArg[])
{
	operation[0].name = ZERO;
	operation[0].requiredStackSize = 0;
	operation[0].execute = ZeroExecute;
	
	operation[1].name = IS_COEFF;
	operation[1].requiredStackSize = 1;
	operation[1].execute = IsCoeffExecute;
	
	operation[2].name = IS_ZERO;
	operation[2].requiredStackSize = 1;
	operation[2].execute = IsZeroExecute;
	
	operation[3].name = CLONE;
	operation[3].requiredStackSize = 1;
	operation[3].execute = CloneExecute;
	
	operation[4].name = ADD;
	operation[4].requiredStackSize = 2;
	operation[4].execute = AddExecute;
	
	operation[5].name = MUL;
	operation[5].requiredStackSize = 2;
	operation[5].execute = MulExecute;
	
	operation[6].name = NEG;
	operation[6].requiredStackSize = 1;
	operation[6].execute = NegExecute;
	
	operation[7].name = SUB;
	operation[7].requiredStackSize = 2;
	operation[7].execute = SubExecute;
	
	operation[8].name = IS_EQ;
	operation[8].requiredStackSize = 2;
	operation[8].execute = IsEqExecute;
	
	operation[9].name = DEG;
	operation[9].requiredStackSize = 1;
	operation[9].execute = DegExecute;
	
	operation[10].name = PRINT;
	operation[10].requiredStackSize = 1;
	operation[10].execute = PrintExecute;
	
	operation[11].name = POP;
	operation[11].requiredStackSize = 1;
	operation[11].execute = PopExecute;
	
	opWithArg[0].name = DEG_BY;
	opWithArg[0].requiredStackSize = ConstantRequiredStackSize;
	opWithArg[0].execute = DegByExecute;
	opWithArg[0].argErrorType = WRONG_VARIABLE;
	opWithArg[0].argMinValue = 0;
	opWithArg[0].argMaxValue = UINT_MAX;
	
	opWithArg[1].name = AT;
	opWithArg[1].requiredStackSize = ConstantRequiredStackSize;
	opWithArg[1].execute = AtExecute;
	opWithArg[1].argErrorType = WRONG_VALUE;
	opWithArg[1].argMinValue = LONG_MIN;
	opWithArg[1].argMaxValue = LONG_MAX;
	
	opWithArg[2].name = COMPOSE;
	opWithArg[2].requiredStackSize = ArgDependentRequiredStackSize;
	opWithArg[2].execute = ComposeExecute;
	opWithArg[2].argErrorType = WRONG_COUNT;
	opWithArg[2].argMinValue = 0;
	opWithArg[2].argMaxValue = UINT_MAX;
}
