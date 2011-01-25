#pragma once 
#include "stdafx.h"
#include "lisp_access.h"
#include "lisp_base.h"
#include "lisp_array.h"

static int lp_at(slist_elem* next)
{
  int index=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;

  if(!next||!next->_data||
    !next->_next||!next->_next->_data||
    !ATOM_CAST(next)->data||
    !ATOM_CAST(next->_next)->data||
    ATOM_CAST(next->_next)->type!=LTINT)
    return 0;
 
  index=*(int*)ATOM_CAST(next->_next)->data;
  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      if(strlen((char*)ATOM_CAST(next)->data)<index)
        PUSH_STACK_RESULT(LTSTR,&((char*)ATOM_CAST(next)->data)[index]);
      break;
    case LTLIST:
      la=(lisp_atom*)slist_at((slist*)ATOM_CAST(next)->data,index+1);
      if(la)
        PUSH_STACK_RESULT(la->type,la->data);
      break;
    case LTARRAY:
      tarr=(lisp_array*)la->data;
      if(tarr->type!=LTARRAY)
        PUSH_STACK_RESULT(tarr->type,array_at(tarr,index));
      else
      {
        next=next->_next;
        tarr=(lisp_array*)array_at(tarr,index);
        while(tarr&&tarr->type==LTARRAY)
        {
          next=next->_next;
          if(next&&ATOM_CAST(next)->type==LTINT)
            index=*(int*)ATOM_CAST(next)->data;
          else break;
          tarr=(lisp_array*)array_at(tarr,index);
        }
        if(tarr)
          PUSH_STACK_RESULT(tarr->type,array_at(tarr,index));
      }
      break;
    default:
      break;
  }
  return 2;
}

void load_access()
{
  lisp_install_symbol("at",(void*)new_atom(LTCFNPTR,(void*)&lp_at),0);
}
