#include "Ucodei.h"
#include "Label.h"
#include "UcodeiStack.h"
#include "ArithmeticLogicUnit.h"
#include "Interpret.h"

Interpret::Interpret() : sysStack(STACKSIZE), memStack(STACKSIZE)
{
  arBase = 4;
  tcycle = 0;
  exeCount = 0 ;
}

int Interpret::findAddr(int n)
{
  int temp;

  if (!instrBuf[n].value1)
    errmsg("findAddr()", "Lexical level is zero ...");
  else if (instrBuf[n].value2 < 1) 
    errmsg("findAddr()", "Negative offset ...");

  for (temp = arBase; instrBuf[n].value1 != sysStack[temp+3]; temp = sysStack[temp]) {
    if ((temp > STACKSIZE) || (temp < 0 ))
      cout << "Lexical level :  " << instrBuf[n].value1 << ' ' 
           << "Offset        :  " << instrBuf[n].value2 << '\n';
  }

  return (temp + instrBuf[n].value2 + 3);
}

void Interpret::predefinedProc(int procIndex)
{
  int data, temp;

  // read
  if (procIndex == READPROC) {
    cin >> data;
    temp = memStack.pop();
    sysStack[temp] = data;
  } else if (procIndex == WRITEPROC) { // write
    temp = memStack.pop();
    cout << ' ' << temp;
    outputFile << ' ' << temp;
  } else if (procIndex == LFPROC) { // lf : line feed
    outputFile.put('\n');
    cout << "\n";
  }
}

void Interpret::statistic()
{
  int i, opcode;

  outputFile << "\n\n   **********************\n\n";
  outputFile << "\n\n\n                 " << "#### Statistics ####\n";
  outputFile << "\n\n    ****  Static Instruction Counts  ****\n\n\n";
  for (i=0, opcode = notop ; opcode <= none; opcode++) {
    if ( staticCnt[opcode] != 0) {
      outputFile.width(5);
      outputFile.setf(ios::left, ios::adjustfield);
      outputFile << opcodeName[opcode] <<  "  =  " ;
      outputFile.width(5);
      outputFile.setf(ios::left, ios::adjustfield);
      outputFile <<  staticCnt[opcode] << "   ";
      i++;
      if (i % 4 == 0) outputFile.put('\n');
    }
  }
  outputFile << "\n\n\n  ****  Dynamic instruction counts  ****\n\n\n";
  for (i=0, opcode = notop ; opcode <= none; opcode++) {
    if (dynamicCnt[opcode] != 0) {
      outputFile.width(5);
      outputFile.setf(ios::left, ios::adjustfield);
      outputFile << opcodeName[opcode] << "  =  " ;
      outputFile.width(8);
      outputFile.setf(ios::left, ios::adjustfield);
      outputFile << dynamicCnt[opcode] << "  " ;
      i++;
      if ( i % 4 == 0) outputFile << "\n";
    }
  }
  outputFile << "\n\n Executable instruction count  =   " << exeCount;
  outputFile << "\n\n Total execution cycle         =   " << tcycle;
  outputFile << "\n";
}

void Interpret::execute(int startAddr)
{
  int parms;
  int temp, temp1;
  int pc;
  // int returnAddress;

  pc = startAddr;
  cout << " == Executing ...  ==\n";
  cout << " == Result         ==\n";
  while (pc >= 0) {
    // Instruction Register
    Instruction ir = instrBuf[pc];

    // 명령어당 사용량
    // 디버깅용 총 사이클수(소모된 클럭수) 측정용
    dynamicCnt[ir.opcode]++;
    if (executable[ir.opcode]) exeCount++;
    tcycle = opcodeCycle[ir.opcode];

    //cout << opcodeName[ir.opcode] << "  = " << memStack.top() << endl;
    // instruction execution
    switch(ir.opcode) {
      case call:
        if ((temp = ir.value1) < 0) {
          // call assembly functions
          predefinedProc(temp);
          // jump return address
          //memStack.restore();
          //pc = memStack.pop();
        } else {
          // return base address
          memStack.push(pc + 1);
          //memStack.store();
          // 이 부분이 실제로 함수 코드로 점프하는 부분
          pc = ir.value1 - 1;
          arBase = sysStack.top() + 1;
        }
        break;
      case proc:
        // value 1: block number(base)
        // value 2: static level => lexical level (static chain)
        // 함수에서 사용 할 메모리 공간 확보
        sysStack.spSet(arBase + 3);
        // 함수에서 사용하는 메모리 공간의 고유 키값 셋팅 : block number
        // 만약 이 값이 서로 같은 함수가 존재한다면 서로의 공간을 공유한다.
        sysStack[arBase+3] = ir.value1;
        break;
      case retv:
        temp = memStack.pop();
      case ret:
        //memStack.restore();
        pc = memStack.pop();
        if (ir.opcode == retv)
          memStack.push(temp);
        break;
      case sym:
        // 함수에서 사용할 메모리 공간 확보
        sysStack.spSet(sysStack.top() + ir.value2);
        //ir.value1 // block number
        //ir.value2 // size
        break;

      // Arithmetic Logical Unit
      case add:
      case sub:
      case divop:
      case modop:
      case mult:
      case andop:
      case orop:
        alu.setReg2(memStack.pop());
        alu.setReg1(memStack.pop());
        memStack.push(alu.execute(ir.opcode));
        break;
      case neg:
      case notop:
      case incop:
      case decop:
        alu.setReg1(memStack.pop());
        memStack.push(alu.execute(ir.opcode));
        break;

      case lod:
        memStack.push(sysStack[findAddr(pc)]);
        break;
      case lda:
        memStack.push(findAddr(pc));
        break;
      case str:
        sysStack[findAddr(pc)] = memStack.pop();
        break;
      case ldi:
        if ((memStack.top() <= 0) || (memStack.top() > STACKSIZE))
          errmsg("execute()", "Illegal ldi instruction ...");
        temp = memStack.pop();
        memStack.push(sysStack[temp]);
        break;
      case sti:
        temp = memStack.pop();
        sysStack[memStack.pop()] = temp;
        break;

      case ldc:
        memStack.push(ir.value1);
        break;
      case gt:
        temp = memStack.pop();
        memStack.push(memStack.pop()>temp);
        break;
      case lt:
        temp = memStack.pop();
        memStack.push(memStack.pop()<temp);
        break;
      case ge:
        temp = memStack.pop();
        memStack.push(memStack.pop()>=temp);
        break;
      case le:
        temp = memStack.pop();
        memStack.push(memStack.pop()<=temp);
        break;
      case eq:
        temp = memStack.pop();
        memStack.push(memStack.pop()==temp);
        break;
      case ne:
        temp = memStack.pop();
        memStack.push(memStack.pop()!=temp);
        break;
      case ujp:
        pc = ir.value1 - 1;
        break;
      case tjp:
        if (memStack.pop()) pc = ir.value1 - 1;
        break;
      case fjp:
        if (!memStack.pop()) pc = ir.value1 - 1;
        break;

      case endop:
        pc = -2;
        break;
      case bgn:
        sysStack.spSet(sysStack.top() + ir.value1);
        break;
      case nop:
        break;
    }
    pc++;
  }
  cout << '\n';
  statistic();
}