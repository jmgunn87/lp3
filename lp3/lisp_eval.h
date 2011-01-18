#ifndef LISP_EVAL_H
#define LISP_EVAL_H

#pragma once
#include "lisp_defs.h"

int lisp_eval(lisp_atom* atom,
              slist_elem* next);

#endif