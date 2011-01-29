#pragma once
#include "stdafx.h"
//#include "lisp_types.h"
#include "lisp_string.h"

/**
 * Format directives
 *-----------------------------------
   ~T   => tabulation
   ~nT  => number of spaces in tabulation
   ~A   => string
   ~D   => decimal
   ~X   => hex
   ~O   => octal
   ~B   => binary
   ~< and ~>  => justification start, end
   ~n<  => justification width
   ~(text~) converts text to lower case. 
   ~:@(text~) converts text to upper case. 
   ~:(text~) capitalizes each word in text. 
   ~@(text~) capitalizes the first word in text and converts the rest to lower case. 
   ~{ and ~} iteration start and end. 
   ~R spell an integer
   ~P plural
   ~F floating point
   ~E scientific notation
   ~G ~F or ~E, depending upon magnitude
   ~$ monetary
   ~! 
Flushes the output if format destination is a port. 

~_ 
Print a #\space character 

~n_ 
print n #\space characters. 

~nC 
Takes n as an integer representation for a character. No arguments are consumed. n is converted to a character by integer->char. n must be a positive decimal number. 


 */
//static 
char* lp_format(char* format,
                       slist_elem* next)
{
  int i=0;
  int slen=0;
  int fcount=1;
  char at=0;
  char comma=0;
  char* ret_str=0;

  if(!format)
    return 0;

  /* please make sure this is null-terminated...*/
  slen=strlen(format);

  for(;i<slen;++i,at=0,comma=0,fcount=1)
    if(format[i]=='~')
    {
      ++i;
      /* extract the format count */
      if(i<slen&&isdigit(format[i]))
      {
        fcount=atoi(&format[i]);
        while(i<slen&&isdigit(format[++i]));
      }
      /* set any commas or @'s*/
      if(format[i]==':'){comma=1;++i;}
      if(format[i]=='@'){at=1;++i;}

      if(i>=slen)
        return 0;

      switch(format[i])
      {
      case '|'://page separators
        for(;fcount>=0;--fcount)
          printf("|");
        break;
      case '%': //new lines
        for(;fcount>=0;--fcount)
          printf("\n");
        break;
      case '&'://fresh lines
        printf("\r\n?");
        break;
      case '~': //tildes
        for(;fcount>=0;--fcount)
          printf("~");
        break;
      case '*'://jumps n arguments forward. 
        printf("*");
        break;
   //~:*  => jumps 1 argument backward. 
   //~n:* => jumps n arguments backward.
   //~@*  => jumps to the 0th argument. 
   //~n@* => jumps to the nth argument (beginning from 0) 
      case 'C': //character
        if(fcount)
          printf("%c",fcount);
        else printf("C");
        break;
      case 'T'://tabulation
        if(fcount)
          for(;fcount>=0;--fcount)printf(" ");
        else printf("\t");
        break;
      case 'A':
        //for(;fcount>=0;--fcount)
        printf("A");
        break;
      case 'D'://decimal
        printf("D");
        break;
      case 'X'://hex
        printf("X");
        break;
      case 'O'://octal
        printf("O");
        break;
      case 'B'://binary
        printf("B");
        break;
      case 'R'://spell an integer
        printf("R");
        break;
      case 'P'://plural
        printf("P");
        break;
      case 'F':// floating point
        printf("F");
        break;
      case 'E'://scientific notation
        printf("E");
        break;
      case 'G'://?general
        printf("G");
        break;
      case '$':// monetary
        printf("$");
        break;
      case '<'://justification
        printf("<");
        break;
      case '('://case conversion
        printf("(");
        break;
      case '{'://iteration
        //for(;fcount>=0;--fcount)
        printf("{");
        break;
      default:printf("error");
        break;
      }
    }
    else/* print what we see */
      printf("%c",format[i]);

  return 0;
}