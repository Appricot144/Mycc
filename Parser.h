#ifndef ___parser_h
#define ___parser_h

#include"Token.h"

extern Token token;
extern int errF;

void expression(void);

void term(void);

void factor(void);

void chkTkn(Kind op);

#endif
