#ifndef __INTERPRET_H
#define __INTERPRET_H

class Interpret {
  ArithmeticLogicUnit alu;
  Instruction ir;

  UcodeiStack callStack;
  UcodeiStack sysStack;
  UcodeiStack memStack;
  int arBase;
  long int tcycle;
  long int exeCount;

  void predefinedProc(int);
  int findAddr(int);
  void statistic();
public:
  void execute(int);
  Interpret();
  virtual ~Interpret() { }
};

#endif
