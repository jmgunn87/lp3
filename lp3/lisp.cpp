//// lisp.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"
//#include "lisp_base.h"
//#include "lisp_array.h"
//#include <windows.h>
//
///* REPL */
//void lisp_repl()
//{
//  int in_cursor=0;
//  char input[256];
//  slist* lisp_in=0;
//  slist_elem* sle=0;
//  FILE* console=0;
//
//   /*open the console get some lists*/
//  printf("-= LP3 REPL =-\r\n");
//  console=fopen("CON","r");
//  while(1)
//  {
//    printf(">");
//    memset((void*)input,0,256);
//    fgets(input,254,console);
//    lisp_in=parse_lisp((const char*)input,0);
//    if(lisp_in)
//    {
//      sle=lisp_in->_head;
//      while(sle)
//      {
//        lisp_eval((lisp_atom*)sle->_data,sle->_next);
//        sle=sle->_next;
//      }
//      slist_destroy(result_stack);
//      result_stack=new_slist();
//    }
//    else printf("list error!");
//    printf("\n");
//  }
//}
//
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
//
//lisp_array* md_array()
//{
//  int r=0;
//  int c=0;
//  lisp_array* arr=0;
//  int** table=(int**)malloc(sizeof(int*)*5);
//  for(;r<5;++r)
//  {
//    table[r]=(int*)malloc(sizeof(int)*5);
//    for(c=0;c<5;++c)
//      table[r][c]=c;
//  }
//
//  arr=new_lisp_array(LTARRAY,5,0);
//  lisp_install_symbol("table",new_atom(LTARRAY,(void*)arr),0);
//  for(r=0;r<5;++r)
//    ((lisp_array**)arr->data)[0]=new_lisp_array(LTINT,5,(void*)&table[r]);
//  return arr;
//}
//int _tmain(int argc, _TCHAR* argv[])
//{
//  int i=0;
//  int auth=0;
//  char name[256];
//  memset((void*)name,0,256);
//
//
//  lisp_array* la=md_array();
//  //LISP_TYPE lt;
//  //int depth=0;
//  //array_type_depth((const lisp_array*)la,&lt,&depth);
//  //printf("%d %d\n",lt,depth);
//
//  //int* steps=(int*)malloc(sizeof(int)*depth);
//  //for(;i<depth;++i)
//  //{
//  //  steps[i]=array_depth_step((const lisp_array*)la,i,4);
//  //  printf("step %d\n",steps[i]);
//  //}
//
//  load_lisplib();
//  
//  /*install a dummy variable*/
//  auth=getchar();
//  if(!isdigit(auth))
//    auth=0;
//  lisp_install_symbol("authorized",new_atom(LTINT,(void*)&auth),0);
//  fflush(stdin);
//  gets(name);
//  lisp_install_symbol("author",new_atom(LTSTR,(void*)&name),0);
//
//  lp3_gen_file((const char*)"C:\\Users\\Kerry\\Documents\\lp3.txt",//input template
//               (const char*)"C:\\Users\\Kerry\\Documents\\lp3out.txt");//output generated file
//
//  system("PAUSE");
//	return 0;
//}
//
