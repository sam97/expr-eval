#include "tempfile.h"

static FILE* tmp;

void mktmp() {
	tmpfile_s(&tmp);
}

int tmpputs(String s) {
	if (!s)
		return;
	if (!tmp)
		mktmp();

	printf("%s", s);
	return fputs(s, tmp);
}

void tmpcopy(FILE* source) {
	int c;

	fflush(tmp);
	rewind(tmp);
	while ((c = fgetc(tmp)) >= 0) {
		fputc(c, source);
	}
}

int tmpclose() {
	return fclose(tmp);
}
