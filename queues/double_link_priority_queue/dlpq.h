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

#ifndef DLPQ_H
#define DLPQ_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//#define DLPQ_DEBUG true

#define _ALLOC(size) calloc(1,size)
#define _FREE(ptr) free(ptr)
#define _LOG(msg,args...) printf(msg,args)

struct dlpq_node {
    void * value;
    struct dlpq_node * next;
    struct dlpq_node * prev;
};

struct dlpq {
    struct dlpq_node * first; // list of tree roots
    struct dlpq_node * last;
    unsigned int size;
    bool (*compare)( void *, void * );
#ifdef DLPQ_DEBUG
    void (*__dump_function)( void * ); // Debug object dumper
#endif // DLPQ_DEBUG
};

extern struct dlpq * new_dlpq( bool (*)( void *, void * ) );
extern inline unsigned int dlpq_size( struct dlpq * ) __attribute__((always_inline));
extern inline bool dlpq_empty( struct dlpq * ) __attribute__((always_inline));
extern void dlpq_push( struct dlpq * head, void * data );
extern inline void * dlpq_pop( struct dlpq * ) __attribute__((always_inline));
extern inline void * dlpq_unshift( struct dlpq * ) __attribute__((always_inline));
extern void free_dlpq( struct dlpq ** );
#ifdef DLPQ_DEBUG
extern void _dlpq_debug( struct dlpq * );
extern void _dlpq_setup_debug( struct dlpq *, void (*)( void * ) );
#endif // DLPQ_DEBUG
#endif // DLPQ_H
