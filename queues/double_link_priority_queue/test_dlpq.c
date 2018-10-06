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
