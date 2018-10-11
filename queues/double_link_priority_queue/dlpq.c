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

struct dlpq * new_dlpq( bool (*compare_function)( void *, void * ) )
{
    struct dlpq * head = NULL;

    if( compare_function == NULL )
    {
        return NULL;
    }

    head = ( struct dlpq * ) _ALLOC( sizeof( struct dlpq ) );

    if( head == NULL )
    {
        return NULL;
    }

    head->first   = NULL;
    head->last    = NULL;
    head->size    = 0;
    head->compare = compare_function;

    return head;
}

inline unsigned int dlpq_size( struct dlpq * head )
{
    return ( head == NULL ) ? 0 : head->size;
}

inline bool dlpq_empty( struct dlpq * head )
{
    return ( head == NULL || head->size == 0 ) ? true : false;
}

void dlpq_push( struct dlpq * head, void * data )
{
    struct dlpq_node * new_node   = NULL;
    struct dlpq_node * last_node  = NULL;
    struct dlpq_node * start_node = NULL;

    if( head == NULL )
    {
        return;
    }

    new_node = _ALLOC( sizeof( struct dlpq_node ) );

    if( new_node == NULL )
    {
        return;
    }

    new_node->next  = NULL;
    new_node->prev  = NULL;
    new_node->value = data;

    if( dlpq_empty( head ) )
    {
        head->first = new_node;
        head->last  = new_node;
        head->size  = 1;
        return;
    }

    start_node = head->first;
    last_node  = start_node;
    head->size++;

    // Handle possible head replacement
    if( head->compare( data, start_node->value ) )
    {
        head->first = new_node;
        new_node->next = start_node;
        start_node->prev = new_node;
    }
    else
    {
        // Locate insertion point
        while( start_node != NULL )
        {
            if( head->compare( data, start_node->value ) )
            {
                last_node->next  = new_node;
                new_node->prev   = last_node;
                new_node->next   = start_node;
                start_node->prev = new_node;
                break;
            }

            last_node  = start_node;
            start_node = start_node->next;

            // Handle tail replacement
            if( start_node == NULL )
            {
                new_node->prev   = head->last;
                head->last->next = new_node;
                head->last       = new_node;
                new_node->next   = NULL;
                break;
            }
        }
    }

    return;
}

inline void * dlpq_pop( struct dlpq * head )
{
    // Take an element from head->first
    struct dlpq_node * temp = NULL;
    void *             data = NULL;

    if( dlpq_empty( head ) )
    {
        return NULL;
    }

    head->size--;

    temp        = head->first;
    data        = temp->value;
    head->first = temp->next;

    _FREE( temp );

    return data;
}

inline void * dlpq_unshift( struct dlpq * head )
{
    // Take an element head->last;
    struct dlpq_node * temp = NULL;
    void *             data = NULL;

    if( dlpq_empty( head ) )
    {
        return NULL;
    }

    head->size--;

    temp       = head->last;
    data       = temp->value;
    head->last = temp->prev;

    _FREE( temp );
    temp = NULL;

    return data;
}

void free_dlpq( struct dlpq ** head )
{
    struct dlpq_node * next_node = NULL;

    if( *head == NULL )
    {
        return;
    }

    if( dlpq_empty( *head ) && (*head) != NULL )
    {
        _FREE( *head );
        return;
    }

    next_node = (*head)->last;

    while( next_node != (*head)->first )
    {
        next_node = next_node->prev;
        _FREE( next_node->next );
    }

    if( next_node != NULL )
    {
        _FREE( next_node );
    }

    _FREE( (*head) );
    (*head) = NULL;
    return;
}

#ifdef DLPQ_DEBUG
void _dlpq_setup_debug( struct dlpq * head, void (*debug)( void * ) )
{
    if( head == NULL || debug == NULL || debug == NULL )
    {
        return;
    }

    head->__dump_function = debug;

    return;
}

void _dlpq_debug( struct dlpq * head )
{
    void *             data  = NULL;
    struct dlpq_node * temp  = NULL;
    unsigned int       index = 0;

    if( head == NULL )
    {
        _LOG( "dlpq head is %p\n", head );
    }

    temp = head->first;

    _LOG(
        "HEAD: %p, first: %p last: %p size: %u\n",
        head,
        head->last,
        head->first,
        head->size
    );

    while( temp != NULL )
    {
        _LOG(
            "Node #%u: %p prev %p next %p\n",
            index,
            temp,
            temp->prev,
            temp->next
        );

        if( head->__dump_function != NULL )
        {
            head->__dump_function( temp->value );
        }

        temp = temp->next;
        index++;
    }

    return;
}
#endif // DLPQ_DEBUG
