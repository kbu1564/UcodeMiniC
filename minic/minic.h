#ifndef __MINIC_H
#define __MINIC_H

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

#endif

