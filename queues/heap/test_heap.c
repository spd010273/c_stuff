#include "heap.h"

inline bool compare( void *, void * );
int main( void );
void debug( void * );

inline bool compare( void * a, void * b )
{
    if( a == NULL || b == NULL )
    {
        return false;
    }

    return (*(unsigned int *)a) > (*(unsigned int *)b);
}

int main( void )
{
    struct heap * test = NULL;
    unsigned int * foo = NULL;
    foo = malloc( sizeof( unsigned int ) * 7 );

    if( foo == NULL )
    {
        return 1;
    }

    void * test_node = NULL;

    foo[0] = 1;
    foo[1] = 2;
    foo[2] = 3;
    foo[3] = 4;
    foo[4] = 6;
    foo[5] = 12;
    foo[6] = 454;

    test = new_heap( compare );

    if( test == NULL )
    {
        return 1;
    }

    heap_insert( test, &(foo[0]) );
    heap_insert( test, &(foo[1]) );
    heap_insert( test, &(foo[2]) );
    heap_insert( test, &(foo[3]) );
    heap_insert( test, &(foo[6]) );
    heap_insert( test, &(foo[5]) );
    heap_insert( test, &(foo[4]) );

#ifdef DEBUG
    test->debug = debug;
    debug_heap( test );
#endif
    while( test->in_use > 0 )
    {
        test_node = heap_min( test );
        printf( "Got val %p: %u\n", test_node, test_node == NULL ? 0 : *(unsigned int *) test_node);
        heap_delete_min( test );
        printf( "deleted that position\n" );
    }
    return 0;
}

void debug( void * a )
{
    printf( "%p, %u\n", a, *(unsigned int *)a);
    return;
}
