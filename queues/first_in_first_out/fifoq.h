/*
 *    Copyright 2018 Chris Autry
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy of
 *    this software and associated documentation files (the "Software"), to deal in
 *    the Software without restriction, including without limitation the rights to
 *    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *    the Software, and to permit persons to whom the Software is furnished to do so,
 *    subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
extern void free_fifoq( struct fifoq ** );

#endif // FIFOQ_H
