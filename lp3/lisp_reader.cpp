#pragma once
#include "stdafx.h"
#include "lisp_reader.h"

/**********************************************
 * parser configurations                      *
 *--------------------------------------------*
 **********************************************/
#define ACCEPT 17
#define STACK_INIT_SIZE 100
#define STACK_GROW_AMOUNT 50

/**********************************************
 * temp item variables                        *
 *--------------------------------------------*
 **********************************************/
static void* __item_synth_val=0;

/**********************************************
 * define our memory allocation functions     *
 *--------------------------------------------*
 * This will be either a user defined pointer *
 * or by default clib memory allocators.      *
 **********************************************/
static void*(*allocator)(size_t)=malloc;
static void*(*re_allocator)(void*,size_t)=realloc;
static void(*de_allocator)(void*)=free;

/****************************************************
 * declare, define and initialize the parser stack  *
 * and global macro functions for use by the parser *
 *--------------------------------------------------*
 ****************************************************/
static int stack_cursor=-1;
static int stack_size=STACK_INIT_SIZE;
/*declare and intialize the stack*/
static lp_token* stack=0;

/*stack macros*/
#define EXTEND_STACK()\
{\
  stack_size+=STACK_GROW_AMOUNT;\
  stack=(lp_token*)re_allocator(stack,stack_size*sizeof(lp_token));\
}
#define PUSH_STACK(id,val)\
{\
  if(stack_cursor+1>=stack_size-1)\
    EXTEND_STACK();\
  stack[stack_cursor+1].lex_id=id;\
  stack[stack_cursor+1].lex_val=val;\
  ++stack_cursor;\
}
#define POP_STACK(n)( stack_cursor-=n )
#define PEEK_STACK(n)( stack[stack_cursor-n] )
  
//parser table declaration
static signed char parse_table[16][13]=
{{0,1,2,3,0,4,5,0,6,7,8,9,0},
{0,0,0,0,0,0,0,0,0,0,0,0,17},
{0,10,2,3,0,4,5,0,6,7,8,9,-2},
{0,0,0,0,0,0,-5,-5,-5,-5,-5,-5,-5},
{0,0,0,0,0,0,-4,-4,-4,-4,-4,-4,-4},
{0,0,11,3,12,4,5,13,6,7,8,9,0},
{0,0,0,0,0,0,-10,-10,-10,-10,-10,-10,-10},
{0,0,0,0,0,0,-11,-11,-11,-11,-11,-11,-11},
{0,0,0,0,0,0,-12,-12,-12,-12,-12,-12,-12},
{0,0,0,0,0,0,-13,-13,-13,-13,-13,-13,-13},
{0,0,0,0,0,0,0,0,0,0,0,0,-3},
{0,0,11,3,14,4,5,-8,6,7,8,9,0},
{0,0,0,0,0,0,0,15,0,0,0,0,0},
{0,0,0,0,0,0,-6,-6,-6,-6,-6,-6,-6},
{0,0,0,0,0,0,0,-9,0,0,0,0,0},
{0,0,0,0,0,0,-7,-7,-7,-7,-7,-7,-7}};

//reduction actions switch function
static int __prh(int* ri)
{
  switch(-*ri)
  {
    case 2:
      if(!__item_synth_val)
        __item_synth_val=new_slist();
      slist_push((slist*)__item_synth_val,
                 (void*)PEEK_STACK(0).lex_val);
      *ri=1;
      return 1;
    case 3:
      slist_push((slist*)PEEK_STACK(0).lex_val,
                 (void*)PEEK_STACK(1).lex_val);
      __item_synth_val=PEEK_STACK(0).lex_val;
      *ri=1;
      return 2;
    case 4:
      __item_synth_val=PEEK_STACK(0).lex_val;
      *ri=2;
      return 1;
    case 5:
      __item_synth_val=PEEK_STACK(0).lex_val;
      *ri=2;
      return 1;
    case 6:
      __item_synth_val=new_atom(LTLIST,(void*)new_slist());
      *ri=3;
      return 2;
    case 7:
      __item_synth_val=new_atom(LTLIST,PEEK_STACK(1).lex_val);
      *ri=3;
      return 3;
    case 8:
      if(!__item_synth_val)
        __item_synth_val=(void*)new_slist();
      slist_push((slist*)__item_synth_val,
                 (void*)PEEK_STACK(0).lex_val);
      *ri=4;
      return 1;
    case 9:
      slist_push((slist*)PEEK_STACK(0).lex_val,
                 (void*)PEEK_STACK(1).lex_val);
      __item_synth_val=PEEK_STACK(0).lex_val;
      *ri=4;
      return 2;
    case 10:
      __item_synth_val=new_atom(LTID,PEEK_STACK(0).lex_val);
      *ri=5;
      return 1;
    case 11:
      __item_synth_val=new_atom(LTINT,PEEK_STACK(0).lex_val);
      *ri=5;
      return 1;
    case 12:
      __item_synth_val=new_atom(LTFLOAT,PEEK_STACK(0).lex_val);
      *ri=5;
      return 1;
    case 13:
      __item_synth_val=new_atom(LTSTR,PEEK_STACK(0).lex_val);
      *ri=5;
      return 1;
    default:
      return-1;
  }
}

