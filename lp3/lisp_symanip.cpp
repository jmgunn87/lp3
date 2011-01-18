#pragma once 
#include "stdafx.h"
#include "lisp_base.h"

static int lp_defun_or_macro(slist_elem* next,
                             char is_fn)
{
  char* sym=0;
  slist* def_list=0;

  if(!next)
    return 0;

  def_list=new_slist();

  /* get the symbol name */
  if(!IS_LTYPE(next->_data,LTID))
    return 0;

  sym=(char*)malloc(strlen((const char*)ATOM_CAST(next)->data)+1);
  sprintf((char*)sym,"%s\0",(const char*)ATOM_CAST(next)->data);
  if(!sym)
    return 0;
  else slist_append(def_list,(void*)sym);

  /* get params */
  if(!next->_next||
     !next->_next->_data||
     !IS_LTYPE(next->_next->_data,LTLIST))
    return 0;
  slist_append(def_list,(void*)ATOM_CAST(next->_next)->data);

  /* get body */
  if(!next->_next->_next||
     !next->_next->_next->_data||
     !IS_LTYPE(next->_next->_next->_data,LTLIST))
    return 0;
  slist_append(def_list,(void*)ATOM_CAST(next->_next->_next)->data);

  /* install macro or fn */
  if(lisp_install_symbol(sym,new_atom(is_fn?LTLISPFN:LTLISPMACRO,(void*)def_list),0))
    return 3;
  return 0;
}
static int lp_defun(slist_elem* next)
{
  return lp_defun_or_macro(next,1);
}
static int lp_defmacro(slist_elem* next)
{
  return lp_defun_or_macro(next,0);
}
static int lp_defvar(slist_elem* next)
{
  if(!next)
    return 0;
  if(IS_LTYPE(next->_data,LTID)&&next->_next)
    if(lisp_install_symbol((const char*)ATOM_CAST(next)->data,next->_next->_data,0))
      return 2;
  return 0;
}

static int lp_undef(slist_elem* next)
{
  if(!next)
    return 0;
  if(IS_LTYPE(next->_data,LTID))
    if(lisp_uninstall_symbol((const char*)ATOM_CAST(next)->data,0))
      return 1;
  return 0;
}
static int lp_setf(slist_elem* next)
{
  lisp_atom* atom=0;

  if(!next)
    return 0;

  /* try and retrieve the symbol */
  if(IS_LTYPE(next->_data,LTID)&&next->_next)
    if(!lisp_install_symbol((const char*)ATOM_CAST(next)->data,ATOM_CAST(next->_next),0))
      if(lisp_get_symbol((const char*)ATOM_CAST(next)->data,(void**)&atom,0))
      {
        atom=ATOM_CAST(next->_next);
        return 2;
      }
      else return 0;
    else return 2;
  return 0;
}
static int lp_let(slist_elem* next){return 0;}
static int lp_defp(slist_elem* next)
{
  int* res=(int*)malloc(sizeof(int));
  *res=1;

  if(!next)
  {
    *res=0;
    slist_push(result_stack,(void*)new_atom(LTINT,(void*)res));
    return 0;
  }

  if(ATOM_CAST(next)->type!=LTID||
     !lisp_get_symbol((const char*)ATOM_CAST(next)->data,0,0)&&
     !lisp_get_symbol((const char*)ATOM_CAST(next)->data,0,1))
  {
    *res=0;
    slist_push(result_stack,(void*)new_atom(LTINT,(void*)res));
  }
  else
    slist_push(result_stack,(void*)new_atom(LTINT,res));

  return 1;
}

void load_symanip()
{
  lisp_install_symbol("defp",(void*)new_atom(LTCFNPTR,(void*)&lp_defp),0);
  lisp_install_symbol("defmacro",(void*)new_atom(LTCFNPTR,(void*)&lp_defmacro),0);
  lisp_install_symbol("defun",(void*)new_atom(LTCFNPTR,(void*)&lp_defun),0);
  lisp_install_symbol("defvar",(void*)new_atom(LTCFNPTR,(void*)&lp_defvar),0);
  lisp_install_symbol("undef",(void*)new_atom(LTCFNPTR,(void*)&lp_undef),0);
  lisp_install_symbol("setf",(void*)new_atom(LTCFNPTR,(void*)&lp_setf),0);
  lisp_install_symbol("let",(void*)new_atom(LTCFNPTR,(void*)&lp_let),0);
}