#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "minic.h"

const char *keyword[NO_KEYWORDS] = {
	"const", "else", "if", "int", "return", "void", "while"
};

const char *symbol[] = {
	"!",  "!=",  "%",  "%=", 0,    0,
	"&&", "(",   ")",  "*",  "*=", "+",
	"++", "+=",  ",",  "-",  "--", "-=",
	"/",  "/=",  ";",  "<",  "<=", "=",
	"==", ">",   ">=", "[",  "]",  0,

	"const", "else", "if", "int", "return", "void",
	"while", "{", "||", "}"
};

struct tockenType {
	int number;
	union {
		char id[ID_LENGTH];
		int num;
	} value;
};

void lexicalError(int n)
{
	printf(" *** Lexical Error : ");
	switch (n) {
		case 1:
			printf("an identifier length must be less than 12.\n");
			break;
		case 2:
			printf("next character must be &.\n");
			break;
		case 3:
			printf("next character must be |.\n");
			break;
		case 4:
			printf("invalid character!!!\n");
			break;
	}
}

int getIntNum(char ch)
{
	return ch - '0';
}

int isalpha(char ch)
{
	if (ch >= 'a' && ch <= 'z') return 1;
	if (ch >= 'A' && ch <= 'Z') return 1;
	return 0;
}

int isalnum(char ch)
{
	if (isalpha(ch)) return 1;
	if (ch >= '0' && ch <= '9') return 1;
	return 0;
}

int superLetter(char ch)
{
	if (isalpha(ch) || ch == '_') return 1;
	return 0;
}

int superLetterOrDigit(char ch)
{
	if (isalnum(ch) || ch == '_') return 1;
	return 0;
}

int isspace(char ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\n') return 1;
	return 0;
}

struct tockenType scanner()
{
	struct tockenType tocken;
	int i, index;
	char ch, id[ID_LENGTH];

	tocken.number = tnull;

	do {
		while (isspace(ch = getchar()));
		if (superLetter(ch)) {
			i = 0;
			do {
				if (i < ID_LENGTH) id[i++] = ch;
				ch = getchar();
			} while (superLetterOrDigit(ch));

			if (i >= ID_LENGTH) lexicalError(1);
			id[i] = '\0';
			ungetc(ch, stdin);

			for (index = 0; index < NO_KEYWORDS; index++)
				if (!strcmp(id, symbol[index+30])) break;

			if (index < NO_KEYWORDS)
				tocken.number = tnum[index];
			else {
				tocken.number = tident;
				strcpy(tocken.value.id, id);
			}
		}
		else if (isdigit(ch)) {
			tocken.number = tnumber;
			tocken.value.num = getIntNum(ch);
		}
		else switch (ch) {
			case '/':
				ch = getchar();
				if (ch == '*')
					do {
						while (ch != '*') ch = getchar();
						ch = getchar();
					} while(ch != '/');
				else if (ch == '/')
					while (getchar() != '\n');
				else if (ch == '=') tocken.number = tdivAssign;
				else {
					tocken.number = tdiv;
					ungetc(ch, stdin);
				}
				break;
			case '!':
				ch = getchar();
				if (ch == '=')
					tocken.number = tnotequ;
				else {
					tocken.number = tnot;
					ungetc(ch, stdin);
				}
				break;
			case '%':
				ch = getchar();
				if (ch == '=')
					tocken.number = tmodAssign;
				else {
					tocken.number = tmod;
					ungetc(ch, stdin);
				}
				break;
			case '&':
				ch = getchar();
				if (ch == '&')
					tocken.number = tand;
				else {
					lexicalError(2);
					ungetc(ch, stdin);
				}
				break;
			case '*':
				ch = getchar();
				if (ch == '=')
					tocken.number = tmulAssign;
				else {
					tocken.number = tmul;
					ungetc(ch, stdin);
				}
				break;
			case '+':
				ch = getchar();
				if (ch == '+')
					tocken.number = tinc;
				else if (ch == '=')
					tocken.number = taddAssign;
				else {
					tocken.number = tplus;
					ungetc(ch, stdin);
				}
				break;
			case '-':
				ch = getchar();
				if (ch == '-')
					tocken.number = tdec;
				else if (ch == '=')
					tocken.number = tsubAssign;
				else {
					tocken.number = tminus;
					ungetc(ch, stdin);
				}
				break;
			case '<':
				ch = getchar();
				if (ch == '=')
					tocken.number = tlesse;
				else {
					tocken.number = tless;
					ungetc(ch, stdin);
				}
				break;
			case '>':
				ch = getchar();
				if (ch == '=')
					tocken.number = tgreate;
				else {
					tocken.number = tgreat;
					ungetc(ch, stdin);
				}
				break;
			case '=':
				ch = getchar();
				if (ch == '=')
					tocken.number = tequal;
				else {
					tocken.number = tassign;
					ungetc(ch, stdin);
				}
				break;
			case '|':
				ch = getchar();
				if (ch == '|')
					tocken.number = tor;
				else {
					lexicalError(3);
					ungetc(ch, stdin);
				}
				break;
			case '(': tocken.number = tlparen;    break;
			case ')': tocken.number = trparen;    break;
			case ',': tocken.number = tcomma;     break;
			case ';': tocken.number = tsemicolon; break;
			case '[': tocken.number = tlbracket;  break;
			case ']': tocken.number = trbracket;  break;
			case '{': tocken.number = tlbrace;    break;
			case '}': tocken.number = trbrace;    break;
			case EOF: tocken.number = teof;       break;
			default:
				printf("Current character : %c", ch);
				lexicalError(4);
				break;
		}
	} while (tocken.number == tnull);

	return tocken;
}

int main()
{
	struct tockenType tn;
	char text[TEXT_LENGTH] = { 0, };

	do {
		tn = scanner();

		switch (tn.number) {
		case tnull:
		case teof:
			continue;
		case tident:
			strcpy(text, tn.value.id);
			break;
		default:
			strcpy(text, symbol[tn.number]);
			break;
		}

		printf("%s\t%d\t", text, tn.number);
		if (tn.number == tident)
			printf("%s\n", tn.value.id);
		else
			printf("%d\n", 0);
	} while (tn.number != teof);
	return 0;
}

