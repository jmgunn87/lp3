#pragma once 
#include "stdafx.h"
#include "lisp_base.h"

static char _loaded=0;

typedef enum _MATH_OP
{
 LM_MULT=1,
 LM_ADD=2,
 LM_DIV=3,
 LM_SUB=4,
 LM_MOD=5

} MATH_OP;

static int eval_operand(lisp_atom* atom)
{
  int t=0;

  if(!atom)
    return 0;
  if(atom->type==LTINT)
    return *(int*)atom->data;
  else if(atom->type==LTID)
    if(!lisp_get_symbol((const char*)atom->data,(void**)&atom,0))
      return 0;
    else return eval_operand(atom);
  else if(atom->type==LTLIST)
  {
    lisp_eval(atom,0);
    if(result_stack->_head)
      if(ATOM_CAST(result_stack->_head)->type==LTINT)
      {
        t=*(int*)((lisp_atom*)slist_peek(result_stack))->data;
        slist_pop(result_stack);
        return t;
      }
  }
  return 0;
}
static int lp_math_eval(slist_elem* next,
                        MATH_OP _operator)
{
  lisp_atom* modifier=0;
  int swallowed=0;
  int operand=0;
  int* sum=(int*)malloc(sizeof(int));
  *sum=0;
  
  if(!next)
    return 0;

  if(ATOM_CAST(next)->type==LTID)
    modifier=ATOM_CAST(next);

  while(next)
  {   
    operand=eval_operand(ATOM_CAST(next));
    switch(_operator)
    {
      case LM_MULT:
        if(!*sum)
          (*sum)=operand;
        else (*sum)*=operand;
        break;
      case LM_ADD:
        (*sum)+=operand;
        break;
      case LM_DIV:
        if(!*sum)
          (*sum)=operand;
        else (*sum)||operand!=0?(*sum)/=operand:0;/* catch div by zero */
        break;
      case LM_SUB:
        if(!*sum)
          (*sum)=operand;
        else (*sum)-=operand;
        break;
      case LM_MOD:
        if(!*sum)
          (*sum)=operand;
        else (*sum)%=operand;
        break;
      default:
        break;
    }
    next=next->_next;
    ++swallowed;
  }

  /* apply the value to the modifier.
     if the first value sent to any
     math op is an id the result of 
     the operations will be stored
     in that id. */
  if(modifier)
  {
    slist_push(result_stack,(void*)new_atom(LTID,modifier->data));
    if(lisp_get_symbol((const char*)modifier->data,(void**)&modifier,0))
      *(int*)(*modifier).data=*sum;
  }else slist_push(result_stack,(void*)new_atom(LTINT,(void*)sum));

  return swallowed;
}

static int lp_mult(slist_elem* next)
{
  return lp_math_eval(next,LM_MULT);
}
static int lp_add(slist_elem* next)
{
  return lp_math_eval(next,LM_ADD);
}
static int lp_div(slist_elem* next)
{
  return lp_math_eval(next,LM_DIV);
}
static int lp_sub(slist_elem* next)
{
  return lp_math_eval(next,LM_SUB);
}
static int lp_mod(slist_elem* next)
{
  return lp_math_eval(next,LM_MOD);
}
void load_math_module()
{
  lisp_install_symbol("*",(void*)new_atom(LTCFNPTR,(void*)&lp_mult),0);
  lisp_install_symbol("+",(void*)new_atom(LTCFNPTR,(void*)&lp_add),0);
  lisp_install_symbol("/",(void*)new_atom(LTCFNPTR,(void*)&lp_div),0);
  lisp_install_symbol("-",(void*)new_atom(LTCFNPTR,(void*)&lp_sub),0);
  lisp_install_symbol("%",(void*)new_atom(LTCFNPTR,(void*)&lp_mod),0);
}