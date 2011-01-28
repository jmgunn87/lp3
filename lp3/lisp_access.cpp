#pragma once 
#include "stdafx.h"
#include "lisp_access.h"
#include "lisp_types.h"
#include "lisp_array.h"

static lisp_atom lp_at(slist_elem* next)
{
  int index=0;
  char* str=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  if(ATOM_CAST(next->_next)->type!=LTINT)
    return ret;
  index=*(int*)ATOM_CAST(next->_next)->data;

  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      str=(char*)ATOM_CAST(next)->data;
      if(strlen(str)>=index)
      {
        ret.type=LTSTR;
        ret.data=(void*)ALLOC_STR(2);
        memcpy(ret.data,(const void*)&((char*)ATOM_CAST(next)->data)[index],1);
        ((char*)ret.data)[1]='\0';
        return ret;
      }
      return ret;
    case LTLIST:
      la=(lisp_atom*)slist_at((slist*)ATOM_CAST(next)->data,index+1);
      if(la)
      {
        ret.type=la->type;
        ret.data=la->data;
        return ret;
      }
      return ret;
    case LTARRAY:
      //tarr=(lisp_array*)la->data;
      //if(tarr->type!=LTARRAY)
      //  PUSH_STACK_RESULT(tarr->type,array_at(tarr,index));
      //else
      //{
      //  next=next->_next;
      //  tarr=(lisp_array*)array_at(tarr,index);
      //  while(tarr&&tarr->type==LTARRAY)
      //  {
      //    next=next->_next;
      //    if(next&&ATOM_CAST(next)->type==LTINT)
      //      index=*(int*)ATOM_CAST(next)->data;
      //    else break;
      //    tarr=(lisp_array*)array_at(tarr,index);
      //  }
      //  if(tarr)
      //    PUSH_STACK_RESULT(tarr->type,array_at(tarr,index));
      //}
      break;
    default:
      break;
  }
  return ret;
}

void load_access()
{
  lisp_install_symbol("at",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,2,2,lp_at)),0);
}
