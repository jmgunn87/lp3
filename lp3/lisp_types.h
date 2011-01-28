#ifndef LISP_TYPES_H
#define LISP_TYPES_H

#pragma once
#include "lisp_symtbl.h"

#define IS_LTYPE(a,t)( ((lisp_atom*)a)->type==t )
#define ATOM_CAST(sle)( ((lisp_atom*)sle->_data) )

typedef enum _LISP_TYPE
{
  LTNIL=0,
  LTTRUE=1,
  LTID=8,
  LTINT=9,
  LTFLOAT=10,
  LTSTR=11,
  LTLIST=12,
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
lisp_atom* atom_copy(lisp_atom* atom);
void atom_destroy(lisp_atom* atom);

#define CFN_ARGNOCIEL -1

typedef lisp_atom(*cfnptr)(slist_elem*);

typedef struct _lisp_cfn
{
  char eval_args;//0 = dont eval, 1+ = eval
  int argc_floor;//0 to *
  int argc_ciel;//0 to ARGNOCIEL
  cfnptr its_fnptr;
} lisp_cfn;

lisp_cfn* new_lisp_cfn(char eval,
                       int floor,
                       int ciel,
                       cfnptr fnptr);


typedef struct _lisp_array
{
  LISP_TYPE type;
  unsigned int length;
  void* data;

} lisp_array;

lisp_array* new_lisp_array(LISP_TYPE type,
                           int length,
                           void* data);


#define ALLOC_BOOL()( (char*)malloc(sizeof(char)) )
#define ALLOC_INT()( (int*)malloc(sizeof(int)) )
#define ALLOC_FLOAT()( (float*)malloc(sizeof(float)) )
#define ALLOC_STR(len)( (char*)malloc(sizeof(char)*len+1) )
#define ALLOC_ARRAY(type,count)( (type*)malloc(sizeof(type)*count) )
#define NEW_ATOM_ID(name)( new_atom(LTID,(void*)name) )
#define NEW_ATOM_STR(str)( new_atom(LTSTR,(void*)str) )
#define NEW_ATOM_INT(val)( new_atom(LTINT,(void*)val) )
#define NEW_ATOM_FLOAT(val)( new_atom(LTFLOAT,(void*)val) )
#define NEW_ATOM_LIST(list)( new_atom(LTINT,(void*)list) )
#define NEW_ATOM_LISTFN(list)( new_atom(LTLISPFN,(void*)list) )
#define NEW_ATOM_LISTMACRO(list)( new_atom(LTLISPMACRO,(void*)list) )
#define NEW_ATOM_FNPTR(fnptr)( new_atom(LTLISPMACRO,(void*)fnptr) )

#endif