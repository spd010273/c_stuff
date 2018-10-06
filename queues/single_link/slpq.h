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

#ifndef SLPQ_H
#define SLPQ_H

/*
 * Minimum includes for default memory management and logging macros
 *
 * Uncomment if you are rolling your own
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define __ALLOC(size) calloc(1,size)
#define __FREE(ptr) free(ptr)
#define __REALLOC(ptr,size) realloc(ptr,size)
#define __LOG(msg,...) printf(msg,__VA_ARGS__)

//#define SLPQ_DEBUG true

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

extern struct slpq * new_slpq( bool (*)( void *, void * ) );
extern void free_slpq( struct slpq * );
extern inline unsigned int slpq_size( struct slpq * ) __attribute__((always_inline))__;
extern inline bool slpq_empty( struct slpq * ) __attribute__((always_inline));
extern inline void * slpq_pop( struct slpq * ) __attribute__((always_inline));
extern void slpq_push( struct slpq *, void * );

#ifdef SLPQ_DEBUG
extern void _slpq_debug( struct slpq * );
extern void _slpq_setup_debug( struct slpq *, void (*)( void * ) );
#endif // SLPQ_DEBUG

#endif // SLPQ_H
