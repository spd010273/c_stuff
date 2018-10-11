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

#include "fibq.h"

struct fibq * new_fibq( bool (*compare_function)( void *, void * ) )
{
    struct fibq * new_fibq = NULL;

    if( compare_function == NULL )
    {
        _LOG( "Invalid comparison function: %p\n", compare_function );
        return NULL;
    }

    new_fibq = ( struct fibq * ) __ALLOC( sizeof( struct fibq ) );

    if( new_fibq == NULL )
    {
        __LOG(
            "Failed to allocate memory for new "\
            "fibonacci priority queue: got %p\n",
            new_fibq
        );

        return NULL;
    }

    new_fibq->heap             = NULL;
    new_fibq->min_node         = NULL;
    new_fibq->compare_function = compare_function;
    new_fibq->node_count       = 0;
    new_fibq->mark_count       = 0;
    new_fibq->heap_size        = 0;

    return new_fibq;
}

void _free_fibq_node( struct fibq_node * node )
{
    if( node == NULL )
    {
        return;
    }

    if( node->left != NULL )
    {
        _free_fibq_node( node->left );
    }

    if( node->right != NULL )
    {
        _free_fibq_node( node->right );
    }

    if( node->child != NULL )
    {
        _free_fibq_node( node->child );
    }

    __FREE( node );
    return;
}

void free_fibq( struct fibq * head )
{
    unsigned int i = 0;

    if( head == NULL )
    {
        return;
    }

    for( i = 0; i < head->heap_size; i++ )
    {
        _free_fibq_node( head->heap[i] );
    }

    __FREE( head->heap );
    __FREE( head );
    
    return; 
}

void fibq_insert( struct fibq * head, void * data )
{
    struct fibq_node * new_node = NULL;

    if( head == NULL || data == NULL )
    {
        return;
    }

    if( head->heap == NULL )
    {
        head->heap = ( struct fibq_node ** ) __ALLOC(
            sizeof( struct fibq_node * )
        );
    }
    else
    {
        head->heap = ( struct fibq_node ** ) __REALLOC(
            head->heap,
            sizeof( struct fibq_node * )
          * ( head->heap_size + 1 )
        );
    }

    if( head->heap == NULL )
    {
        __LOG( "Failed to extend fibq heap: got %p\n", head->heap );
        return;
    }

    head->heap[head->heap_size] = NULL;

    new_node = ( struct fibq_node * ) __ALLOC( sizeof( struct fibq_node ) );

    if( new_node == NULL )
    {
        __LOG(
            "Failed to allocate memory for new fibq node: got %p\n",
            new_node
        );
        return;
    }

    new_node->child  = NULL;
    new_node->right  = NULL;
    new_node->left   = NULL;
    new_node->marked = false;
    new_node->rank   = 0;
    new_node->data   = data;

    if( head->min_node == NULL )
    {
        head->min_node = new_node;
    }
    else
    {
        if( head->compare_function( new_node->data, head->min_node->data ) )
        {
            head->min_node = new_node;
            head->min_Val  = new_node->data;
        }
    }

    head->heap[head->heap_size] = new_node;
    head->node_count++;
    head->heap_size++;

    return;
}

struct fibq * fibq_merge( struct fibq * a_head, struct fibq * b_head )
{
    if( a_head == NULL || b_head == NULL )
    {
        if( a_head == NULL && b_head == NULL )
        {
            return NULL;
        }
        else if( a_head == NULL )
        {
            return b_head;
        }
        else
        {
            return a_head;
        }
    }

    if( a_head->compare_function != b_head->compare_function )
    {
        __LOG(
            "Cannot merge two fibqs with "\
            "different comparison functions %p, %p\n",
            a_head->compare_function,
            b_head->compare_function
        );

        return NULL;
    }

}
