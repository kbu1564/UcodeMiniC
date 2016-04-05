#include "Ucodei.h"
#include "Label.h"
#include "ArithmeticLogicUnit.h"

int ArithmeticLogicUnit::execute(int opcode) {
  // reg1 : Accumulator register
  switch (opcode) {
    case add:
      reg1 = reg1 + reg2;
      break;
    case sub:
      reg1 = reg1 - reg2;
      break;
    case divop:
      if (reg2 == 0) errmsg("ALU", "Divide by Zero ...");
      reg1 = reg1 / reg2;
      break;
    case modop:
      reg1 = reg1 % reg2;
      break;
    case mult:
      reg1 = reg1 * reg2;
      break;

    case andop:
      reg1 = reg1 & reg2;
      break;
    case orop:
      reg1 = reg1 | reg2;
      break;
    case incop:
      reg1++;
      break;
    case decop:
      reg1--;
      break;
    case neg:
      reg1 = -reg1;
      break;
    case notop:
      reg1 = !reg1;
      break;
  }
  return reg1;
}

