#pragma once
#include "stdafx.h"
#include "lisp_cmp.h"
#include "lisp_types.h"

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
static lisp_atom lp_cmp(slist_elem* next,
                        CMP_OP op)
{
  lisp_atom* lval=ATOM_CAST(next);
  lisp_atom* rval=ATOM_CAST(next->_next);
  lisp_atom ret;
  ret.type=LTNIL;

  /***************************************
   * evaluate all expressions            *
   ***************************************/
  next=next->_next;
  while(1) 
  {
    if(lval->type!=rval->type)
      return ret;
    
    switch(lval->type)
    {
    case LTSTR:
    case LTID:
      if(!cmp_str((char*)lval->data,(char*)rval->data,op))
        return ret;
      break;
    case LTFLOAT:
      if(!cmp_float(*(float*)lval->data,*(float*)rval->data,op))
      return ret;
      break;
    case LTINT:
      if(!cmp_int(*(int*)lval->data,*(int*)rval->data,op))
      return ret;
      break;
    case LTLISPFN:
    case LTLISPMACRO:
    case LTLIST:
    default:
      return ret;
      break;
    }
    lval=rval;
    if(next->_next)
    {
      rval=ATOM_CAST(next->_next);
      next=next->_next;
    }else break;
  }
  ret.type=LTTRUE;
  return ret;
}
static lisp_atom lp_eq(slist_elem* next){return lp_cmp(next,CMP_EQ);}
static lisp_atom lp_neq(slist_elem* next){return lp_cmp(next,CMP_NEQ);}
static lisp_atom lp_lt(slist_elem* next){return lp_cmp(next,CMP_LT);}
static lisp_atom lp_leq(slist_elem* next){return lp_cmp(next,CMP_LEQ);}
static lisp_atom lp_gt(slist_elem* next){return lp_cmp(next,CMP_GT);}
static lisp_atom lp_geq(slist_elem* next){return lp_cmp(next,CMP_GEQ);}

void load_cmp()
{
  lisp_install_symbol("=",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_eq)),0);
  lisp_install_symbol("equal",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_eq)),0);
  lisp_install_symbol("~=",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_neq)),0);
  lisp_install_symbol("unequal",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_neq)),0);
  lisp_install_symbol("<",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_lt)),0);
  lisp_install_symbol("less",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_lt)),0);
  lisp_install_symbol("<=",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_leq)),0);
  lisp_install_symbol("less-or-equal",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_leq)),0);
  lisp_install_symbol(">",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_gt)),0);
  lisp_install_symbol("greater",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_gt)),0);
  lisp_install_symbol(">=",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_geq)),0);
  lisp_install_symbol("greater-or-equal",(void*)new_atom(LTCFNPTR,(void*)new_lisp_cfn(1,0,CFN_ARGNOCIEL,lp_geq)),0);
}