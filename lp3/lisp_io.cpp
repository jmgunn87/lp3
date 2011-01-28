#pragma once
#include "stdafx.h"
#include "lisp_io.h"
#include "lisp_types.h"

static FILE* lp_stream=0;

static lisp_atom lp_echo(slist_elem* next)
{
  lisp_atom ret;
  lisp_atom* atom=0;
  slist_elem* sle=0;

  if(!lp_stream)
    lp_stream=stdout;

  while(next)
  {
    atom=ATOM_CAST(next);
    switch(atom->type)
    {
      case LTID:
        if(lisp_get_symbol((const char*)atom->data,(void**)&atom,0))
          continue;
        else fprintf(lp_stream,"%s",atom->data);
        break;
      case LTSTR:
        fprintf(lp_stream,"%s",atom->data);
        break;
      case LTINT:
        fprintf(lp_stream,"%d",*(int*)atom->data);
        break;
      case LTFLOAT:
        fprintf(lp_stream,"%f",*(float*)atom->data);
        break;
      case LTLIST:
      //  lisp_eval(ATOM_CAST(next),0);
      //  printf("%d\n",PEEK_STACK_RESULT(0));
        break;
      case LTARRAY:
        break;
      case LTLISPFN:
        //fprintf(stdout,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTLISPMACRO:
        //fprintf(stdout,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTCFNPTR:
        fprintf(lp_stream,"0x%X",atom->data);
      default:
        break;
    }
    next=next->_next;
  }
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;
  return ret;
}

static lisp_atom lp_set_stream(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  if(IS_LTYPE(next->_data,LTSTR)&&
     IS_LTYPE(next->_next->_data,LTSTR))
  {
    /* if we already have an active stream, close it*/
    if(lp_stream)
      fclose(lp_stream);

    lp_stream=fopen((const char*)ATOM_CAST(next)->data,
                     (const char*)ATOM_CAST(next->_next)->data);
    if(lp_stream)
    {
      ret.type=LTTRUE;
      ret.data=(void*)LTTRUE;
      return ret;
    }
  }
  return ret;
}
static lisp_atom lp_close_stream(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;
  if(lp_stream)
  {
    ret.type=LTTRUE;
    ret.data=(void*)LTTRUE;
    fclose(lp_stream);
  }
  return ret;
}

void load_io()
{
  lisp_install_symbol("echo",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(ARG_ZEROTOMANY,lp_echo)),0);
  lisp_install_symbol("set-stream",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(2,lp_set_stream)),0);
  lisp_install_symbol("close-stream",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,lp_close_stream)),0);
}