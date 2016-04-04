#include "Ucodei.h"
#include "Label.h"

Label::Label()
{
  int index;

  labelCnt = 2;
  strcpy(labelTable[0].labelName, "read");
  labelTable[0].address = READPROC;
  labelTable[0].instrList = NULL;
  strcpy(labelTable[1].labelName, "write"); /* modified by juyoon */
  labelTable[1].address = WRITEPROC;
  labelTable[1].instrList = NULL;
  strcpy(labelTable[2].labelName, "lf");
  labelTable[2].address = LFPROC;
  labelTable[2].instrList = NULL;
  for (index = 3; index < MAXLABELS; index++)
    labelTable[index].address = UNDEFINED;
}

void Label::insertLabel(char label[], int value)
{
  struct fixUpList* ptr;
  int index;

  for (index = 0; (index <= labelCnt) 
    && strcmp(labelTable[index].labelName, label); index++);

  labelTable[index].address = value;
  if (index > labelCnt) {
    strcpy(labelTable[index].labelName, label);
    labelCnt = index;
    labelTable[index].instrList = NULL;
  } else {
    ptr = labelTable[index].instrList;
    labelTable[index].instrList = NULL;
    while (ptr) { // backpatching
      instrBuf[ptr->instrAddress].value1 = value;
      ptr = ptr->next;
    }
  }
}

void Label::findLabel(char label[], int instr)
{
  struct fixUpList* ptr;
  int index;

  for (index=0; (index <= labelCnt)
    && strcmp(label, labelTable[index].labelName); index++);

  if (index > labelCnt) { // not found
    strcpy(labelTable[index].labelName, label);
    labelCnt = index;
    ptr = new fixUpList;
    if (ptr == NULL) errmsg("findLabel()", "Out of memory -- new");
    labelTable[index].instrList= ptr;
    ptr->instrAddress = instr;
    ptr->next = NULL;
  } else { // found
    ptr = labelTable[index].instrList;
    if (ptr) addFix(ptr, instr);
    else instrBuf[instr].value1=labelTable[index].address;
  }
}

void Label::addFix(struct fixUpList* prev, int instr)
{
  struct fixUpList* succ;

  while (prev->next) prev = prev->next;
  succ = new fixUpList;
  if (succ == NULL) errmsg("addFix()", "Out of memory");
  succ-> instrAddress = instr;
  succ-> next = NULL;
  prev-> next = succ; 
//delete succ;
}

void Label::checkUndefinedLabel()
{
  int index;

  for (index = 0; index <= labelCnt; index++)
    if (labelTable[index].address == UNDEFINED)
      errmsg("undefined label", labelTable[index].labelName);
}
