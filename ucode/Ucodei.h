#ifndef __UCODEI_H
#define __UCODEI_H

/* modified by juyoon, from here... */
/*
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
*/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <locale>

using namespace std;

/* ... to here, to relect new standard of c++ */

const int MAXINSTR   = 2000;
const int MAXLABELS  = 500;
const int STACKSIZE  = 2000;
const int LABELSIZE  = 10;
const int NO_OPCODES = 41;             // 36 + 5

enum { FALSE, TRUE };
enum procIndex { READPROC = -1, WRITEPROC = -2, LFPROC = -3, UNDEFINED = -1000 };
enum opcode {
	notop, neg,   incop, decop, dupl,  swp, add,  sub,   mult, divop,
	modop, andop, orop,  gt,    lt,   ge,  le,   eq,    ne,
	lod,   ldc,   lda,   ldi,   ldp,  str, sti,  ujp,   tjp,  fjp,
	call, ret,   retv,  chkh,  chkl, nop, proc, endop, bgn,  sym,
	dump,  none
};

typedef struct {
     int opcode;
     int value1;
     int value2;
     int value3;
} Instruction;

extern const char*    opcodeName[NO_OPCODES];
extern const int      executable[NO_OPCODES];
extern const int      opcodeCycle[NO_OPCODES];
extern       long int dynamicCnt[NO_OPCODES];
extern       int      staticCnt[NO_OPCODES];

extern Instruction instrBuf[MAXINSTR];
extern ifstream inputFile;
extern ofstream outputFile;

void errmsg(const char* s, const char* s2);
#endif
