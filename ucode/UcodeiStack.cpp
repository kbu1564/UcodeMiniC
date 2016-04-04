#include "Ucodei.h"
#include "UcodeiStack.h"

UcodeiStack::UcodeiStack(int size)
{
     stackArray = new int[size];
     sp = -1;
     push(-1); push(-1); push(-1); push(0);
     push(0);   push(0); push(-1); push(1);
}

void UcodeiStack::push(int value)
{
     if (sp == STACKSIZE) errmsg("push()", "Stack Overflow...");
     stackArray[++sp] = value;
}

int UcodeiStack::pop()
{
     if (sp == 0) errmsg("pop()", "Stack Underflow...");
     return stackArray[sp--];
}

void UcodeiStack::dump()
{
	int i;

	cout << "stack dump (address : value)\n";
	for ( i = sp-10; i <=sp; ++i)
		cout << ' ' << i << " : " << stackArray[i] << "\n";
	cout << '\n';
}

int& UcodeiStack::operator[](int index)
{
     return stackArray[index];
}
