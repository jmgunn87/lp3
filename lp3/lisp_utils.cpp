#pragma once
#include "stdafx.h"
#include "lisp_utils.h"
#include "lisp_types.h"
#include <windows.h>

static lisp_atom lp_sleep(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;

  if(ATOM_CAST(next)->type!=LTINT)
  {
    ret.type=LTNIL;
    ret.data=(void*)LTNIL;
    return ret;
  }

  Sleep((DWORD)*(int*)ATOM_CAST(next)->data);
  return ret;
}
static lisp_atom lp_length(slist_elem* next)
{
  int length=0;
  lisp_atom ret;
  ret.type=LTINT;
  ret.data=(void*)ALLOC_INT();
 
  switch(ATOM_CAST(next)->type)
  {
  case LTTRUE:
  case LTINT:
  case LTFLOAT:
    length=1;
    break;
  case LTSTR:
    length=strlen((char*)ATOM_CAST(next)->data);
    break;
  case LTLIST:
    length=((slist*)ATOM_CAST(next)->data)->_size;
    break;
  case LTARRAY:
    length=((lisp_array*)ATOM_CAST(next)->data)->length;
    break;
  default:
    length=0;
  }
  *(int*)ret.data=length;
  return ret;
}
void load_utils()
{
  lisp_install_symbol("sleep",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,1,1,lp_sleep)),0);
  lisp_install_symbol("length",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,1,1,lp_length)),0);
}