#include "dlpq.h"

bool less_than( void *, void * );
void dumper( void * );
int main( void );

bool less_than( void * a, void * b )
{
    if( (*(int *) a) < (*(int *) b ) )
    {
        return true;
    }

    return false;
}

void dumper( void * val )
{
    printf( "VALUE: %d\n", (*(int *) val));
    return;
}

int main( void )
{
    int a = 6;
    int b = 3;
    int c = 2;
    int d = 0;
    int e = 1;
    int f = 18;
    void * temp = NULL;
    struct dlpq * head = NULL;

    head = new_dlpq( &less_than );

    printf( "Init'd head is size %u\n", dlpq_size( head ) );

    dlpq_push( head, &a );
    dlpq_push( head, &b );
    dlpq_push( head, &c );
    dlpq_push( head, &d );
    dlpq_push( head, &e );
    dlpq_push( head, &f );

    printf( "Done pushing\n" );
    while( !dlpq_empty( head ) )
    {
        temp = dlpq_pop( head );
        printf( "Node %p val %d\n", temp, (*(int *) temp) );
    }

#ifdef DLPQ_DEBUG
    _dlpq_setup_debug( head, &dumper );
    _dlpq_debug( head );
#endif // DLPQ_DEBUG
    free_dlpq( head );
    head = NULL;

    return 0;
}
