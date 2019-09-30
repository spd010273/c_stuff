#include "binoq.h"

struct binoq * new_binoq( bool (*compare)( void *, void * ) )
{
    struct binoq *    head = NULL;
    struct linked_l * heap = NULL;

    if( compare == NULL )
    {
        return NULL;
    }

    head = ( struct binoq * ) _ALLOC( sizeof( struct binoq ) );

    if( head == NULL )
    {
        return NULL;
    }

    heap = ( struct linked_l * ) _ALLOC( sizeof( struct linked_l ) );

    if( heap == NULL )
    {
        _FREE( head );
        return NULL;
    }

    heap->size    = 0;
    heap->head    = NULL;
    heap->tail    = NULL;
    head->roots   = heap;
    head->compare = compare;

    return head;
}

void free_binoq( struct binoq * head )
{
    if( head == NULL )
    {
        return;
    }

    if( head->roots == NULL )
    {
        _FREE( head );
        return;
    }

    if( head->roots->size == 0 )
    {
        _FREE( head->roots );
        _FREE( head );
        return;
    }

    return;
}

void binoq_push( struct binoq * head, void * data )
{
    struct binoq_node * new_node = NULL;

    if( head == NULL || data == NULL )
    {
        return;
    }

    new_node = ( struct binoq_node * ) _ALLOC( sizeof( struct binoq_node ) );

    if( new_node == NULL )
    {
        return;
    }

    new_node->data = data;

    if( head->roots->size == 0 )
    {
       
    }
}


void free_linked_l( struct linked_l * ll )
{
    struct linked_node * temp = NULL;

    if( ll == NULL )
    {
        return;
    }

    while( ll->head != NULL )
    {
        temp = ll->head;
        ll->head = ll->head->next;
        free_linked_node( temp );
    }

    _FREE( ll );
    return;
}

void free_binoq_node( struct binoq_node * node )
{
    if( node == NULL )
    {
        return;
    }

    free_linked_l( node->children );
    _FREE( node );
    return;
}

void free_linked_node( struct linked_node * ln )
{
    if( ln == NULL )
    {
        return;
    }

    free_binoq_node( ln->current );
    _FREE( ln );
    return;
}

void init_linked_l( struct linked_l * ll )
{
    if( ll == NULL )
    {
        return;
    }

    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;
    return;
}

void init_binoq_node( struct binoq_node * node, void * data )
{
    if( binoq_node == NULL || data == NULL )
    {
        return;
    }

    n->children = ( struct linked_l * ) _ALLOC( sizeof( struct linked_l ) );

    init_linked_l( n->children );

    n->parent = NULL;
    n->data   = data;
    n->rank   = 0;
    return;
}

void init_binoq( struct binoq * head, struct linked_l * ll )
{
    struct linked_node * iterator = NULL;

    if( head == NULL || ll == NULL )
    {
        return;
    }

    head->roots = ll;

    if( ll->head == NULL )
    {
        return;
    }

    iterator = ll->head->next;

    while( iterator != NULL )
    {
        if( head->compare( iterator->current->data, head->minimum->data ) )
        {
            binoq->minimum = iterator->current;
        }

        iterator = iterator->next;
    }

    return;
}

void remove_linked_l( struct linked_l * ll, struct binoq_node * node )
{
    struct linked_node * temp     = NULL;
    struct linked_node * prev     = NULL;
    struct linked_node * iterator = NULL;

    if( ll == NULL || node == NULL )
    {
        return;
    }

    if( ll->head->current == node )
    {
        temp = ll->head;

        ll->head = ll->head->next;
        ll->size = ll->size - 1;
        _FREE( temp );
        return;
    }

    prev     = ll->head;
    iterator = previous->next;

    while( iterator != NULL )
    {
        if( iterator->current == node )
        {
            prev->next = iterator->next;
            ll->size   = ll->size - 1;
            _FREE( iterator );
            return;
        }

        prev     = iterator;
        iterator = iterator->next;
    }

    return;
}

void insert_linked_l( struct linked_l * ll, struct binoq_node * node )
{
    struct linked_node * l_node   = NULL;
    struct linked_node * iterator = NULL;
    struct linked_node * prev     = NULL;

    if( ll == NULL || node == NULL )
    {
        return;
    }

    l_node = ( struct linked_node * ) _ALLOC( sizeof( struct linked_node ) );

    if( l_node == NULL )
    {
        return;
    }

    l_node->current = node;
    l_node->next    = NULL;

    if( ll->size == 0 )
    {
        ll->head = l_node;
        ll->tail = l_node;
        ll->size = 1;
        return;
    }

    while( iterator != NULL )
    {
        if( node->rank < iterator->current->rank )
        {
            break;
        }

        prev     = iterator;
        iterator = iterator->next;
    }

    if( iterator == ll->head )
    {
        l_node->next = ll->head;
        ll->head     = l_node;
    }
    else
    {
        prev->next   = l_node;
        l_node->next = iterator;
    }

    ll->size = ll->size + 1;

    return;
}

{
    if( p == NULL || q == NULL )
    {
        return 0;
    }

    if( 
}
