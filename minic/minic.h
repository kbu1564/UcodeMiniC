#ifndef __MINIC_H
#define __MINIC_H

#define NO_KEYWORDS 7
#define ID_LENGTH   12
#define TEXT_LENGTH 12

enum tsymbol {
  tnull = -1,
  tnot,   tnotequ,    tmod,       tmodAssign, tident,     tnumber,
  tand,   tlparen,    trparen,    tmul,       tmulAssign, tplus,
  tinc,   taddAssign, tcomma,     tminus,     tdec,       tsubAssign,
  tdiv,   tdivAssign, tsemicolon, tless,      tlesse,     tassign,
  tequal, tgreat,     tgreate,    tlbracket,  trbracket,  teof,
  /*                      word symbols                             */
  tconst, telse,      tif,        tint,       treturn,    tvoid,
  twhile, tlbrace,    tor,        trbrace
};

enum tsymbol tnum[NO_KEYWORDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile
};

#endif

