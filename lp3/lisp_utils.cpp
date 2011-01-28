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
void load_utils()
{
  lisp_install_symbol("sleep",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,1,1,lp_sleep)),0);
}