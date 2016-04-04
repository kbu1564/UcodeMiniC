#include "Ucodei.h"
#include "Label.h"
#include "UcodeiStack.h"
#include "Interpret.h"

Interpret::Interpret() : stack(STACKSIZE)
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

  for (temp = arBase; instrBuf[n].value1 != stack[temp+3]; temp = stack[temp]) {
    if ((temp > STACKSIZE) || (temp < 0 ))
    cout << "Lexical level :  " << instrBuf[n].value1 << ' ' 
         << "Offset        :  " << instrBuf[n].value2 << '\n';
  }

  return (temp + instrBuf[n].value2 + 3);
}

void Interpret::predefinedProc(int procIndex)
{
  static ifstream dataFile;
  static int readFirst = TRUE;

  // char dataFileName[20];
  int data, temp;

  // read
  if (procIndex == READPROC) {
    /*
    if (readFirst) {
      cout << "\nEnter Data File Name : ";
      cin >> dataFileName;
      dataFile.open(dataFileName, ios::in);
      if (dataFileName == NULL)
        errmsg("checkPredefined()", "Data file open error ...");
      readFirst = FALSE;
    }
    dataFile >> data;
    */
    cin >> data;
    temp = stack.pop();
    stack[temp] = data;
    stack.spSet(stack.top()-4);
  } else if (procIndex == WRITEPROC) { // write
    temp = stack.pop();
    cout << ' ' << temp;
    outputFile << ' ' << temp ;
    stack.spSet(stack.top()-4);
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
    dynamicCnt[instrBuf[pc].opcode]++;
    if (executable[instrBuf[pc].opcode]) exeCount++;
    tcycle = opcodeCycle[instrBuf[pc].opcode];
    switch(instrBuf[pc].opcode) {
      case notop:
        stack.push(!stack.pop());
        break;
      case neg:
        stack.push(-stack.pop());
        break;
      case add:
        stack.push(stack.pop()+stack.pop());
        break;
      case divop:
        temp = stack.pop();
        if (temp == 0) errmsg("execute()", "Divide by Zero ...");
        stack.push(stack.pop()/temp);
        break;
      case sub:
        temp = stack.pop();
        stack.push(stack.pop()-temp);
        break;
      case mult:
        stack.push(stack.pop()*stack.pop());
        break;
      case modop:
        temp = stack.pop();
        stack.push(stack.pop()%temp);
        break;
      case andop:
        stack.push(stack.pop() & stack.pop());
        break;
      case orop:
        stack.push(stack.pop() | stack.pop());
        break;
      case gt:
        temp = stack.pop();
        stack.push(stack.pop()>temp);
        break;
      case lt:
        temp = stack.pop();
        stack.push(stack.pop()<temp);
        break;
      case ge:
        temp = stack.pop();
        stack.push(stack.pop()>=temp);
        break;
      case le:
        temp = stack.pop();
        stack.push(stack.pop()<=temp);
        break;
      case eq:
        temp = stack.pop();
        stack.push(stack.pop()==temp);
        break;
      case ne:
        temp = stack.pop();
        stack.push(stack.pop()!=temp);
        break;
      case swp:
        temp = stack.pop();
        temp1 = stack.pop();
        stack.push(temp);
        stack.push(temp1);
        break;
      case lod:
        stack.push(stack[findAddr(pc)]);
        break;
      case ldc:
        stack.push(instrBuf[pc].value1);
        break;
      case lda:
        stack.push(findAddr(pc));
        break;
      case str:
        stack[findAddr(pc)] = stack.pop();
        break;
      case ldi:
        if ((stack.top() <= 0) || (stack.top() > STACKSIZE))
          errmsg("execute()", "Illegal ldi instruction ...");
        temp = stack.pop();
        stack.push(temp);
        stack[stack.top()] = stack[temp];
        break;
      case sti:
        temp = stack.pop();
        stack[stack.pop()] = temp;
        break;
      case ujp:
        pc = instrBuf[pc].value1 - 1;
        break;
      case tjp:
        if (stack.pop()) pc = instrBuf[pc].value1 - 1;
        break;
      case fjp:
        if (!stack.pop()) pc = instrBuf[pc].value1 - 1;
        break;
      case chkh:
        temp = stack.pop();
        if (temp > instrBuf[pc].value1)
          errmsg("execute()", "High check failed...");
        stack.push(temp);
        break;
      case chkl:
        temp = stack.pop();
        if (temp < instrBuf[pc].value1)
          errmsg("execute()", "Low check failed...");
        stack.push(temp);
        break;
      case ldp:
        // 함수 정보 저장용 공간 3칸 확보
        parms = stack.top() + 1;    // save sp
        stack.spSet(stack.top()+4); // set a frame
        break;
      case call:
        if ((temp=instrBuf[pc].value1) < 0) predefinedProc(temp);
        else {
          // param : func parameter stack pointer
          // set up param value in `ldp` inst
          stack[parms+2] = pc + 1; // return base address

          // linked list 방식으로 관리중인 before pointer 셋팅
          stack[parms+1] = arBase; // func stack information
          arBase = parms; // next func call index

          // 이 부분이 실제로 함수 코드로 점프하는 부분
          pc = instrBuf[pc].value1 - 1; // pc = func code address
        }
        break;
      case retv:
        temp = stack.pop();
      case ret:
        stack.spSet(arBase - 1);
        if (instrBuf[pc].opcode == retv)
          stack.push(temp);
        pc = stack[arBase+2] - 1;
        arBase = stack[arBase+1];
        break;
      /*
      case stp:
        returnAddress = stack[arBase+2] - 1;
        stack.spSet(arBase - 1);
        arBase = stack[arBase  1];
        break;
      case ret:
        pc = returnAddress;
        break;
      */
      case proc:
        // value 1: (size of paras  size of local vars)
        // value 2: block number(base)
        // value 3: static level => lexical level (static chain)
        // 함수에서 사용 할 메모리 공간 확보
        stack.spSet(arBase + instrBuf[pc].value1 + 3);
        // 함수에서 사용하는 메모리 공간의 고유 키값 셋팅 : block number
        // 만약 이 값이 서로 같은 함수가 존재한다면 서로의 공간을 공유한다.
        stack[arBase+3] = instrBuf[pc].value2;

        // lexical level : innerBlock 을 처리하기위한 용도로 현재 쓰이진 않음
        for (temp = stack[arBase+1];
             stack[temp+3] != instrBuf[pc].value3 -1;
             temp = stack[temp]);
        // innerBlock 을 처리하기 위한 부모 Block Address
        // 이 또한 내부적으로 linked list 자료구조를 사용
        stack[arBase] = temp;
        break;
      case endop:
        pc = -2;
        break;
      case bgn:
        stack.spSet(stack.top() + instrBuf[pc].value1);
        break;
      /*
      case ones:
        temp = stack.pop();
        stack.push(~temp);
        break;
      */
      case nop:
      case sym:
        break;
      /* augmented operation code */
      case incop:
        temp = stack.pop();
        stack.push(++temp);
        break;
      case decop:
        temp = stack.pop();
        stack.push(--temp);
        break;
      case dupl:
        temp = stack.pop();
        stack.push(temp);
        stack.push(temp);
        break;
      case dump:
        stack.dump();
        break;
      }
    pc++;
  }
  cout << '\n';
  statistic();
}