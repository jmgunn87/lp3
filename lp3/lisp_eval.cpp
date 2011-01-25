#pragma once
#include "stdafx.h"
#include "lisp_eval.h"
#include "lisp_base.h"

static int lisp_eval_list(lisp_atom* atom,
                          slist_elem* next)
{
  slist_elem* sle=0;
  int list_position=0;

  if(!atom||!atom->data)
    return 0;

  sle=((slist*)atom->data)->_head;
  while(sle)
  {
    list_position=lisp_eval((lisp_atom*)sle->_data,sle->_next);
    while(sle&&list_position!=-1)
    {
      sle=sle->_next;
      --list_position;
    }        
  }
  return 0;
}

static int lisp_eval_fun_or_macro(lisp_atom* atom,
                                  slist_elem* next,
                                  char is_fn)
{
  slist_elem* sle=0;
  slist_elem* params=0;
  slist_elem* body=0;
  int list_position=0;
  if(!atom)
    return 0;

  /* zip through the fn list to params*/
  sle=atom->data?((slist*)atom->data)->_head:0;
  if(!sle)return 0;
  params=((slist*)sle->_next->_data)->_head;

  /* load param list */
  if(params)
    while(params&&next)
    {
      if(lisp_install_symbol((const char*)ATOM_CAST(params)->data,next->_data,0))
        ;
      params=params->_next;
      next=next->_next;
      ++list_position;
    }

  /* now eval the body */
  body=((slist*)sle->_next->_next->_data)->_head;
  if(!body)
    return 0;
  lisp_eval(ATOM_CAST(body),body->_next);

  return list_position;
}

int lisp_eval(lisp_atom* atom,
              slist_elem* next)
{
  int list_position=0;
  slist_elem* sle=0;
  slist_elem* sle2=0;
  lisp_atom* id_atom=0;

  if(!atom)
    return 0;
  while(atom&&atom->data)
    switch(atom->type)
    {
    case LTID:
      if(lisp_get_symbol((char*)atom->data,(void**)&atom,0))
        continue;
      else return 0;
    case LTLIST:
      return lisp_eval_list(atom,next);
    case LTLISPFN:
      return lisp_eval_fun_or_macro(atom,next,1);
    case LTLISPMACRO:
      return lisp_eval_fun_or_macro(atom,next,0);
    case LTCFNPTR:
      return ((int(*)(slist_elem*))atom->data)(next);
    case LTSTR:
    case LTINT:
    case LTFLOAT:
    case LTBOOLEAN:
      slist_push(result_stack,(void*)atom);
      return 0;
    default:
      return 0;
    }return 0;
}

