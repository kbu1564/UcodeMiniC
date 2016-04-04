#ifndef __ASSEMBLE_H
#define __ASSEMBLE_H

class Assemble {
  int instrCnt;
  char lineBuffer[80];
  int bufIndex;
  Label labelProcess;
  char label[LABELSIZE];

  void getLabel();
  int getOpcode();
  int getOperand();
  void instrWrite();
public:
  void assemble();
  int startAddr;
  Assemble() {
    instrCnt = 0;
  }
};

#endif
