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
    *e = 2;
    *f = 100;

    _slpq_setup_debug( head, &debug_function );

    slpq_push( head, (void *) a );
    slpq_push( head, (void *) b );
    slpq_push( head, (void *) c );
    slpq_push( head, (void *) d );
    slpq_push( head, (void *) e );
    slpq_push( head, (void *) f );

    _slpq_debug( head );
    free_slpq( head );

    if( head != NULL )
    {
        printf( "Failed to free head\n" );
        return -1;
    }

    free( a );
    free( b );
    free( c );
    free( d );
    free( e );
    free( f );

    return 0;
}
