#pragma once 
#include "stdafx.h"
#include "lisp_access.h"
#include "lisp_base.h"

static int lp_at(slist_elem* next)
{
  int index=0;
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
      printf("at:%c\n",((char*)ATOM_CAST(next)->data)[index]);
      break;
    case LTLIST:
      la=(lisp_atom*)slist_at((slist*)ATOM_CAST(next)->data,index+1);
      if(!la)
        return 2;
      printf("at:%d\n",*(int*)la->data);
    default:
      break;
  }
  return 2;
}

void load_access()
{
  lisp_install_symbol("at",(void*)new_atom(LTCFNPTR,(void*)&lp_at),0);
}
