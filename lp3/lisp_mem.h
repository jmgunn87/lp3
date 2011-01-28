#ifndef LISP_MEM_H
#define LISP_MEM_H

#pragma once

void init_mem_pools(size_t num_pools);
char assign_mem_pool(size_t chunk_size,
                     size_t count);
void* pallocate(size_t size);
void pdeallocate(void* elem);

#endif