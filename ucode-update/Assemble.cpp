#include "Ucodei.h"
#include "Label.h"
#include "Assemble.h"

void Assemble::getLabel()
{
  int i;

  while (isspace(lineBuffer[bufIndex])) bufIndex++;
  for (i=0; i<=LABELSIZE && !isspace(label[i]=lineBuffer[bufIndex]);
    bufIndex++, i++);
  label[i] = '\0';
}

int Assemble::getOpcode()
{
  char mnemonic[5];
  int index;

  // always start at 12-th column

  bufIndex = 11;
  index = 0;
  while (index < 5 && !isspace(lineBuffer[bufIndex])) {
    mnemonic[index++] = lineBuffer[bufIndex++]; 
  }
  mnemonic[index] = '\0';

  for(index=notop; index < none; index++)
    if (!strcmp(mnemonic, opcodeName[index])) break;

  if (index == none) errmsg("Illegal opcode", mnemonic);
  return index;
}

int Assemble::getOperand()
{
  int result;

  while (isspace(lineBuffer[bufIndex])) bufIndex++;
  result= 0;
  while (isdigit(lineBuffer[bufIndex]) && lineBuffer[bufIndex] != '\n')
    result = 10 * result + (lineBuffer[bufIndex++] - '0');
  return result;
}

void Assemble::instrWrite()
{
  int i, j;
  char ch;

  inputFile.seekg(0, ios::beg);

  outputFile << "\n line       object           ucode  source program\n\n";
  for (i=1; i<=instrCnt; i++) {
    outputFile.width(4);
    outputFile << i << "    (";
    outputFile.width(2);
    outputFile << instrBuf[i].opcode;
    j = instrBuf[i].opcode;
    if (j == chkl || j == chkh || j==ldc   || j == bgn ||
        j == ujp  || j == call  || j == fjp || j == tjp) {
      outputFile.width(5);
      outputFile << instrBuf[i].value1 << "     ";
    } else if (j==lod || j==str || j==lda || j==sym || j==proc) {
      outputFile.width(5);
      outputFile << instrBuf[i].value1 ;
      outputFile.width(5);
      outputFile << instrBuf[i].value2 ;
    } else {
      outputFile << "          ";
    }
    outputFile << ")     ";

    while ((ch=inputFile.get()) != '\n') // copy input to output
      outputFile.put(ch);
    outputFile.put('\n');
  }
  outputFile << "\n\n\n   ****    Result    ****\n\n\n";
}

void Assemble::assemble()
{
  int done = FALSE;
  int end = FALSE;
  int n;

  cout << " == Assembling ... ==" << '\n';
  while (!inputFile.eof() && !inputFile.fail() && !end) {
    instrCnt++;
    bufIndex = 0;
    inputFile.getline(lineBuffer, sizeof(lineBuffer));
    if (!isspace(lineBuffer[0])) {
      getLabel();
      labelProcess.insertLabel(label, instrCnt);
    }
    instrBuf[instrCnt].opcode = n = getOpcode();
    staticCnt[n]++;
    switch (n) {
    case chkl:
    case chkh:
    case ldc :
      instrBuf[instrCnt].value1 = getOperand();
      break;
    case lod:
    case str:
    case lda:
    case sym:
      instrBuf[instrCnt].value1 = getOperand();
      instrBuf[instrCnt].value2 = getOperand();
      break;
    case proc:
      instrBuf[instrCnt].value1 = getOperand();
      instrBuf[instrCnt].value2 = getOperand();
      break;
    case bgn:
      instrBuf[instrCnt].value1 = getOperand();
      startAddr = instrCnt;
      done = TRUE;
      break;
    case ujp:
    case call:
    case fjp:
    case tjp:
      getLabel();
      labelProcess.findLabel(label, instrCnt);
      break;
    case endop:
      if (done) end = TRUE;
      break;
    }
  }
  instrWrite();
}