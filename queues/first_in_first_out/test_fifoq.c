#include "fifoq.h"

int main( void );

int main()
{
    struct fifoq * head = NULL;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    void * temp = NULL;
    unsigned int ind = 0;

    head = new_fifoq();
    fifoq_push( head, (void * )&a );
    fifoq_push( head, (void * )&b );
    fifoq_push( head, (void * )&c );
    fifoq_push( head, (void * )&d );

    if( fifoq_empty( head ) )
    {
        printf( "queue empty :(" );
        return -1;
    }

    printf( "Done pushing\n" );
    printf( "Head %p, first: %p, last %p, size %u\n", head, head->first, head->last, head->size );

    temp = fifoq_peek( head );
    printf( "Front: %p val: %d\n", temp, (*(int *) temp ) );
    temp = fifoq_peek_back( head );
    printf( "Back: %p val: %d\n", temp, (*(int *) temp ) );

    printf( "Fifoq contents:\n" );
    while( !fifoq_empty( head ) )
    {
        temp = fifoq_pop( head );
        printf( "entry #%d, %p:%d\n", ind, temp, (*(int *) temp ) );
        ind++;
    }

    fifoq_push_front( head, (void *)&d );
    fifoq_push_front( head, (void *)&c );
    fifoq_push_front( head, (void *)&b );
    fifoq_push_front( head, (void *)&a );

    printf( "Fifoq contents:\n" );
    while( !fifoq_empty( head ) )
    {
        temp = fifoq_pop_back( head );
        printf( "entry #%d, %p:%d\n", ind, temp, (*(int *) temp ) );
        ind++;
    }

    free_fifoq( head );
    return 0;
}
