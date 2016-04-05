#ifndef __ARITHMETICLOGICUNIT_H
#define __ARITHMETICLOGICUNIT_H

class ArithmeticLogicUnit {
  int reg1;
  int reg2;
public:
  ArithmeticLogicUnit() : reg1(0), reg2(0) {}
  void setReg1(int value) { reg1 = value; }
  void setReg2(int value) { reg2 = value; }
  int  getReg1() { return reg1; }
  int  getReg2() { return reg2; }
  int  execute(int opcode);
};

#endif
