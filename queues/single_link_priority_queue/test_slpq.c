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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "slpq.h"

bool greater_than( void *, void * );
int main( void );
void debug_function( void * );

bool greater_than( void * a, void * b )
{
    if( a == NULL || b == NULL )
    {
        printf( "compare function received null pointer: a %p, b %p\n", a, b );
        return false;
    }

    if( (*(int *) a) > (*(int * ) b) )
    {
        return true;
    }

    return false;
}

void debug_function( void * data )
{
    if( data == NULL )
    {
        printf( "val: NULL\n" );
    }
    else
    {
        printf( "val: %d\n", (*(int *) data) );
    }

    return;
}

int main( void )
{
    struct slpq * head = NULL;
    int *         a    = NULL;
    int *         b    = NULL;
    int *         c    = NULL;
    int *         d    = NULL;
    int *         e    = NULL;
    int *         f    = NULL;
    void *        val  = NULL;

    a = calloc( 1, sizeof( int ) );
    b = calloc( 1, sizeof( int ) );
    c = calloc( 1, sizeof( int ) );
    d = calloc( 1, sizeof( int ) );
    e = calloc( 1, sizeof( int ) );
    f = calloc( 1, sizeof( int ) );

    if( a == NULL || b == NULL || c == NULL || d == NULL || e == NULL || f == NULL )
    {
        printf( "Failed to allocate memory for test variabled\n" );
        return -1;
    }

    head = new_slpq( &greater_than );

    if( head == NULL )
    {
        printf( "Received a null head :(\n" );
        return -1;
    }

    *a = 5;
    *b = 1;
    *c = 12;
    *d = 9;
    *e = 1;
    *f = 100;
    
#ifdef SLPQ_DEBUG
    _slpq_setup_debug( head, &debug_function );
#endif // SLPQ_DEBUG


    slpq_push( head, (void *) a );
    slpq_push( head, (void *) b );
    slpq_push( head, (void *) c );
    slpq_push( head, (void *) d );
    slpq_push( head, (void *) e );
    slpq_push( head, (void *) f );

#ifdef SLPQ_DEBUG
    _slpq_debug( head );
#endif // SLPQ_DEBUG

    while( !slpq_empty( head ) )
    {
        val = slpq_pop( head );
        printf( "Node %p val %d\n", val, (*(int *) val) );
    }

    free_slpq( (struct slpq **) &head );
    head = NULL;
    
    free( a );
    free( b );
    free( c );
    free( d );
    free( e );
    free( f );

    a = NULL;
    b = NULL;
    c = NULL;
    d = NULL;
    e = NULL;
    f = NULL;

    return 0;
}
