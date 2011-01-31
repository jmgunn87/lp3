#pragma once 
#include "stdafx.h"
#include "lisp_access.h"
#include "lisp_types.h"
#include "lisp_array.h"
#include "lisp_cmp.h"

static lisp_atom lp_at(slist_elem* next)
{
  int index=0;
  char* str=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  if(ATOM_CAST(next->_next)->type!=LTINT)
    return ret;
  index=*(int*)ATOM_CAST(next->_next)->data;

  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      str=(char*)ATOM_CAST(next)->data;
      if(strlen(str)>=index)
      {
        ret.type=LTSTR;
        ret.data=(void*)ALLOC_STR(2);
        memcpy(ret.data,(const void*)&((char*)ATOM_CAST(next)->data)[index],1);
        ((char*)ret.data)[1]='\0';
        return ret;
      }
      return ret;
    case LTLIST:
      la=(lisp_atom*)slist_at((slist*)ATOM_CAST(next)->data,index+1);
      if(la)
      {
        ret.type=la->type;
        ret.data=la->data;
        return ret;
      }
      return ret;
    case LTARRAY:
      la=ATOM_CAST(next);
      tarr=(lisp_array*)la->data;
      if(tarr->type!=LTARRAY)
        return ret;
      else
      {
        next=next->_next;

        /*traverse the array through each of its dimensions*/
        tarr=(lisp_array*)array_at(tarr,index);      
        while(tarr&&tarr->type==LTARRAY)
        {
          next=next->_next;
          if(next&&ATOM_CAST(next)->type==LTINT)
            index=*(int*)ATOM_CAST(next)->data;
          else return ret;
          tarr=(lisp_array*)array_at(tarr,index);
        }
        /*this will either be a terminal dimension (iee not LTARRAY) or null/nil*/
        if(tarr&&next&&next->_next)
          if(ATOM_CAST(next->_next)->type==LTINT&&
             ATOM_CAST(next->_next)->data!=0)
          {
            ret.type=tarr->type;
            ret.data=array_at(tarr,*(int*)ATOM_CAST(next->_next)->data);
            return ret;
          }
      }
      return ret;
    default:
      break;
  }
  return ret;
}

