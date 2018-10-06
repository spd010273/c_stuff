#ifndef FIFOQ_H
#define FIFOQ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _ALLOC(size) calloc(1,size)
#define _FREE(ptr) free(ptr)
#define _LOG(msg,args...) printf(msg,args)

struct fifoq_node {
    struct fifoq_node * next;
    struct fifoq_node * prev;
    void * data;
};

struct fifoq {
    unsigned int size;
    struct fifoq_node * first;
    struct fifoq_node * last;
};

extern struct fifoq * new_fifoq( void );
extern inline bool fifoq_empty( struct fifoq * ) __attribute__((always_inline));
extern inline unsigned int fifoq_size( struct fifoq * ) __attribute__((always_inline));
extern inline void fifoq_push( struct fifoq *, void * ) __attribute__((always_inline));
extern inline void fifoq_push_front( struct fifoq *, void * ) __attribute__((always_inline));
extern inline void * fifoq_pop( struct fifoq * ) __attribute__((always_inline));
extern inline void * fifoq_peek( struct fifoq * ) __attribute__((always_inline));
extern inline void * fifoq_pop_back( struct fifoq * ) __attribute__((always_inline));
extern inline void * fifoq_peek_back( struct fifoq * ) __attribute__((always_inline));
extern void free_fifoq( struct fifoq * );

#endif // FIFOQ_H
