#pragma once 
#include "stdafx.h"
#include "lisp_base.h"
#include "lisp_reader.h"
#include "lisp_math.h"
#include "lisp_symanip.h"
#include "lisp_cmp.h"
#include "lisp_flow.h"
#include "lisp_io.h"
#include "lisp_access.h"

static char _is_loaded=0;

void load_lisplib()
{
  if(_is_loaded)
    return;

  load_symanip();
  load_math_module();
  load_cmp();
  load_io();
  load_flow();
  load_access();

  _is_loaded=1;
}