#include "binq.h"

struct binq * new_binq(
    bool (*compare)( void *, void * )
#ifdef BINQ_DEBUG
  , void (*debug)( void * )
#endif // BINQ_DEBUG
)
{
    struct binq * new_heap = NULL;

    if( compare == NULL )
    {
        _LOG( "Comparison function is %p", compare );
        return NULL;
    }

    new_heap = ( struct binq * ) _ALLOC( sizeof( struct binq ) );

    if( new_heap == NULL )
    {
        return NULL;
    }

    new_heap->in_use   = 0;
    new_heap->compare  = compare;
    new_heap->max_size = ( unsigned int ) BINQ_HEAP_CHUNK_SIZE;
    new_heap->data     = ( void ** ) _ALLOC(
        BINQ_HEAP_CHUNK_SIZE * sizeof( void * )
    );
#ifdef BINGQ_DEBUG
    new_heap->binq_debug = debug;
#endif // BINQ_DEBUG

    if( new_heap->data == NULL )
    {
        _FREE( new_heap );
        return NULL;
    }

    return new_heap;
}

void free_binq( struct binq * heap )
{
    if( heap == NULL )
    {
        return;
    }

    if( heap->data != NULL )
    {
        _FREE( heap->data );
        heap->data     = NULL;
        heap->max_size = 0;
        heap->in_use   = 0;
    }

    _FREE( heap );
    return;
}

void binq_push( struct binq * heap, void * data )
{
    unsigned int index        = 0;
    unsigned int parent_index = 0;
    void *       current      = NULL;
    void *       parent       = NULL;

    if( heap == NULL || data == NULL)
    {
        return;
    }

    if( heap->in_use == heap->max_size )
    {
        // Need to extendo the heap
        heap->max_size = heap->max_size + BINQ_HEAP_CHUNK_SIZE;
        heap->data = _REPALLOC(
            heap->data,
            heap->max_size * sizeof( void * )
        );

        if( heap == NULL )
        {
            _LOG(
                "Failed to extend heap by %lu entries",
                (unsigned long) BINQ_HEAP_CHUNK_SIZE
            );

            return;
        }
    }

    index        = heap->in_use;
    heap->in_use = heap->in_use + 1;
    current      = heap->data[index];

    if( index )
    {
        parent_index = ( index - 1 ) >> 1;
        parent = heap->data[parent_index];

        do {
            if( heap->compare( parent, data ) )
            {
                break;
            }

            *current     = *parent;
            index        = parent_index;
            current      = heap->data[index];
            parent_index = ( index - 1 ) >> 1;
            parent       = heap->data[parent_index];
        } while( index );
    }

    *current = *data;

    return;
}

void * binq_pop( struct binq * heap )
{
    void *       head               = NULL;
    void *       temp               = NULL;
    void *       child              = NULL;
    void *       lowest_child       = NULL;
    unsigned int index              = 0;
    unsigned int lowest_child_index = 0;
    unsigned int other_child_index  = 0;
    unsigned int child_index        = 0;

    if( heap == NULL || heap->data == NULL )
    {
        return NULL;
    }

    head = heap->data[0]; 

    if( head == NULL )
    {
        return NULL;
    }

    while( 1 )
    {
        child_index = ( index << 1 ) + 1;    

        if( child_index >= heap->in_use )
        {
            break;
        }

        child             = heap->data[child_index];
        other_child_index = child_index + 1;

        if( other_child_index < heap->in_use )
        {
            other_child = heap->data[other_child_index];

            if( heap->compare( child, other_child ) )
            {
                lowest_child       = child;
                lowest_child_index = child_index;
            }
            else
            {
                lowest_child       = other_child;
                lowest_child_index = other_child_index;
            }
        }
        else
        {
            lowest_child       = child;
            lowest_child_index = child_index;
        }

        if( heap->compare( temp, lowest_child ) )
        {
            break;
        }

        heap->data[index] = *lowest_child;
        index = lowest_child_index;
    }

    heap->data[index] = temp;

    return head;
}

void binq_delete( struct binq * heap, void * )
{

}
