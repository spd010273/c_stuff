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

    free_fifoq( (struct fifoq ** ) &head );
    return 0;
}
