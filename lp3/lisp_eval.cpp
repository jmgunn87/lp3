#pragma once
#include "stdafx.h"
#include "lisp_eval.h"


static lisp_atom lisp_eval_fun_or_macro(lisp_atom* atom,
                                        slist_elem* next,
                                        char is_fn)
{
  slist_elem* sle=0;
  slist_elem* params=0;
  slist_elem* body=0;

  /* zip through the fn list to params*/
  sle=atom->data?((slist*)atom->data)->_head:0;
  if(!sle)return *atom;
  params=((slist*)sle->_next->_data)->_head;

  /* load param list */
  if(params)
    while(params&&next)
    {
      if(lisp_install_symbol((const char*)ATOM_CAST(params)->data,
                             next->_data,0))
        ;
      params=params->_next;
      next=next->_next;
    }

  /* now eval the body */
  body=((slist*)sle->_next->_next->_data)->_head;
  if(!body)return *atom;
  return lisp_eval(ATOM_CAST(body),body->_next);
}

lisp_atom lisp_eval(lisp_atom* atom,
                    slist_elem* next)
{
  int argcount=0;
  lisp_cfn* lcfn=0;
  lisp_atom ret_atom;
  slist_elem* start=next;

  /* init the return atom */
  ret_atom.type=LTNIL;
  ret_atom.data=0;

  while(atom&&atom->data)
  {
    /* check the eval type */
    if(atom->eval_type==LEQUOTE)
    {
      ret_atom=*atom;
      return ret_atom;
    }
    else if(atom->eval_type==LEBQUOTE)
    {
      ret_atom=*atom;
      return ret_atom;
    }

    switch(atom->type)
    {
    case LTID:
      ret_atom=*atom;
      if(lisp_get_symbol((char*)atom->data,(void**)&atom,0))
        continue;
      else return ret_atom;/* just return the symbol */
    case LTLIST:
      start=((slist*)atom->data)->_head;
      if(start)
        return lisp_eval((lisp_atom*)start->_data,start->_next);
      else return ret_atom;
    case LTLISPFN:
      start=next;
      while(next)/* eval arguments before a call */
      {
        *ATOM_CAST(next)=lisp_eval(ATOM_CAST(next),next->_next);
        next=next->_next;
      }
      return lisp_eval_fun_or_macro(atom,start,1);
    //case LTLISPMACRO:
    //  start=next;
    //  while(next)/* eval arguments 
    //                before a call */
    //  {
    //    *ATOM_CAST(next)=lisp_eval(ATOM_CAST(next),next->_next);
    //    next=next->_next;
    //  }
    //  return lisp_eval_fun_or_macro(atom,start,0);
    case LTCFNPTR:
      lcfn=(lisp_cfn*)atom->data;
      start=next;
      while(next)/* eval arguments before a call if specified */
      {
        if(lcfn->eval_args)
          *ATOM_CAST(next)=lisp_eval(ATOM_CAST(next),next->_next);
        next=next->_next;
        ++argcount;
      }
      /* now validate that we have the right number of args*/
      if(argcount>=lcfn->argc_floor)
        if(lcfn->argc_ciel==CFN_ARGNOCIEL||
           argcount<=lcfn->argc_ciel)
          return lcfn->its_fnptr(start);
      return ret_atom;
    default:
      ret_atom=*atom;
      return ret_atom;
    }
  }
  return ret_atom;
}