static lisp_atom lp_between(slist_elem* next)
{
  int from=0;
  int to=0;
  int diff=0;
  char* str=0;
  slist* lst=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  if(ATOM_CAST(next->_next)->type!=LTINT)
    return ret;
  from=*(int*)ATOM_CAST(next->_next)->data;
  if(ATOM_CAST(next->_next->_next)->type!=LTINT)
    return ret;
  to=*(int*)ATOM_CAST(next->_next->_next)->data;
  diff=to-from;

  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      str=(char*)ATOM_CAST(next)->data;
      if(strlen(str)>=to&&
         strlen(str)>=from&&
         from<=to)/*make sure these indices are correct*/
      {

        ret.type=LTSTR;
        ret.data=(void*)ALLOC_STR(diff+2);
        strncpy((char*)ret.data,(const char*)&((char*)ATOM_CAST(next)->data)[from],diff+1);
        ((char*)ret.data)[diff+1]='\0';
        return ret;
      }
      return ret;
    case LTLIST:
      lst=slist_range((slist*)ATOM_CAST(next)->data,from,to);
      if(lst)
      {
        ret.type=LTLIST;
        ret.data=(void*)lst;
        return ret;
      }
      return ret;
    //case LTARRAY:
    //  la=ATOM_CAST(next);
    //  tarr=(lisp_array*)la->data;
    //  if(tarr->type!=LTARRAY)
    //    return ret;
    //  else
    //  {
    //    next=next->_next;

    //    /*traverse the array through each of its dimensions*/
    //    tarr=(lisp_array*)array_at(tarr,index);      
    //    while(tarr&&tarr->type==LTARRAY)
    //    {
    //      next=next->_next;
    //      if(next&&ATOM_CAST(next)->type==LTINT)
    //        index=*(int*)ATOM_CAST(next)->data;
    //      else return ret;
    //      tarr=(lisp_array*)array_at(tarr,index);
    //    }
    //    /*this will either be a terminal dimension (iee not LTARRAY) or null/nil*/
    //    if(tarr&&next&&next->_next)
    //      if(ATOM_CAST(next->_next)->type==LTINT&&
    //         ATOM_CAST(next->_next)->data!=0)
    //      {
    //        ret.type=tarr->type;
    //        ret.data=array_at(tarr,*(int*)ATOM_CAST(next->_next)->data);
    //        return ret;
    //      }
    //  }
    //  return ret;
    default:
      break;
  }
  return ret;
}
static lisp_atom lp_first(slist_elem* next)
{
  int index=0;
  char* str=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      str=(char*)ATOM_CAST(next)->data;
      if(strlen(str)>=index)
      {
        ret.type=LTSTR;
        ret.data=(void*)ALLOC_STR(2);
        memcpy(ret.data,(const void*)&((char*)ATOM_CAST(next)->data)[0],1);
        ((char*)ret.data)[1]='\0';
        return ret;
      }
      return ret;
    case LTLIST:
      la=(lisp_atom*)slist_at((slist*)ATOM_CAST(next)->data,1);
      if(la)
      {
        ret.type=la->type;
        ret.data=la->data;
        return ret;
      }
      return ret;
    case LTARRAY:
      //la=ATOM_CAST(next);
      //tarr=(lisp_array*)la->data;
      //if(tarr->type!=LTARRAY)
      //  return ret;
      //else
      //{
      //  next=next->_next;

      //  /*traverse the array through each of its dimensions*/
      //  tarr=(lisp_array*)array_at(tarr,index);      
      //  while(tarr&&tarr->type==LTARRAY)
      //  {
      //    next=next->_next;
      //    if(next&&ATOM_CAST(next)->type==LTINT)
      //      index=*(int*)ATOM_CAST(next)->data;
      //    else return ret;
      //    tarr=(lisp_array*)array_at(tarr,index);
      //  }
      //  /*this will either be a terminal dimension (iee not LTARRAY) or null/nil*/
      //  if(tarr&&next&&next->_next)
      //    if(ATOM_CAST(next->_next)->type==LTINT&&
      //       ATOM_CAST(next->_next)->data!=0)
      //    {
      //      ret.type=tarr->type;
      //      ret.data=array_at(tarr,*(int*)ATOM_CAST(next->_next)->data);
      //      return ret;
      //    }
      //}
      //return ret;
    default:
      break;
  }
              return ret; 
}
static lisp_atom lp_index(slist_elem* next)
{
  int index=0;
  char* str=0;
  lisp_array* tarr=0;
  lisp_atom* la=0;
  lisp_atom ret;
  ret.type=LTNIL;
  ret.data=(void*)LTNIL;

  switch(ATOM_CAST(next)->type)
  {
    case LTSTR:
      if(ATOM_CAST(next)->type!=LTSTR)
        return ret;
      str=(char*)ATOM_CAST(next)->data;
      ret.type=LTINT;
      ret.data=(void*)ALLOC_INT();
      *(int*)ret.data=strcspn(str,(char*)ATOM_CAST(next->_next)->data);
      return ret;
    case LTLIST:
      if(slist_find((slist*)ATOM_CAST(next)->data,
                    &index,
                    ATOM_CAST(next->_next),
                    atom_match))
      {
        ret.type=LTINT;
        ret.data=(void*)ALLOC_INT();
        *(int*)ret.data=index;
      }
      return ret;
    //case LTARRAY:
      //la=ATOM_CAST(next);
      //tarr=(lisp_array*)la->data;
      //if(tarr->type!=LTARRAY)
      //  return ret;
      //else
      //{
      //  next=next->_next;

      //  /*traverse the array through each of its dimensions*/
      //  tarr=(lisp_array*)array_at(tarr,index);      
      //  while(tarr&&tarr->type==LTARRAY)
      //  {
      //    next=next->_next;
      //    if(next&&ATOM_CAST(next)->type==LTINT)
      //      index=*(int*)ATOM_CAST(next)->data;
      //    else return ret;
      //    tarr=(lisp_array*)array_at(tarr,index);
      //  }
      //  /*this will either be a terminal dimension (iee not LTARRAY) or null/nil*/
      //  if(tarr&&next&&next->_next)
      //    if(ATOM_CAST(next->_next)->type==LTINT&&
      //       ATOM_CAST(next->_next)->data!=0)
      //    {
      //      ret.type=tarr->type;
      //      ret.data=array_at(tarr,*(int*)ATOM_CAST(next->_next)->data);
      //      return ret;
      //    }
      //}
      //return ret;
    default:
      break;
  }
  return ret; 
}
void load_access()
{
  lisp_install_symbol("at",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,2,CFN_ARGNOCIEL,lp_at)),0);
  lisp_install_symbol("between",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,3,3,lp_between)),0);
  lisp_install_symbol("first",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,1,1,lp_first)),0);
  lisp_install_symbol("index",(void*)new_atom(LENORMAL,LTCFNPTR,(void*)new_lisp_cfn(1,2,2,lp_index)),0);
}
