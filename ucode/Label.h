#ifndef __LABEL_H
#define __LABEL_H

class Label {
  struct fixUpList {
    int instrAddress;
    struct fixUpList* next;
  };
  struct labelEntry {
    char labelName[LABELSIZE];
    int address;
    struct fixUpList* instrList;
  };
  struct labelEntry labelTable[MAXLABELS];
  int labelCnt;
  void addFix(struct fixUpList*, int);
public:
  void insertLabel(char [], int);
  void findLabel(char [], int);
  void checkUndefinedLabel();
  Label();
  virtual ~Label() { }
};

#endif

