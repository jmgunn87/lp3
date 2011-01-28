#pragma once 
#include "stdafx.h"
#include "lisp_mem.h"
#include <assert.h>

/* 
 
 slists, slist_elems, lisp_atoms,
 char*,
 floats,
 ints,
 arrays,

 */
#define TREE_PARENT(pos) ( (pos-1)/2 )
#define TREE_LEFT(pos)   ( (pos*2)+1 )
#define TREE_RIGHT(pos)  ( (pos*2)+2 )

typedef struct _mem_chunk
{
  char is_free;
  char* chunk;

} mem_chunk;

typedef struct _mem_pool
{
  size_t chunk_size;
  size_t chunk_count;
  size_t last_free_index;
  mem_chunk* its_pool;

} mem_pool;

static size_t pool_count=0;
static mem_pool* _gmem_pool=0;

void init_mem_pools(size_t num_pools)
{
  if(!_gmem_pool)
  {
    _gmem_pool=(mem_pool*)calloc(num_pools,sizeof(mem_pool));
    pool_count=num_pools;
  }
  assert(_gmem_pool[0].its_pool==0);
}
char assign_mem_pool(size_t chunk_size,
                     size_t count)
{
  int i=0;
  int c=0;
  assert(_gmem_pool);
  for(;i<pool_count;++i)
    if(!_gmem_pool[i].its_pool)
    {
      _gmem_pool[i].chunk_size=chunk_size;
      _gmem_pool[i].chunk_count=count;
      _gmem_pool[i].last_free_index=0;
      _gmem_pool[i].its_pool=(mem_chunk*)malloc(count*sizeof(mem_chunk));
      for(;c<count;++c)
      {
        _gmem_pool[i].its_pool[c].is_free=true;
        _gmem_pool[i].its_pool[c].chunk=(char*)malloc(chunk_size);
      }
      return 1;
    }
  return 0;
}

void* pallocate(size_t size)
{
  int p=0;
  int c=0;

  assert(_gmem_pool);
  for(;p<pool_count;++p)/*for each pool*/
    if(_gmem_pool[p].chunk_size==size)/*find the appropriate size*/
      for(c=0;c<_gmem_pool[p].chunk_count;++c)/*for each chunk, find a chunk*/
        if(_gmem_pool[p].its_pool[c].is_free)/*if its free allocate*/
        {
          _gmem_pool[p].its_pool[c].is_free=false;
          return (void*)_gmem_pool[p].its_pool[c].chunk;
        }
  return 0;
}

void pdeallocate(void* elem)
{
  int p=0;
  int c=0;
  size_t size=strlen((char*)elem);

  assert(_gmem_pool);
  for(;p<pool_count;++p)/*for each pool*/
    for(c=0;c<_gmem_pool[p].chunk_count;++c)/*for each chunk, find a chunk*/
      if(!_gmem_pool[p].its_pool[c].is_free&&
         (int)_gmem_pool[p].its_pool[c].chunk==(int)elem)
      {
        _gmem_pool[p].its_pool[c].is_free=0;
        memset((void*)_gmem_pool[p].its_pool[c].chunk,0,_gmem_pool[p].chunk_size);
        *(char*)elem=0;
      }
  return;
}