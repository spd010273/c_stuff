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

#ifndef FIBQ_H
#define FIBQ_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define __ALLOC(size) calloc(1,size)
#define __FREE(ptr) free(ptr)
#define __REALLOC(ptr,size) realloc(ptr,size)
#define __LOG(msg,...) printf(msg,__VA_ARGS__)

//#define FIBQ_DEBUG true

struct fibq_node {
    void * data;
    unsigned int rank;
    bool marked;
    struct fibq_node * child;
    struct fibq_node * left;
    struct fibq_node * right;
    struct fibq_node * parent;
};

struct fibq {
    struct fibq_node ** heap;
    bool (*compare_function)( void *, void * );
    void (*__debug_function)( void * );
    struct fibq_node * min_node;
    void * min_val;
    unsigned int node_count;
    unsigned int mark_count;
    unsigned int heap_size;
};


extern struct fibq * new_fibq( bool (*)( void *, void * ) );
void _free_fibq_node( struct fibq_node * );
extern void free_fibq( struct fibq * );
extern void fibq_insert( struct fibq *, void * );
extern struct fibq * fibq_merge( struct fibq *, struct fibq * );
#endif // FIBQ_H
