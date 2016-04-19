%{
  /* lex source for Mini C */
  #include <stdio.h>
  #include <stdlib.h>
  #include "minic.h"
%}
%%
"const"   return(tconst);

"int"     return(tint);
"void"    return(tvoid);

"if"      return(tif);
"else"    return(telse);
"return"  return(treturn);
"while"   return(twhile);

"!"       return(tnot);
"&"       return(tand);
"|"       return(tor);
"*"       return(tmul);
"%"       return(tmod);
"/"       return(tdiv);
"+"       return(tplus);
"-"       return(tminus);

","       return(tcomma);
";"       return(tsemicolon);

"="       return(tassign);
"<"       return(tless);
">"       return(tgreat);

"=="      return(tequal);
"!="      return(tnotequ);
"<="      return(tlesse);
">="      return(tgreate);
"&&"      return(tand);
"||"      return(tor);
"++"      return(tinc);
"--"      return(tdec);
"+="      return(taddAssign);
"-="      return(tsubAssign);
"*="      return(tmulAssign);
"/="      return(tdivAssign);
"%="      return(tmodAssign);

"("       return(tlparen);
")"       return(trparen);
"["       return(tlbracket);
"]"       return(trbracket);
"{"       return(tlbrace);
"}"       return(trbrace);

[a-zA-Z_][a-zA-Z0-9_]*                      return(tident);
[1-9][0-9]*|0([0-7]+|(x|X)[0-9a-fA-F]*)?    return(tnumber);
"/*"([^*]|\*+[^*/])*\**"*/"   ;
"//".*                        ;
[ \t\n]                       ;
.         return(yytext[0]);
%%
int yywrap() {
  return 1;
}

int main() {
  enum tsymbol tn;
  while ((tn = yylex())) {
    if (tn == teof) break;

    printf("%s\t%d\t", yytext, tn);
		if (tn == tident) printf("%s\n", yytext);
		else printf("%d\n", 0);
  }
  return 0;
}

