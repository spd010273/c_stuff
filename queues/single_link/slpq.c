#include "slpq.h"

struct slpq * new_slpq( bool (*compare_function)( void *, void * ) )
{
    struct slpq * new_head = NULL;

    if( compare_function == NULL )
    {
        __LOG( "Compare function is %p\n", compare_function );
        return NULL;
    }

    new_head = __ALLOC( sizeof( struct slpq ) );

    if( new_head == NULL )
    {
        __LOG( "Could not allocate memory for slpq: got %p\n", new_head );
        return NULL;
    }

    new_head->size             = 0;
    new_head->compare_function = compare_function;
    new_head->head             = NULL;

    return new_head;
}

void free_slpq( struct slpq * head )
{
    struct slpq_node * node = NULL;
    struct slpq_node * last = NULL;

    if( head == NULL )
    {
        return;
    }

    node = head->head;

    while( node != NULL )
    {
        last = node;
        node = node->next;
        __FREE( last );
        last = NULL;
    }

    __FREE( head );
    head = NULL;

    return;
}

unsigned int slpq_size( struct slpq * head )
{
    unsigned int       index = 0;
    struct slpq_node * node  = NULL;

    if( slpq_empty( head ) )
    {
        return 0;
    }

    node = head->head;

    while( node->next != NULL )
    {
        node = node->next;
        index++;
    }

    return index;
}

bool slpq_empty( struct slpq * head )
{
    if( head == NULL || head->head == NULL )
    {
        return true;
    }

    return false;
}

void slpq_push( struct slpq * head, void * data )
{
    struct slpq_node * node = NULL;
    struct slpq_node * last = NULL;
    struct slpq_node * new_node = NULL;

    if( head == NULL || data == NULL || head->compare_function == NULL )
    {
        return;
    }

    new_node = ( struct slpq_node * ) __ALLOC( sizeof( struct slpq_node ) );

    if( new_node == NULL )
    {
        __LOG( "Error allocating memory for new slpq node: got %p\n", new_node );
        return;
    }

    new_node->data = data;

    // Handle first node in list
    if( head->head == NULL )
    {
        head->head = new_node;
        head->size++;
        return;
    }

    node = head->head;
    // Handle possible head replacement
    if( head->compare_function( new_node->data, node->data ) )
    {
        // Replace head
        new_node->next = head->head;
        head->head     = new_node;
    }
    else
    {
        // Locate insertion point
        last = node;

        while( node != NULL )
        {
            if( head->compare_function( new_node->data, node->data ) )
            {
                last->next = new_node;
                new_node->next = node;
                break;
            }

            last = node;
            node = node->next;

            // We are at the tail of the list and have not found an
            //  insertion point, add node to tail of the list
            if( node == NULL )
            {
                last->next = new_node;
                break;
            }
        }
    }

    head->size++;

    return;
}

void * slpq_pop( struct slpq * head )
{
    struct slpq_node * node = NULL;
    void *             data = NULL;

    if( slpq_empty( head ) )
    {
        return NULL;
    }

    node       = head->head;
    data       = node->data;
    head->head = node->next;

    __FREE( node );
    node = NULL;

    return data;
}

#ifdef SLPQ_DEBUG
void _slpq_setup_debug( struct slpq * head, void (*debug_function)( void * ) )
{
    if( head == NULL || debug_function == NULL )
    {
        return;
    }

    head->__debug_function = debug_function;
    return;
}

void _slpq_debug( struct slpq * head )
{
    struct slpq_node * node = NULL;

    if( head == NULL )
    {
        __LOG( "Head is %p\n", head );
        return;
    }

    if( slpq_empty( head ) )
    {
        __LOG( "Head is %p, no nodes\n", head );
        return;
    }

    __LOG(
        "HEAD: %p\n size: %u\n list start: %p\n",
        head,
        head->size,
        head->head
    );

    node = head->head;

    while( node != NULL )
    {
        __LOG(
            "NODE: %p, DATA: %p, NEXT: %p\n",
            node,
            node->data,
            node->next
        );

        if( head->__debug_function != NULL )
        {
            head->__debug_function( node->data );
        }

        node = node->next;
    }

    return;
}
#endif // SLPQ_DEBUG
