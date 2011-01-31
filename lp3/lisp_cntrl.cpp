#pragma once 
#include "stdafx.h"
#include "lisp_cntrl.h"
#include "lisp_types.h"
#include "lisp_eval.h"

static char atom_truth(lisp_atom* atom)
{
  if(!atom)
    return 0;
  while(1)
    switch(atom->type)
    {
      case LTID:
        if(lisp_get_symbol((const char*)atom->data,(void**)&atom,0))
          continue;
        return 0;
      case LTFLOAT:
        if(!atom->data||*(float*)atom->data<1)
          return 0;
        return 1;
      case LTINT:
        if(!atom->data||*(int*)atom->data<1)
          return 0;
        return 1;
      case LTSTR:
      case LTLISPFN:
      case LTLISPMACRO:
        return atom->data?1:0;
      case LTLIST:
        if(!atom->data||((slist*)atom->data)->_size<1)
          return 0;
        return 1;
      case LTTRUE:
        return 1;
      case LTNIL:
      default:
        return 0;
    }
}
static lisp_atom lp_not(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  if(!atom_truth(ATOM_CAST(next)))
  {
    ret.type=LTTRUE;
    ret.data=(void*)LTTRUE;
  }
  return ret;
}
static lisp_atom lp_if(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  /**************************************
   * evaluate the condition and execute *
   * the correct branch                 *
   **************************************/
  lisp_atom atom=lisp_eval(ATOM_CAST(next),0);
  if(atom_truth(&atom))
    return lisp_eval(ATOM_CAST(next->_next),0);
  if(next->_next->_next)
    return lisp_eval(ATOM_CAST(next->_next->_next),0);
  return ret;
}

static lisp_atom lp_do(slist_elem* next)
{
  slist* tlist=0;
  slist_elem* it=0;
  lisp_atom* condition=0;
  lisp_atom* action=0;
  lisp_atom* body=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  /*************************************
   * make sure we have our 2 args are  *
   * both lists                        *
   *************************************/
  if(ATOM_CAST(next)->type!=LTLIST||
     ATOM_CAST(next->_next)->type!=LTLIST)
    return ret;
  

  /*****************************************
   * set any local iterators               *
   *****************************************/
  tlist=(slist*)ATOM_CAST(next)->data;
  if(!tlist)return ret;
  it=tlist->_head;
  if(!it)return ret;
  //let(it->data)

  /*****************************************
   * extract the loop condition and action *
   *****************************************/
  it=it->_next;
  if(!it)return ret;
  condition=atom_copy(ATOM_CAST(it));
  it=it->_next;
  if(!it)return ret;
  action=atom_copy(ATOM_CAST(it));
  it=0;
  body=ATOM_CAST(next->_next);

  /********************************************
   * now start the loop                       *
   ********************************************/
  while(1)
  {
    ret=lisp_eval(atom_copy(condition),0);
    if(!atom_truth(&ret))
      break;
    lisp_eval(atom_copy(body),0);
    lisp_eval(atom_copy(action),0);
  }
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;
  return ret;
}

static lisp_atom lp_and(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTTRUE;
  ret.data=(void*)LTTRUE;

  while(next)
  {
    if(!atom_truth(ATOM_CAST(next)))
    {
      ret.type=LTNIL;
      ret.data=(void*)LTNIL;
      return ret;
    }
    next=next->_next;
  }
  return ret;
}
static lisp_atom lp_or(slist_elem* next)
{
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  while(next)
  {
    if(atom_truth(ATOM_CAST(next)))
    {
      ret.type=LTTRUE;
      ret.data=(void*)LTTRUE;
      return ret;
    }
    next=next->_next;
  }
  return ret;
}

void load_flow()
{
  lisp_install_symbol("and",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_and)),0);
  lisp_install_symbol("&&",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_and)),0);
  lisp_install_symbol("or",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_or)),0);
  lisp_install_symbol("||",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_or)),0);
  lisp_install_symbol("if",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(0,2,3,lp_if)),0);
  lisp_install_symbol("not",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,1,1,lp_not)),0);
  lisp_install_symbol("while",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(0,2,2,lp_do)),0);
}