#ifndef TEMPFILE_INC_GUARD
#define TEMPFILE_INC_GUARD

#include <stdio.h>
#include "helperlib.h"

void mktmp();
int tmpputs(String s);
void tmpcopy(FILE *source);
int tmpclose();

#endif
