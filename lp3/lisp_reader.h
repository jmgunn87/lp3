#ifndef LISP_READER_H
#define LISP_READER_H

#pragma once
#include "lisp_types.h"

#define LPAREN 6
#define RPAREN 7

#ifndef LPTOKENDECL_H
#define LPTOKENDECL_H

typedef struct lp_token
{
  int lex_id;
  void* lex_val;
}lp_token;

#endif

slist* parse_lisp(const char* input,
                  int* input_pos);

#endif