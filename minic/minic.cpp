#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "minic.h"

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
				if (!strcmp(id, keyword[index])) break;
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
			default:
				printf("Current character : %c", ch);
				lexicalError(4);
				break;
		}
	} while (tocken.number == tnull);

	return tocken;
}
