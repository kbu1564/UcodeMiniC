#ifndef __UCODEI_STACK_H
#define __UCODEI_STACK_H

class UcodeiStack {
     int size;
     int sp;
     int bp;
     int* stackArray;
public:
     void push(int) ;
     int pop() ;
     int top() { return sp; }
     void spSet(int n) { sp = n; }
     void store() { bp = sp; }
     void restore() { sp = bp; }
     void dump();
     int& operator[](int);
     UcodeiStack(int) ; 
     ~UcodeiStack() { delete[] stackArray; }
}; 

#endif
