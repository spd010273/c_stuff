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

struct fifoq * new_fifoq()
{
    struct fifoq * head = NULL;

    head = ( struct fifoq * ) _ALLOC( sizeof( struct fifoq ) );

    if( head == NULL )
    {
        _LOG( "Failed to allocate memory for fifoq: got %p", head );
        return NULL;
    }

    head->size  = 0;
    head->first = NULL;
    head->last  = NULL;

    return head;
}

inline bool fifoq_empty( struct fifoq * head )
{
    return ( head == NULL || head->size == 0 ) ? true : false;
}

inline unsigned int fifoq_size( struct fifoq * head )
{
    return ( head == NULL ) ? 0 : head->size;
}

inline void fifoq_push( struct fifoq * head, void * data )
{
    struct fifoq_node * node = NULL;
    struct fifoq_node * temp = NULL;

    if( head == NULL || data == NULL )
    {
        return;
    }

    node = ( struct fifoq_node * ) _ALLOC( sizeof( struct fifoq_node ) );

    if( node == NULL )
    {
        _LOG( "Failed to allocate memory for new fifoq_node: got %p", node );
        return;
    }

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    if( head->size == 0 )
    {
        head->first = node;
        head->last  = node;
    }
    else
    {
        temp       = head->last;
        temp->next = node;
        node->prev = temp;
        head->last = node;
    }

    head->size++;

    return;
}

inline void fifoq_push_front( struct fifoq * head, void * data )
{
    struct fifoq_node * node = NULL;
    struct fifoq_node * temp = NULL;

    if( head == NULL || data == NULL )
    {
        return;
    }

    node = ( struct fifoq_node * ) _ALLOC( sizeof( struct fifoq_node ) );

    if( node == NULL )
    {
        _LOG( "Failed to allocate memory for new fifoq_node: got %p", node );
        return;
    }

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    if( head->size == 0 )
    {
        head->first = node;
        head->last  = node;
    }
    else
    {
        temp        = head->first;
        temp->prev  = node;
        node->next  = temp;
        head->first = node;
    }

    head->size++;

    return;
}

inline void * fifoq_pop( struct fifoq * head )
{
    struct fifoq_node * temp = NULL;
    void *              data = NULL;

    if( fifoq_empty( head ) )
    {
        return NULL;
    }

    temp = head->first;
    data = temp->data;

    if( head->size == 1 )
    {
        // We just popped the last node
        head->first = NULL;
        head->last  = NULL;
    }
    else
    {
        head->first       = temp->next;
        head->first->prev = NULL;
    }

    _FREE( temp );
    temp = NULL;
    head->size--;

    return data;
}

inline void * fifoq_peek( struct fifoq * head )
{
    if( fifoq_empty( head ) )
    {
        return NULL;
    }

    return head->first->data;
}

inline void * fifoq_pop_back( struct fifoq * head )
{
    struct fifoq_node * temp = NULL;
    void *              data = NULL;

    if( fifoq_empty( head ) )
    {
        return NULL;
    }

    temp = head->last;
    data = temp->data;

    if( head->size == 1 )
    {
        head->first = NULL;
        head->last  = NULL;
    }
    else
    {
        head->last       = temp->prev;
        head->last->next = NULL;
    }

    _FREE( temp );
    temp = NULL;
    head->size--;

    return data;
}

inline void * fifoq_peek_back( struct fifoq * head )
{
    if( fifoq_empty( head ) )
    {
        return NULL;
    }

    return head->last->data;
}

void free_fifoq( struct fifoq ** head )
{
    struct fifoq_node * node = NULL;
    struct fifoq_node * last = NULL;

    if( fifoq_empty( (*head) ) )
    {
        if( (*head) != NULL )
        {
            _FREE( (*head) );
        }

        return;
    }

    node = (*head)->first;

    while( node != NULL )
    {
        last = node;
        node = node->next;
        _FREE( last );
    }

    _FREE( (*head) );
    (*head) = NULL;
    return;
}
