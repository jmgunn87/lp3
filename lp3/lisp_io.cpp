#pragma once
#include "stdafx.h"
#include "lisp_io.h"
#include "lisp_base.h"

static int lp_echo(slist_elem* next)
{
  char* id=0;
  int* swallowed=ALLOC_INT();

  if(!next)
    return 0;
  while(next)
  {
    switch(ATOM_CAST(next)->type)
    {
      case LTID:
        id=(char*)ATOM_CAST(next)->data;
        if(lisp_get_symbol((const char*)id,&next->_data,0))
          continue;
        else fprintf(stdout,"%s",id);
        break;
      case LTSTR:fprintf(stdout,"%s",ATOM_CAST(next)->data);
        break;
      case LTINT:fprintf(stdout,"%d",*(int*)ATOM_CAST(next)->data);
        break;
      case LTFLOAT:fprintf(stdout,"%f",*(float*)ATOM_CAST(next)->data);
        break;
      case LTLISPFN:fprintf(stdout,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTLISPMACRO:fprintf(stdout,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTCFNPTR:fprintf(stdout,"0x%X",ATOM_CAST(next)->data);
      default:
        break;
    }
    next=next->_next;
    ++*swallowed;
  }
  PUSH_STACK_RESULT(LTINT,swallowed);
  return *swallowed;
}
static int lp_set_stream(slist_elem* next)
{
  if(IS_LTYPE(next->_data,LTSTR))
    if(next->_next&&IS_LTYPE(next->_next->_data,LTSTR))
    {
      freopen((const char*)ATOM_CAST(next)->data,
              (const char*)ATOM_CAST(next->_next)->data,
              stdout);
      return 2;
    }
  return 0;
}
static int lp_close_stream(slist_elem* next)
{
  fclose(stdout);
  return 0;
}

void load_io()
{
  lisp_install_symbol("echo",(void*)new_atom(LTCFNPTR,(void*)&lp_echo),0);
  lisp_install_symbol("set-stream",(void*)new_atom(LTCFNPTR,(void*)&lp_set_stream),0);
  lisp_install_symbol("close-stream",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),0);
}