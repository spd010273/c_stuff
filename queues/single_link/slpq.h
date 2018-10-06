#ifndef SLPQ_H
#define SLPQ_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define __ALLOC(size) calloc(1,size)
#define __FREE(ptr) free(ptr)
#define __REALLOC(ptr,size) realloc(ptr,size)
#define __LOG(msg,...) printf(msg,__VA_ARGS__)

#define SLPQ_DEBUG true

struct slpq_node {
    void * data;
    struct slpq_node * next;
};

struct slpq {
    struct slpq_node * head;
    unsigned int size;
    bool (*compare_function)( void *, void * );
#ifdef SLPQ_DEBUG
    void (*__debug_function)( void * );
#endif // SLPQ_DEBUG
};

struct slpq * new_slpq( bool (*)( void *, void * ) );
void free_slpq( struct slpq * );
unsigned int slpq_size( struct slpq * );
bool slpq_empty( struct slpq * );
void slpq_push( struct slpq *, void * );
void * slpq_pop( struct slpq * );

#ifdef SLPQ_DEBUG
void _slpq_debug( struct slpq * );
void _slpq_setup_debug( struct slpq *, void (*)( void * ) );
#endif // SLPQ_DEBUG

#endif // SLPQ_H
