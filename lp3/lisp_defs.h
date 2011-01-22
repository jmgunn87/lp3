#ifndef LISP_DEFS_H
#define LISP_DEFS_H

#pragma once
#include "stdafx.h"
#include "slist.h"

#define SYMTBL_BUCKETS 32
#define LOCAL_SYM_BUCKETS 25
#define SIZE_BITS ( sizeof(size_t)*CHAR_BIT )
#define IS_LTYPE(a,t)( ((lisp_atom*)a)->type==t )
#define ATOM_CAST(sle)( ((lisp_atom*)sle->_data) )
#define PUSH_STACK_RESULT(id,val)( slist_push(result_stack,(void*)new_atom(id,(void*)val)) )
#define PEEK_STACK_RESULT(count)( ((list_atom*)slist_peekn(result_stack,count)) )
#define POP_STACK_RESULT(count)( ((list_atom*)slist_pop(result_stack)) )

typedef enum _LISP_TYPE
{
  LTID=8,
  LTINT=9,
  LTFLOAT=10,
  LTSTR=11,
  LTLIST=12,
  LTBOOLEAN=13,
  LTLISPFN=14,
  LTLISPMACRO=15,
  LTARRAY=16,
  LTCFNPTR=17,
  
} LISP_TYPE;

typedef struct _lisp_atom
{
  LISP_TYPE type;
  void* data;

} lisp_atom; 

lisp_atom* new_atom(LISP_TYPE type, void* data);

#define ALLOC_BOOLEAN()( (bool*)malloc(sizeof(bool)) )
#define ALLOC_INT()( (int*)malloc(sizeof(int)) )
#define ALLOC_FLOAT()( (float*)malloc(sizeof(float)) )
#define ALLOC_STR(len)( (char*)malloc(sizeof(char)*len+1) )
#define ALLOC_ARRAY(type,count)( (type*)malloc(sizeof(type)*count) )
#define NEW_ATOM_ID(name)( new_atom(LTID,(void*)name) )
#define NEW_ATOM_STR(str)( new_atom(LTSTR,(void*)str) )
#define NEW_ATOM_BOOLEAN(tf)( new_atom(LTBOOLEAN,(void*)tf) )
#define NEW_ATOM_INT(val)( new_atom(LTINT,(void*)val) )
#define NEW_ATOM_FLOAT(val)( new_atom(LTFLOAT,(void*)val) )
#define NEW_ATOM_LIST(list)( new_atom(LTINT,(void*)list) )
#define NEW_ATOM_LISTFN(list)( new_atom(LTLISPFN,(void*)list) )
#define NEW_ATOM_LISTMACRO(list)( new_atom(LTLISPMACRO,(void*)list) )
#define NEW_ATOM_FNPTR(fnptr)( new_atom(LTLISPMACRO,(void*)fnptr) )

char lisp_install_symbol(const char* name,
                         void* data,
                         char is_local);
char lisp_uninstall_symbol(const char* name,
                           char is_local);
char lisp_get_symbol(const char* name,
                     void** ret_data,
                     char is_local);


#endif