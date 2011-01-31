#pragma once
#include "stdafx.h"
#include "lisp_reader.h"


//static read_numeric
//static read_string
//static read_id

slist* lisp_read(const char* str)
{
  LEVAL_TYPE curr_prefix=LENORMAL;
  slist* stack=new_slist();
  lisp_atom* tatom=0;
  void* tmp=0;
  int strpos=0;
  int sublen=0;
  char is_float=0;

  slist_push(stack,(void*)new_atom(LENORMAL,LTLIST,(void*)new_slist()));

  for(;strpos<strlen(str);++strpos)
    switch(str[strpos])
    {
    case ' ':case '\n':case '\t':case '\r':case '\v':
      break;
    case '\'':curr_prefix=LEQUOTE;
      break;
    case '`':curr_prefix=LEBQUOTE;
      break;
    case ',':curr_prefix=LECOMMA;
      break;
    case '@':
      if(curr_prefix==LECOMMA)
        curr_prefix=LECOMMA_AT;
      else curr_prefix=LEAT;
      break;
    case '(':
      slist_push(stack,(void*)new_atom(curr_prefix,LTLIST,(void*)new_slist()));
      curr_prefix=LENORMAL;
      break;
    case ')':
      if(!stack)
        return 0;
      tmp=slist_pop(stack);
      if(!slist_peek(stack)||((lisp_atom*)slist_peek(stack))->type!=LTLIST)
        return 0;
      slist_append((slist*)((lisp_atom*)slist_peek(stack))->data,tmp);
      break;
    case '\"':
      /* loop through till we see another quote */
      ++strpos;
      sublen=0;
      for(;str[strpos]!='\"';++strpos,++sublen)
      {
        if(str[strpos]=='\\')
        {++strpos;++sublen;}
      }
      /* now copy the string*/
      tatom=new_atom(curr_prefix,LTSTR,malloc(sublen+1));
      curr_prefix=LENORMAL;
      strncpy((char*)tatom->data,&str[strpos-sublen],sublen);
      ((char*)tatom->data)[sublen]='\0';
      slist_append((slist*)((lisp_atom*)slist_peek(stack))->data,(void*)tatom);
      break;
    default:
      if(!isdigit(str[strpos]))
      {
        /* loop through till we see some wspace */
        sublen=0;
        for(;str[strpos]!=' '&&
          str[strpos]!=')'&&
          str[strpos]!='('&&
          str[strpos]!='\"'&&
          str[strpos]!='\''&&
          str[strpos]!='`'&&
          str[strpos]!=','&&
          str[strpos]!='@'&&
          str[strpos]!='\0';++strpos,++sublen)
          ;
        /* now copy the string*/
        tatom=new_atom(curr_prefix,LTID,malloc(sublen+1));
        curr_prefix=LENORMAL;
        strncpy((char*)tatom->data,&str[strpos-sublen],sublen);
        ((char*)tatom->data)[sublen]='\0';
        slist_append((slist*)((lisp_atom*)slist_peek(stack))->data,tatom);
      }
      else
      {
        is_float=strchr(&str[strpos],'.')?1:0;
        sublen=0;
        for(;isdigit(str[strpos])||str[strpos]=='.';++sublen,++strpos)
          ;
        if(is_float)
        {
          tatom=new_atom(curr_prefix,LTINT,malloc(sizeof(float)));
          curr_prefix=LENORMAL;
          *(float*)tatom->data=atof((const char*)&str[strpos-sublen]);
          slist_append((slist*)((lisp_atom*)slist_peek(stack))->data,(void*)tatom);
        }
        else
        {
          tatom=new_atom(curr_prefix,LTINT,malloc(sizeof(int)));
          curr_prefix=LENORMAL;
          *(int*)tatom->data=atoi(&str[strpos-sublen]);
          slist_append((slist*)((lisp_atom*)slist_peek(stack))->data,(void*)tatom);
        }
      }
      --strpos;
      break;
    }
  tatom=(lisp_atom*)slist_pop(stack);
  slist_destroy(stack);
  return (slist*)tatom->data;
}