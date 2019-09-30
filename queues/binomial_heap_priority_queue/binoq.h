#ifndef BINOQ_H
#define BINOQ_H

#include "postgres.h"

#define _ALLOC(size) palloc0(size)
#define _FREE(ptr) pfree(ptr)
#define _REALLOC(ptr,size) repalloc(ptr,size)
#define _LOG(msg,args...) elog(DEBUG1,msg,args)

#define BINOQ_DEBUG DEBUG

struct linked_node {
    struct binoq_node * current;
    struct linked_node * next;
};

struct binoq_node {
    void * data;
    struct binoq_node * parent;
    struct linked_l * children;
    unsigned int rank;
};

struct linked_l {
    struct linked_node * head;
    struct linked_node * tail;
    unsigned int size;
};

struct binoq {
    struct linked_l * roots;
    struct binoq_node * minimum;
    bool (*compare)(void *, void *);
};

struct binoq * new_binoq( bool (*)( void *, void * ) );
void free_binoq( struct binoq * );
void binoq_push( struct binoq *, void * );
void * binoq_pop( struct binoq * );
unsigned int binoq_size( struct binoq * );
bool binoq_empty( struct binoq * );

// Helper functions
void free_linked_l( struct linked_l * );
void free_binoq_node( struct binoq_node * );
void free_linked_node( struct linked_node * );

void init_linked_l( struct linked_l * );
void init_binoq_node( struct binoq_node *, void * );
void init_binoq( struct binoq *, struct linked_l * );

void remove_linked_l( struct linked_l *, struct binoq_node * );
void insert_linked_l( struct linked_l *, struct binoq_node * );
#endif // BINOQ_H
