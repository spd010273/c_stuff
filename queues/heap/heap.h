#ifndef HEAP_H
#define HEAP_H

#define HEAP_DEBUG DEBUG
#define HEAP_CHUNK_SIZE 16

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
#include "postgres.h"

#define _ALLOC(size) palloc0(size)
#define _FREE(ptr) pfree(ptr)
#define _REALLOC(ptr,size) repalloc(ptr,size)
#define _LOG(msg,args...) elog(DEBUG1,msg,args)
*/

#define _ALLOC(size) calloc(1,size)
#define _FREE(ptr) free(ptr)
#define _LOG(msg,args...) printf(msg,args)
#define _REALLOC(ptr,size) realloc(ptr,size)

struct heap {
#ifdef HEAP_DEBUG
    void         (*debug)( void * );
#endif
    bool         (*compare)( void *, void * );
    unsigned int in_use;
    unsigned int size;
    void **      heap;
};

extern struct heap * new_heap( bool (*)( void *, void * ) );
extern void heap_insert( struct heap *, void * );
extern void * heap_min( struct heap * );
extern void heap_delete_min( struct heap * );
extern void free_heap( struct heap * );
extern inline bool heap_empty( struct heap * );
extern inline unsigned int heap_size( struct heap * );

extern inline unsigned int _left_child( unsigned int );
extern inline unsigned int _right_child( unsigned int );
extern inline unsigned int _parent( unsigned int );

#ifdef HEAP_DEBUG
void debug_heap( struct heap * );
#endif // HEAP_DEBUG
#endif // HEAP_H
