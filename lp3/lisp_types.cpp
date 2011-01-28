#pragma once
#include "stdafx.h"
#include "lisp_types.h"

lisp_atom* new_atom(LISP_TYPE type, void* data)
{
  lisp_atom* la=(lisp_atom*)malloc(sizeof(lisp_atom));
  la->type=type;
  la->data=data;
  return la;
}

lisp_atom* atom_copy(lisp_atom* atom)
{
  slist* tlist=0;
  slist_elem* tle=0;
  lisp_atom* copy=0;

  copy=new_atom(LTNIL,(void*)LTNIL);
  if(!atom)
    return copy;

  switch(atom->type)
  {
  case LTTRUE:
    (*copy).type=LTTRUE;
    (*copy).data=(void*)LTTRUE;
    return copy;
  case LTID:
    (*copy).type=LTID;
    (*copy).data=malloc(strlen((char*)atom->data)+1);
    ((char*)(*copy).data)[strlen((char*)atom->data)]='\0';
    strcpy((char*)(*copy).data,(char*)atom->data);
    return copy;
  case LTINT:
    (*copy).type=LTINT;
    (*copy).data=malloc(sizeof(int));
    *(int*)(*copy).data=*(int*)atom->data;
    return copy;
  case LTFLOAT:
    (*copy).type=LTFLOAT;
    (*copy).data=malloc(sizeof(float));
    *(float*)(*copy).data=*(float*)atom->data;
    return copy;
  case LTSTR:
    (*copy).type=LTSTR;
    (*copy).data=malloc(strlen((char*)atom->data)+1);
    ((char*)(*copy).data)[strlen((char*)atom->data)]='\0';
    strcpy((char*)(*copy).data,(char*)atom->data);
    return copy;
  case LTLIST:
    (*copy).type=LTLIST;
    (*copy).data=(void*)new_slist();
    tlist=(slist*)atom->data;
    tle=tlist->_head;
    while(tle)
    {
      slist_append((slist*)(*copy).data,
        (void*)atom_copy((lisp_atom*)tle->_data));
      tle=tle->_next;
    }
    return copy;
  case LTLISPFN:
    (*copy).type=LTLISPFN;
    (*copy).data=(void*)new_slist();
    return copy;
  case LTLISPMACRO:
    (*copy).type=LTLISPMACRO;
    (*copy).data=(void*)new_slist();
    return copy;
  case LTARRAY:
    (*copy).type=LTARRAY;
    (*copy).data=0;
    return copy;
  case LTCFNPTR:
    (*copy).type=LTCFNPTR;
    (*copy).data=(void*)new_lisp_cfn(
      ((lisp_cfn*)atom->data)->eval_args,
      ((lisp_cfn*)atom->data)->argc_floor,
      ((lisp_cfn*)atom->data)->argc_ciel,
      ((lisp_cfn*)atom->data)->its_fnptr);
    return copy;
  case LTNIL:
  default:
    return copy;
  }
}
void atom_destroy(lisp_atom* atom)
{
  slist* tlist=0;
  slist_elem* tle=0;
  
  if(!atom)
    return;

  while(1)
    switch(atom->type)
    {
    //case LTID:
    //  (*copy).type=LTID;
    //  (*copy).data=malloc(strlen((char*)atom->data)+1);
    //  ((char*)(*copy).data)[strlen((char*)atom->data)]='\0';
    //  strcpy((char*)(*copy).data,(char*)atom->data);
    //  return copy;
    case LTINT:
    case LTFLOAT:
    case LTSTR:
      free((*atom).data);
      return;
    case LTLISPFN:
    case LTLISPMACRO:
    case LTLIST:
      tlist=(slist*)atom->data;
      tle=tlist->_head;
      while(tle)
      {
        atom_destroy((lisp_atom*)tle->_data);
        tle=tle->_next;
      }
      slist_destroy(tlist);
      return;
      //    case LTARRAY:
//      (*copy).type=LTARRAY;
//      (*copy).data=0;
//      return copy;
    case LTCFNPTR:
      free((*atom).data);
      return;
    default:
      return;
    }
  free(atom);
  atom=0;
}
lisp_cfn* new_lisp_cfn(char eval,
                       int floor,
                       int ciel,
                       lisp_atom(*fnptr)(slist_elem*))
{
  lisp_cfn* lcf=(lisp_cfn*)malloc(sizeof(lisp_cfn));
  lcf->eval_args=eval;
  lcf->argc_floor=floor;
  lcf->argc_ciel=ciel;
  lcf->its_fnptr=fnptr;
  return lcf;
}

lisp_array* new_lisp_array(LISP_TYPE type,
                           int length,
                           void* data)
{
  lisp_array* la=(lisp_array*)malloc(sizeof(lisp_array));
  la->type=type;
  la->length=length;
  if(!data)  
    la->data=calloc(length,sizeof(void*));
  else la->data=data;
  return la;
}

