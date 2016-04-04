/* modified by juyoon, from here... */
/*
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
*/

#include "Ucodei.h"
#include "Label.h"
#include "UcodeiStack.h"
#include "Assemble.h"
#include "Interpret.h"

const char* opcodeName[NO_OPCODES] = {
	"notop", "neg",  "inc", "dec", "dup", "swp", "add",   
	"sub",   "mult", "div", "mod", "and", "or",  "gt",   
	"lt",    "ge",   "le",  "eq",  "ne",  "lod", "ldc",   
	"lda",   "ldi",  "ldp", "str", "sti", "ujp", "tjp", 
	"fjp",   "call", "ret", "retv","chkh","chkl","nop", 
	"proc",  "end",  "bgn", "sym", "dump","none"
};

const int executable[NO_OPCODES] = {
	/* notop */ 1, /* neg */  1, /* inc */  1, /* dec */  1, /* dup */  1,  
	/* swp */   1, /* add */  1, /* sub */  1, /* mult */ 1, /* div */  1,
	/* mod */   1, /* and */  1, /* or */   1, /* gt */   1, /* lt */   1,  
	/* ge */    1, /* le */   1, /* eq */   1, /* ne */   1, /* lod */  1,  
	/* ldc */   1, /* lda */  1, /* ldi */  1, /* ldp */  1, /* str */  1, 
	/* sti */   1, /* ujp */  1, /* tjp */  1, /* fjp */  1, /* call */ 1, 
	/* ret */   1, /* retv */ 1, /* chkh */ 1, /* chkl */ 1, /* nop */  0, 
	/* proc */  1, /* end */  0, /* bgn */  0, /* sym */  0, /* dump */ 1,  
	/* none */  0
};

const int opcodeCycle[NO_OPCODES] = {
	/* notop */ 5, /* neg */   5, /* inc */  1, /* dec */   1, /* dup */    5,  
	/* swp */  10, /* add */  10, /* sub */ 10, /* mult */ 50, /* div */  100,
	/* mod */ 100, /* and */  10, /* or */  10, /* gt */   20, /* lt */    20, 
	/* ge */   20, /* le */   20, /* eq */  20, /* ne */   20, /* lod */    5,
	/* ldc */   5, /* lda */   5, /* ldi */ 10, /* ldp */  10, /* str */    5, 
	/* sti */  10, /* ujp */  10, /* tjp */ 10, /* fjp */  10, /* call */  30, 
	/* ret */  30, /* retv */ 30, /* chkh */ 5, /* chkl */  5, /* nop */    0, 
	/* proc */ 30, /* end */   0, /* bgn */  0, /* sym */   0, /* dump */ 100,  
	/* none */  0
};

long int dynamicCnt[NO_OPCODES] /* = {
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,
	 1
} */ ;

int staticCnt[NO_OPCODES] /* = {
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	 0,0,0,
	 1
} */ ;

ifstream inputFile;
ofstream outputFile;
Instruction instrBuf[MAXINSTR];

void errmsg(const char* s, const char* s2 = "")
{
     cerr << "error !!!  " << s << ":  " << s2 << "\n";
     exit(1);
}

int main(int argc, char* argv[])
{
  Assemble sourceProgram;
  Interpret binaryProgram;

  if (argc != 2) errmsg("main()", "Wrong number of arguments");

  inputFile.open(argv[1], ios::in);
  if (!inputFile) errmsg("cannot open input file", argv[1]);

  outputFile.open(strcat(strtok(argv[1], "."), ".lst"), ios::out);
  // if (!outputFile) errmsg("cannot open output file", argv[2]) ;

  sourceProgram.assemble();
  binaryProgram.execute(sourceProgram.startAddr);

  inputFile.close();
  outputFile.close();

  return 0;
}
