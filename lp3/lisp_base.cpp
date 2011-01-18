#pragma once 
#include "stdafx.h"
#include "lisp_base.h"
#include "lisp_reader.h"
#include "lisp_math.h"
#include "lisp_symanip.h"

static char _is_loaded=0;

static int lp_echo(slist_elem* next)
{
  char* id=0;
  int swallowed=0;

  if(!next)
    return 0;
  while(next)
  {
    switch(ATOM_CAST(next)->type)
    {
      case LTID:
        id=(char*)ATOM_CAST(next)->data;
        if(lisp_get_symbol((const char*)id,&next->_data,0))
        {
          continue;
        }
        else fprintf(stderr,"%s",id);
        break;
      case LTSTR:
        fprintf(stderr,"%s",ATOM_CAST(next)->data);
        break;
      case LTINT:
        fprintf(stderr,"%d",*(int*)ATOM_CAST(next)->data);
        break;
      case LTFLOAT:
        fprintf(stderr,"%f",*(float*)ATOM_CAST(next)->data);
        break;
      case LTLISPFN:
        fprintf(stderr,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTLISPMACRO:
        fprintf(stderr,"%s",(char*)((slist*)ATOM_CAST(next)->data)->_head->_data);
        break;
      case LTCFNPTR:
        fprintf(stderr,"0x%X",ATOM_CAST(next)->data);
      default:
        break;
    }
    next=next->_next;
    ++swallowed;
  }
  return swallowed;
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

static int lp_if(slist_elem* next)
{
  lisp_atom* atom=0;

  if(!next||!next->_next||!next->_next->_next)
    return 0;

  lisp_eval(ATOM_CAST(next),0);
  atom=(lisp_atom*)slist_pop(result_stack);
  if(!atom)
  {
    lisp_eval(ATOM_CAST(next->_next->_next),0);
    return 3;
  }
  while(atom)
    switch(atom->type)
    {
      case LTID:
        if(lisp_get_symbol((const char*)atom->data,(void**)&atom,0))
          continue;
        else
        {
          lisp_eval(ATOM_CAST(next->_next->_next),0);
          return 3;
        }
      case LTFLOAT:
        if(!atom->data||*(float*)atom->data<1)
          lisp_eval(ATOM_CAST(next->_next->_next),0);
        else lisp_eval(ATOM_CAST(next->_next),0);
        return 3;
      case LTINT:
        if(!atom->data||*(int*)atom->data<1)
          lisp_eval(ATOM_CAST(next->_next->_next),0);
        else lisp_eval(ATOM_CAST(next->_next),0);
        return 3;
      case LTSTR:
      case LTLISPFN:
      case LTLISPMACRO:
        if(!atom->data)
          lisp_eval(ATOM_CAST(next->_next->_next),0);
        else lisp_eval(ATOM_CAST(next->_next),0);
        return 3;
      case LTLIST:
        if(!atom->data||((slist*)atom->data)->_size<1)
          lisp_eval(ATOM_CAST(next->_next->_next),0);
        else lisp_eval(ATOM_CAST(next->_next),0);
        return 3;
      default:
        lisp_eval(ATOM_CAST(next->_next->_next),0);
        return 3;
    }
  return 3;
}

static int lp_do(slist_elem* next)
{

}

void load_lisplib()
{
  if(_is_loaded)
    return;

  load_symanip();
  load_math_module();

  lisp_install_symbol("echo",(void*)new_atom(LTCFNPTR,(void*)&lp_echo),0);
  //lisp_install_symbol("echo-format",(void*)new_atom(LTCFNPTR,(void*)&lp_echo),lisp_symtbl,SYMTBL_BUCKETS);
  lisp_install_symbol("open",(void*)new_atom(LTCFNPTR,(void*)&lp_set_stream),0);
  lisp_install_symbol("close",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),0);
  lisp_install_symbol("if",(void*)new_atom(LTCFNPTR,(void*)&lp_if),0);
  //lisp_install_symbol("do",(void*)new_atom(LTCFNPTR,(void*)&lp_if),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol("==",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol("~=",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol("<",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol(">",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol("<=",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);
  //lisp_install_symbol(">=",(void*)new_atom(LTCFNPTR,(void*)&lp_close_stream),lisp_symtbl,SYMTBL_BUCKETS);

  _is_loaded=1;
}