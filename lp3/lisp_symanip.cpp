#pragma once 
#include "stdafx.h"
#include "lisp_symanip.h"
#include "lisp_types.h"
#include "lisp_eval.h"

static lisp_atom lp_defun_or_macro(slist_elem* next,
                                   char is_fn)
{
  char* sym=0;
  slist* def_list=new_slist();
  lisp_atom ret;
  ret.type=LTNIL;
  
  /* get the symbol name */
  if(!IS_LTYPE(next->_data,LTID))
    return ret;

  /* copy and store the sym name*/
  sym=(char*)malloc(strlen((const char*)ATOM_CAST(next)->data)+1);
  sprintf((char*)sym,"%s\0",(const char*)ATOM_CAST(next)->data);
  if(!sym)
    return ret;
  else slist_append(def_list,(void*)sym);

  /* get params */
  if(!next->_next||
     !next->_next->_data||
     !IS_LTYPE(next->_next->_data,LTLIST))
    return ret;
  slist_append(def_list,(void*)ATOM_CAST(next->_next)->data);

  /* get body */
  if(!next->_next->_next||
     !next->_next->_next->_data||
     !IS_LTYPE(next->_next->_next->_data,LTLIST))
    return ret;
  slist_append(def_list,(void*)ATOM_CAST(next->_next->_next)->data);

  /* install macro or fn */
  if(!lisp_install_symbol(sym,new_atom(is_fn?LTLISPFN:LTLISPMACRO,(void*)def_list),0))
    return ret;

  ret.type=LTID;
  ret.data=(void*)sym;
  return ret;
}
static lisp_atom lp_defun(slist_elem* next){return lp_defun_or_macro(next,1);}
static lisp_atom lp_defmacro(slist_elem* next){return lp_defun_or_macro(next,0);}

static lisp_atom lp_defvar(slist_elem* next)
{
  lisp_atom atom;
  atom.type=LTNIL;
  if(IS_LTYPE(next->_data,LTID)&&next->_next)
    if(lisp_install_symbol((const char*)ATOM_CAST(next)->data,next->_next->_data,0))
      return *ATOM_CAST(next);
  return atom;
}
static lisp_atom lp_setf(slist_elem* next)
{
  lisp_atom* atom=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  /* try and retrieve the symbol */
  if(IS_LTYPE(next->_data,LTID)&&next->_next)
    if(!lisp_install_symbol((const char*)ATOM_CAST(next)->data,ATOM_CAST(next->_next),0))
      if(lisp_get_symbol((const char*)ATOM_CAST(next)->data,(void**)&atom,0))
      {
        ret=lisp_eval(ATOM_CAST(next->_next),0);
        (*atom).type=ret.type;
        (*atom).data=ret.data;
        return *atom;
      }
  return ret;
}
static lisp_atom lp_exists(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;

  while(next)
  {
    if(ATOM_CAST(next)->type!=LTID||
       !lisp_get_symbol((const char*)ATOM_CAST(next)->data,0,0)&&
       !lisp_get_symbol((const char*)ATOM_CAST(next)->data,0,1))
    {
      ret.type=LTNIL;
      ret.data=(void*)LTNIL;
      return ret;
    } 
    next=next->_next;
  }
  return ret;
}
static lisp_atom lp_undefine(slist_elem* next)
{
  lisp_atom* atom=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  while(next)
  {
    /* try and retrieve the symbol */
    if(IS_LTYPE(next->_data,LTID))
    {
      if(lisp_get_symbol((const char*)ATOM_CAST(next)->data,(void**)&atom,0)||
         lisp_get_symbol((const char*)ATOM_CAST(next)->data,(void**)&atom,1))
      {
        if(lisp_uninstall_symbol((const char*)ATOM_CAST(next)->data,0)||
           lisp_uninstall_symbol((const char*)ATOM_CAST(next)->data,0))
        {
          atom_destroy(atom);
        }
        else return ret;
      }else return ret;
    }else return ret;
    next=next->_next;
  }

  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;
  return ret;
}
static lisp_atom lp_destroy(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;

  while(next)
  {
    atom_destroy(ATOM_CAST(next));
    next=next->_next;
  }
  return ret;
}
void load_symanip()
{
  //remember
  lisp_install_symbol("destroy",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,1,CFN_ARGNOCIEL,&lp_destroy)),0);
  lisp_install_symbol("forget",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,1,CFN_ARGNOCIEL,&lp_undefine)),0);
  lisp_install_symbol("exists",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,1,CFN_ARGNOCIEL,&lp_exists)),0);
  lisp_install_symbol("macro",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,3,3,&lp_defmacro)),0);
  lisp_install_symbol("process",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,3,3,&lp_defun)),0);
  lisp_install_symbol("variable",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,2,2,&lp_defvar)),0);
  lisp_install_symbol("is",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(0,2,2,lp_setf)),0);
//  lisp_install_symbol("let",(void*)new_atom(LTCFNPTR,(void*)&lp_let),0);
}