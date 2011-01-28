#pragma once
#include "stdafx.h"
#include "lisp_symtbl.h"

slist* local_symtbl[LOCAL_SYM_BUCKETS];
slist* lisp_symtbl[SYMTBL_BUCKETS];

typedef struct _ht_entry
{
  const char* name;
  void* data;
} ht_entry;

static unsigned int hash_symbol(const char* sym,
                                int tbl_size)
{
  const char *s;
  unsigned int h=0;
  for(s=sym;*s;s++)
    h=*s+((h<<9)|(h>>(SIZE_BITS-9)));
  return h%tbl_size;
}

char lisp_install_symbol(const char* name,
                         void* data,
                         char is_local)
{
  slist** symtbl=is_local?local_symtbl:lisp_symtbl;
  int symtbl_size=is_local?LOCAL_SYM_BUCKETS:SYMTBL_BUCKETS;
  ht_entry* he=0;
  unsigned long index=0;
  int slen=0;

  if(!name||!symtbl||lisp_get_symbol(name,0,is_local)==1)
    return 0;

  index=hash_symbol(name,symtbl_size);
  if(!lisp_symtbl[index])
    lisp_symtbl[index]=new_slist();

  he=(ht_entry*)malloc(sizeof(ht_entry));
  he->name=name;
  he->data=data;
  slist_append(symtbl[index],he);
  return 1;
}

char lisp_uninstall_symbol(const char* name,
                           char is_local)
{
  ht_entry* he=0;
  slist** symtbl=is_local?local_symtbl:lisp_symtbl;
  int symtbl_size=is_local?LOCAL_SYM_BUCKETS:SYMTBL_BUCKETS;
  slist_elem* sle=0;
  unsigned long index=0;

  if(!name)
    return 0;

  index=hash_symbol(name,symtbl_size);
  if(!symtbl[index])
    return 0;
  else
  {
    sle=symtbl[index]->_head;
    while(sle)
    {
      he=(ht_entry*)sle->_data;
      if(strcmp(he->name,name)==0)
      {
        (*he).name=0;
        (*he).data=0;
        return 1;
      }
      sle=sle->_next;
    }
  }
  return 0;
}

char lisp_get_symbol(const char* name,
                     void** ret_data,
                     char is_local)
{
  slist** symtbl=is_local?local_symtbl:lisp_symtbl;
  int symtbl_size=is_local?LOCAL_SYM_BUCKETS:SYMTBL_BUCKETS;
  ht_entry* he=0;
  slist_elem* sle=0;
  unsigned long index=0;

  if(!name)
    return 0;

  index=hash_symbol(name,symtbl_size);
  if(!symtbl[index])
    return 0;
  else
  {
    sle=symtbl[index]->_head;
    while(sle)
    {
      he=(ht_entry*)sle->_data;
      if(he&&he->name&&strcmp(he->name,name)==0)
      {
        if(ret_data)
          *ret_data=he->data;
        return 1;
      }
      sle=sle->_next;
    }
  }
  return 0;
}
