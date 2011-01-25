#ifndef LISP_ARRAY_H
#define LISP_ARRAY_H

#pragma once
#include "lisp_base.h"

typedef struct _lisp_array
{
  LISP_TYPE type;
  unsigned int length;
  void* data;

} lisp_array;

lisp_array* new_lisp_array(LISP_TYPE type,
                           int length,
                           void* data);
void array_type_depth(const lisp_array* larray,
                      LISP_TYPE* type,
                      int* depth);
int array_depth_step(const lisp_array* larray,
                     int depth,
                     int width);
void* array_at(lisp_array* la,
               int index);

#endif