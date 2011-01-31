// lisp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lisp_eval.h"
#include "lisp_cmp.h"
#include "lisp_cntrl.h"
#include "lisp_math.h"
#include "lisp_symanip.h"
#include "lisp_mem.h"
#include "lisp_io.h"
#include "lisp_access.h"
#include "lisp_utils.h"
#include "lisp_string.h"

/* REPL */
void lisp_repl()
{
  int in_cursor=0;
  char input[256];
  slist* lisp_in=0;
  slist_elem* sle=0;
  FILE* console=0;
  lisp_atom ret;

   /*open the console get some lists*/
  printf("ReadEvalPrintLoop\r\n");
  console=fopen("CON","r");
  while(1)
  {
    printf(">");
    memset((void*)input,0,256);
    fgets(input,254,console);
    lisp_in=lisp_read((const char*)input);
    if(lisp_in)
    {
      sle=lisp_in->_head;
      while(sle)
      {
        ret=lisp_eval((lisp_atom*)sle->_data,sle->_next);
        sle=sle->_next;
        printf("\n");
      }
    }
    else printf("syntax error: parenthesis unbalanced \n");
  }
}

//int lp3_process(char* str)
//{
//  int pos=0;
//  slist* lisp_in=0;
//  slist_elem* sle=0;
//
//  if(!str)
//    return 0;
//
//  lisp_in=parse_lisp((const char*)str,&pos);
//  if(lisp_in)
//  {
//    sle=lisp_in->_head;
//    while(sle)
//    {
//      lisp_eval((lisp_atom*)sle->_data,sle->_next);
//      sle=sle->_next;
//    }
//    slist_destroy(result_stack);
//    result_stack=new_slist();
//  }
//  else printf("list error!"); 
//  return pos;
//}
//
//int lp3_gen_file(const char* lp3_in,
//                 const char* lp3_out)
//{
//  FILE* fin=0;
//  FILE* fout=0;
//  char* buffer=0;
//  unsigned int cursor=0;
//  long size=0;
//  long result=0;
//
//  fin=fopen(lp3_in,"r+");
//  fout=freopen(lp3_out,"w",stdout);
//  if(!fin||!fout)
//    return -1;
//
//  /* obtain file size:*/
//  fseek(fin,0,SEEK_END);
//  size=ftell(fin);
//  rewind(fin);
//
//  // allocate memory to contain the whole file:
//  buffer=(char*)malloc(sizeof(char)*size);
//
//  // copy the file into the buffer:
//  result=fread(buffer,1,size,fin);
//  buffer[result]='\0';
//
//  for(;cursor<result;)
//  {
//    switch(buffer[cursor])
//    {
//      case '\\':
//        ++cursor;
//        break;
//      case '$':/* see if we have any lp3 */
//        ++cursor;
//        for(;cursor<result&&
//             buffer[cursor+1]!='$'&&
//             isspace(buffer[cursor+1]);++cursor);
//        if(buffer[cursor]=='(')
//        {
//          cursor+=lp3_process(&buffer[cursor]);
//          continue;
//        }
//        break;
//      default:
//        break;
//    }
//    fputc(buffer[cursor],fout);
//    ++cursor;
//  }
//
//  fclose(fin);
//  fclose(fout);
//  free(buffer);
//  return 1;
//}

lisp_array* md_array()
{
  int r=0;
  int c=0;
  lisp_array* arr=0;
  int** table=(int**)malloc(sizeof(int*)*5);
  for(;r<5;++r)
  {
    table[r]=(int*)malloc(sizeof(int)*5);
    for(c=0;c<5;++c)
      table[r][c]=c;
  }

  
  arr=new_lisp_array(LTARRAY,5,0);
  arr->data=(lisp_array**)calloc(5,sizeof(lisp_array*));
  for(r=0;r<5;++r)
  {
    ((lisp_array**)arr->data)[r]=new_lisp_array(LTINT,5,(void*)calloc(5,sizeof(int)));
    for(c=0;c<5;++c)
      ((int*)((lisp_array**)arr->data)[r]->data)[c]=table[r][c];
  }

  lisp_install_symbol("table",new_atom(LENORMAL,LTARRAY,(void*)arr),0);
  return arr;
}



int _tmain(int argc, _TCHAR* argv[])
{
  int i=0;
  int auth=0;
  char name[256];
  memset((void*)name,0,256);

  char* one="one";
  char* two="two";
  int ione=1;
  int i2=2;

  lisp_atom* lst=new_atom(LEQUOTE,LTLIST,(void*)new_slist());
  slist_append((slist*)lst->data,(void*)new_atom(LENORMAL,LTINT,(void*)&ione));
  slist_append((slist*)lst->data,(void*)new_atom(LENORMAL,LTINT,(void*)&ione));
  slist_append((slist*)lst->data,(void*)new_atom(LENORMAL,LTINT,(void*)&i2));
  lisp_install_symbol("lst",(void*)lst,0);

 

  lp_format("test~2Tstring!~3%",0);
//  getchar();

  load_types();
  load_symanip();
  load_math_module();
  load_cmp();
  load_flow();
  load_io();
  load_access();
  load_utils();

  md_array();

  lisp_repl();
  
  

  //
  ///*install a dummy variable*/
  //auth=getchar();
  //if(!isdigit(auth))
  //  auth=0;
  //lisp_install_symbol("authorized",new_atom(LTINT,(void*)&auth),0);
  //fflush(stdin);
  //gets(name);
  //lisp_install_symbol("author",new_atom(LTSTR,(void*)&name),0);

  //lp3_gen_file((const char*)"C:\\Users\\Kerry\\Documents\\lp3.txt",//input template
  //             (const char*)"C:\\Users\\Kerry\\Documents\\lp3out.txt");//output generated file

  system("PAUSE");
	return 0;
}

