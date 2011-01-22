#pragma once
#include "stdafx.h"
#include "lisp_cmp.h"
#include "lisp_base.h"

typedef enum _CMP_OP
{
  CMP_EQ,
  CMP_NEQ,
  CMP_LT,
  CMP_LEQ,
  CMP_GT,
  CMP_GEQ

} CMP_OP;

static char cmp_int(int lval,
                    int rval,
                    CMP_OP op)
{
  switch(op)
  {
    case CMP_EQ:  return lval==rval?1:0;
    case CMP_NEQ: return lval!=rval?1:0;
    case CMP_LT:  return lval<rval?1:0;
    case CMP_LEQ: return lval<=rval?1:0;
    case CMP_GT:  return lval>rval?1:0;
    case CMP_GEQ: return lval>=rval?1:0;
    default:      return 0;
  }
  return 0;
}

static char cmp_float(float lval,
                      float rval,
                      CMP_OP op)
{
  switch(op)
  {
    case CMP_EQ:  return lval==rval?1:0;
    case CMP_NEQ: return lval!=rval?1:0;
    case CMP_LT:  return lval<rval?1:0;
    case CMP_LEQ: return lval<=rval?1:0;
    case CMP_GT:  return lval>rval?1:0;
    case CMP_GEQ: return lval>=rval?1:0;
    default:      return 0;
  }
  return 0;
}

static char cmp_str(char* lval,
                    char* rval,
                    CMP_OP op)
{
  char tres=0;

  if(!lval||rval)
    return 0;

  tres=strcmp((const char*)lval,(const char*)rval)==0;
  if(op==CMP_EQ)
    return tres;
  if(op==CMP_NEQ)
    return ~tres;

  return 0;
}
static int lp_cmp(slist_elem* next,
                  CMP_OP op)
{
  lisp_atom* lval=0;
  lisp_atom* rval=0;
  int* res=ALLOC_INT();
  *res=0;

  if(!next||!next->_next)
    return 0;

  /***************************************
   * evaluate left and right expressions *
   ***************************************/
  lisp_eval(ATOM_CAST(next),0);
  lval=(lisp_atom*)slist_pop(result_stack);
  lisp_eval(ATOM_CAST(next->_next),0);
  rval=(lisp_atom*)slist_pop(result_stack);

  if(lval->type!=rval->type)
    PUSH_STACK_RESULT(LTINT,res);
  else
    switch(lval->type)
    {
      case LTSTR:
      case LTID:
        *res=cmp_str((char*)lval->data,
                     (char*)rval->data,
                     op);
        PUSH_STACK_RESULT(LTINT,res);
        break;
      case LTFLOAT:
        *res=cmp_float(*(float*)lval->data,
                       *(float*)rval->data,
                       op);
        PUSH_STACK_RESULT(LTINT,res);
        break;
      case LTINT:
        *res=cmp_int(*(int*)lval->data,
                     *(int*)rval->data,
                     op);
        PUSH_STACK_RESULT(LTINT,res);
        break;
      case LTLISPFN:
      case LTLISPMACRO:
      case LTLIST:
      default:
        PUSH_STACK_RESULT(LTINT,res);
        break;
    }
  return 2;
}
static int lp_eq(slist_elem* next)
{
  return lp_cmp(next,CMP_EQ);
}
static int lp_neq(slist_elem* next)
{
  return lp_cmp(next,CMP_NEQ);
}
static int lp_lt(slist_elem* next)
{
  return lp_cmp(next,CMP_LT);
}
static int lp_leq(slist_elem* next)
{
  return lp_cmp(next,CMP_LEQ);
}
static int lp_gt(slist_elem* next)
{
  return lp_cmp(next,CMP_GT);
}
static int lp_geq(slist_elem* next)
{
  return lp_cmp(next,CMP_GEQ);
}

void load_cmp()
{
  lisp_install_symbol("=",(void*)new_atom(LTCFNPTR,(void*)&lp_eq),0);
  lisp_install_symbol("!=",(void*)new_atom(LTCFNPTR,(void*)&lp_neq),0);
  lisp_install_symbol("<",(void*)new_atom(LTCFNPTR,(void*)&lp_lt),0);
  lisp_install_symbol("<=",(void*)new_atom(LTCFNPTR,(void*)&lp_leq),0);
  lisp_install_symbol(">",(void*)new_atom(LTCFNPTR,(void*)&lp_gt),0);
  lisp_install_symbol(">=",(void*)new_atom(LTCFNPTR,(void*)&lp_geq),0);
}