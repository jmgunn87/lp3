#ifndef LISP_BASE_H
#define LISP_BASE_H

#pragma once
#include "lisp_reader.h"

extern slist* result_stack;
extern slist* local_symtbl[LOCAL_SYM_BUCKETS];
extern slist* lisp_symtbl[SYMTBL_BUCKETS];

void load_lisplib();

#endif