static int __parse_token(lp_token* input)
{
  int top_state=0;
  int pop_count=0;
  int action=0;
  
  /*initialize the stack at state 0*/
  if(stack_cursor==-1)
    PUSH_STACK(0,0);
  
  while(1)
  {
    top_state=PEEK_STACK(0).lex_id;
    if(!input)action=parse_table[top_state][12];
    else if(input->lex_id>=12)return 0;
    else action=parse_table[top_state][input->lex_id];
    
    if(action==ACCEPT)/*ACCEPT*/
    {
      action=--(PEEK_STACK(0).lex_id);
      __prh(&action);
      return ACCEPT;
    }
    if(action>0)/*SHIFT*/
    {
      PUSH_STACK(action,input->lex_val);
      return 1;
    }
    else if(action<0)/*REDUCE*/
    {
      pop_count=__prh(&action);
      if(pop_count==-1)
        return 0;/*catch errors here*/
      POP_STACK(pop_count);/*pop the stack n times specified by __prh*/
      PUSH_STACK(parse_table[PEEK_STACK(0).lex_id][action],__item_synth_val);/*push the reduction along with any synthesised values*/
      __item_synth_val=0;
      continue;
    }
    else/*error*/
      return 0;
  }
}

static int lisp_lex_token(lp_token* token,
                          const char* str)
{
  int strpos=0;
  int sublen=0;
  char is_float=0;

  if(!token||!str)
    return -1;

  for(;strpos<strlen(str);++strpos)
    switch(str[strpos])
    {
      /* EOF-LP3*/
      case '$':
        return -2;
      case ' ':case '\n':case '\t':case '\r':case '\v':
        break;
      case '(':
        token->lex_id=LPAREN;
        return ++strpos;
      case ')':
        token->lex_id=RPAREN;
        return ++strpos;
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
        token->lex_id=LTSTR;
        token->lex_val=(char*)malloc(sublen+1);
        strncpy((char*)token->lex_val,&str[strpos-sublen],sublen);
        ((char*)token->lex_val)[sublen]='\0';
        return strpos+1;
      default:
        if(!isdigit(str[strpos]))
        {
          /* loop through till we see some wspace */
          sublen=0;
          for(;str[strpos]!=' '&&
               str[strpos]!=')'&&
               str[strpos]!='('&&
               str[strpos]!='\"'&&
               str[strpos]!='\0';++strpos,++sublen)
            ;
          /* now copy the string*/
          token->lex_id=LTID;
          token->lex_val=(char*)malloc(sublen+1);
          strncpy((char*)token->lex_val,&str[strpos-sublen],sublen);
          ((char*)token->lex_val)[sublen]='\0';
          return strpos;
        }
        else
        {
          is_float=strchr(&str[strpos],'.')?1:0;
          sublen=0;
          for(;isdigit(str[strpos])||str[strpos]=='.';++sublen,++strpos);
          if(is_float)
          {
            token->lex_id=LTFLOAT;
            token->lex_val=(float*)malloc(sizeof(float));
            *(float*)token->lex_val=atof((const char*)&str[strpos-sublen]);
          }
          else
          {
            token->lex_id=LTINT;
            token->lex_val=(int*)malloc(sizeof(int));
            *(int*)token->lex_val=atoi(&str[strpos-sublen]);
          }
          return strpos;
        }
        return -1;
    }
  return -1;
}

slist* parse_lisp(const char* input,
                  int* input_pos)
{
  lp_token lexed;
  int str_inc=0;
  int ipos=0;

  if(!input)
    return 0;

  /*make sure the parse stack is initialized and
    that the its cursor is reset(-1)*/
  if(!stack)
    stack=(lp_token*)allocator(STACK_INIT_SIZE*sizeof(lp_token));
  stack_cursor=-1;

  lexed.lex_id=-1;
  lexed.lex_val=0;
 
  while(1)
  {
    /* lex */
    str_inc=lisp_lex_token(&lexed,(const char*)&input[ipos]);
    if(str_inc<0)
      if(__parse_token(0)==0)/* parse EOF */
      {
        if(input_pos)
          *input_pos=str_inc==-1?ipos:++ipos;
        return 0;
      }
      else
      {
        if(input_pos)
          *input_pos=str_inc==-1?ipos:++ipos;
        return (slist*)PEEK_STACK(0).lex_val;
      }

    /* parse */
    if(__parse_token(&lexed)==0)
    {
      if(input_pos)
        *input_pos+=ipos;
      return 0;
    }

    /* update the string position */
    ipos+=str_inc;
  }
  if(input_pos)
    *input_pos+=ipos;
  return 0;
}