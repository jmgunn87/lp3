// lisp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lisp_base.h"
#include <windows.h>

/* REPL */
void lisp_repl()
{
  int in_cursor=0;
  char input[256];
  slist* lisp_in=0;
  slist_elem* sle=0;
  FILE* console=0;

   /*open the console get some lists*/
  printf("-= LP3 REPL =-\r\n");
  console=fopen("CON","r");
  while(1)
  {
    printf(">");
    memset((void*)input,0,256);
    fgets(input,254,console);
    lisp_in=parse_lisp((const char*)input,0);
    if(lisp_in)
    {
      sle=lisp_in->_head;
      while(sle)
      {
        lisp_eval((lisp_atom*)sle->_data,sle->_next);
        sle=sle->_next;
      }
      print_atom(result_stack->_head?(lisp_atom*)result_stack->_head->_data:0);
      result_stack=new_slist();
    }
    else printf("list error!");
    printf("\n");
  }
}

int lp3_process(char* str)
{
  int pos=0;
  slist* lisp_in=0;
  slist_elem* sle=0;

  if(!str)
    return 0;

  lisp_in=parse_lisp((const char*)str,&pos);
  if(lisp_in)
  {
    sle=lisp_in->_head;
    while(sle)
    {
      lisp_eval((lisp_atom*)sle->_data,sle->_next);
      sle=sle->_next;
    }
    print_atom(result_stack->_head?(lisp_atom*)result_stack->_head->_data:0);
    result_stack=new_slist();
  }
  else
    printf("list error!");
   
  return pos;
}

int lp3_gen_file(const char* lp3_in,
                 const char* lp3_out)
{
  FILE* fin=0;
  FILE* fout=0;
  char* buffer=0;
  unsigned int cursor=0;
  long size=0;
  long result=0;

  fin=fopen(lp3_in,"r+");
  fout=freopen(lp3_out,"w",stderr);
  if(!fin||!fout)
    return -1;

  /* obtain file size:*/
  fseek(fin,0,SEEK_END);
  size=ftell(fin);
  rewind(fin);

  // allocate memory to contain the whole file:
  buffer=(char*)malloc(sizeof(char)*size);
  // copy the file into the buffer:
  result=fread(buffer,1,size,fin);
  buffer[result]='\0';

  for(;cursor<result;)
  {
    switch(buffer[cursor])
    {
      case '\\':
        ++cursor;
        break;
      case '$':/* see if we have any lp3 */
        ++cursor;
        for(;cursor<result&&
             buffer[cursor+1]!='$'&&
             isspace(buffer[cursor+1]);++cursor);
        if(buffer[cursor]=='(')
        {
          cursor+=lp3_process(&buffer[cursor]);
          continue;
        }
        break;
      default:
        break;
    }
    fputc(buffer[cursor],fout);
    ++cursor;
  }

  fclose(fin);
  fclose(fout);
  free(buffer);
  return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
  int auth=getchar();
  char name[256];
  memset((void*)name,0,256);

  load_lisplib();
  
  /*install a dummy variable*/
  if(!isdigit(auth))
    auth=0;
  lisp_install_symbol("authorized",new_atom(LTINT,(void*)&auth),0);
  fflush(stdin);
  gets(name);
  lisp_install_symbol("author",new_atom(LTSTR,(void*)&name),0);

  
  lp3_gen_file((const char*)"C:\\Users\\Kerry\\Documents\\lp3.txt",
               (const char*)"C:\\Users\\Kerry\\Documents\\lp3out.txt");
  system("PAUSE");
	return 0;
}

