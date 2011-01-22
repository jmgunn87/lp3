#pragma once 
#include "stdafx.h"
#include "lisp_flow.h"
#include "lisp_base.h"


static int lp_if(slist_elem* next)
{
  int *res=ALLOC_INT();
  *res=0;
  lisp_atom* atom=0;

  /*************************************
   * make sure we have at least 3 args *
   *************************************/
  if(!next||!next->_next||!next->_next->_next)
  {
    PUSH_STACK_RESULT(LTINT,res);
    return 0;
  }

  /**************************************
   * evaluate the condition and execute *
   * the correct branch                 *
   **************************************/
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
  int *res=ALLOC_INT();
  char met_condition=0;
  slist* tlist=0;
  slist_elem* it=0;
  lisp_atom* cres=0;
  lisp_atom* condition=0;
  lisp_atom* action=0;
  lisp_atom* body=0;

  *res=0;

  /*************************************
   * make sure we have at least 2 args *
   * both lists                        *
   *************************************/
  if(!next||!next->_next||
     ATOM_CAST(next)->type!=LTLIST||
     ATOM_CAST(next->_next)->type!=LTLIST)
  {
    PUSH_STACK_RESULT(LTINT,res);
    return 0;
  }

  /*****************************************
   * set any local iterators               *
   *****************************************/
  tlist=(slist*)ATOM_CAST(next)->data;
  if(!tlist)
    return 0;
  it=tlist->_head;
  if(!it)
    return 0;
  //let(it->data)

  /*****************************************
   * extract the loop condition and action *
   *****************************************/
  it=it->_next;
  if(!it)
    return 0;
  condition=ATOM_CAST(it);
  it=it->_next;
  if(!it)
    return 0;
  action=ATOM_CAST(it);
  it=0;

  /********************************************
   * Now start the loop                       *
   ********************************************/
  met_condition=1;
  while(met_condition)
  {
    lisp_eval(condition,0);
    cres=(lisp_atom*)slist_pop(result_stack);
    while(cres)
      switch(cres->type)
      {
        case LTID:
          if(lisp_get_symbol((const char*)cres->data,(void**)&cres,0))
            continue;
          cres=0;  
          met_condition=0;
          break;
        case LTFLOAT:
          if(!cres->data||*(float*)cres->data<1)
            met_condition=0;
          cres=0;
          break;
        case LTINT:
          if(!cres->data||*(int*)cres->data<1)
            met_condition=0;
          cres=0;
          break;
        case LTSTR:
        case LTLISPFN:
        case LTLISPMACRO:
          if(!cres->data)
            met_condition=0;
          cres=0;
          break;
        case LTLIST:
          if(!cres->data||((slist*)cres->data)->_size<1)
            met_condition=0;
          cres=0;
          break;
        default:
          met_condition=0;
          cres=0;
          break;
      }
    if(!met_condition)
      break;
    lisp_eval(ATOM_CAST(next->_next),0);
    lisp_eval(action,0);
  }
  return 2;
}

void load_flow()
{
  lisp_install_symbol("if",(void*)new_atom(LTCFNPTR,(void*)&lp_if),0);
  lisp_install_symbol("do",(void*)new_atom(LTCFNPTR,(void*)&lp_do),0);
}