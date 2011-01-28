#ifndef LISP_SYMTBL_H
#define LISP_SYMTBL_H

#pragma once
#include "slist.h"

#define SYMTBL_BUCKETS 32
#define LOCAL_SYM_BUCKETS 25
#define SIZE_BITS ( sizeof(size_t)*CHAR_BIT )

extern slist* local_symtbl[LOCAL_SYM_BUCKETS];
extern slist* lisp_symtbl[SYMTBL_BUCKETS];

/* symbol table functions */
char lisp_install_symbol(const char* name,
                         void* data,
                         char is_local);
char lisp_uninstall_symbol(const char* name,
                           char is_local);
char lisp_get_symbol(const char* name,
                     void** ret_data,
                     char is_local);

#endif