#include "heap.h"

struct heap * new_heap( bool (*compare)( void *, void * ) )
{
    struct heap * head = NULL;

    if( compare == NULL )
    {
        return NULL;
    }

    head = ( struct heap * ) _ALLOC( sizeof( struct heap ) );

    if( head == NULL )
    {
        return NULL;
    }

    head->compare = compare;
    head->in_use  = 0;
    head->size    = HEAP_CHUNK_SIZE;
    head->heap    = ( void ** ) _ALLOC(
        sizeof( void * ) * HEAP_CHUNK_SIZE
    );

    if( head->heap == NULL )
    {
        _FREE( head );
        _LOG( "heap for %p cannot be allocated", head );
        return NULL;
    }

    return head;
}

void free_heap( struct heap * heap )
{
    unsigned int i = 0;

    if( heap == NULL )
    {
        return;
    }

    if( heap->in_use > 0 && heap->heap != NULL )
    {
        _FREE( heap->heap );
    }

    heap->heap   = NULL;
    heap->size   = 0;
    heap->in_use = 0;

    _FREE( heap );

    return;
}

void * heap_min( struct heap * heap )
{
    if( heap_empty( heap ) )
    {
        return NULL;
    }

    return heap->heap[0];
}

void heap_insert( struct heap * heap, void * data )
{
    bool (*compare)( void *, void * ) = NULL;
    void **      table         = NULL;
    void *       parent        = NULL;
    void *       current       = NULL;
    unsigned int parent_index  = 0;
    unsigned int current_index = 0;

    if( heap == NULL )
    {
        return;
    }

    if( heap->in_use + 1 > heap->size )
    {
        heap->heap = ( void ** ) _REALLOC(
            heap->heap,
            sizeof( void * ) * ( heap->size + HEAP_CHUNK_SIZE )
        );

        if( heap->heap == NULL )
        {
            // Oopsie poopsie
            heap->in_use = 0;
            heap->size   = 0;
            return;
        }

        heap->size = heap->size + HEAP_CHUNK_SIZE;
    }

    compare       = heap->compare;
    table         = heap->heap;
    current_index = heap->in_use;
    current       = table[current_index];

    while( current_index > 0 )
    {
        parent_index = _parent( current_index );
        parent       = table[parent_index]; //TODO add bounds checking / null check

        if( parent != NULL && !compare( data, parent ) )
        {
            table[current_index] = table[parent_index];
            current_index        = parent_index;
            current              = parent;
        }
        else
        {
            break;
        }
    }

    table[current_index] = data;
    heap->in_use         = heap->in_use + 1;
    return;
}

inline bool heap_empty( struct heap * heap )
{
    return ( heap == NULL || heap->in_use == 0 ) ? true : false;
}

inline unsigned int heap_size( struct heap * heap )
{
    return ( heap == NULL || heap->in_use == 0 ) ? 0 : heap->in_use;
}

void heap_delete_min( struct heap * heap )
{
    bool (*compare)( void *, void * ) = NULL;
    void *       temp                 = NULL;
    void **      table                = NULL;
    unsigned int current_index        = 0;
    unsigned int left_child_index     = 0;
    unsigned int right_child_index    = 0;
    unsigned int num_entries          = 0;

    if( heap_empty( heap ) )
    {
        return;
    }

    table         = heap->heap;
    compare       = heap->compare;
    current_index = 0;
    heap->in_use  = heap->in_use - 1;
    num_entries   = heap->in_use;

    if( num_entries > 0 )
    {
        while(
                    left_child_index = _left_child( current_index )
                 && left_child_index < num_entries
             )
        {
            right_child_index = left_child_index + 1;

            if( right_child_index < num_entries )
            {
                if( !compare( table[left_child_index], table[right_child_index] ) )
                {
                    if( compare( table[current_index], table[left_child_index] ) )
                    {
                        temp                    = table[current_index];
                        table[current_index]    = table[left_child_index];
                        table[left_child_index] = temp;
                        current_index           = left_child_index;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if( compare( table[current_index], table[right_child_index] ) )
                    {
                        temp                        = table[current_index];
                        table[current_index]        = table[right_child_index];
                        table[left_child_index + 1] = temp;
                        current_index               = right_child_index;;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    if( heap->in_use + HEAP_CHUNK_SIZE < heap->size )
    {
        heap->heap = ( void ** ) _REALLOC(
            heap->heap,
            sizeof( void * ) * ( heap->size - HEAP_CHUNK_SIZE )
        );

        if( heap->heap == NULL )
        {
            heap->in_use = 0;
            heap->size   = 0;
            return;
        }

        heap->size = heap->size - HEAP_CHUNK_SIZE;
    }

    return;
}

inline unsigned int _left_child( unsigned int index )
{
    return ( index << 1 ) + 1;
}

inline unsigned int _right_child( unsigned int index )
{
    return ( index << 1 ) + 2;
}

inline unsigned int _parent( unsigned int index )
{
    return ( index - 1 ) >> 1;
}

#ifdef HEAP_DEBUG
void debug_heap( struct heap * heap )
{
    unsigned int i = 0;
    _LOG( "HEAP DEBUG==================%s\n", "=" );
    _LOG( "heap head: %p\n", heap );
    if( heap != NULL )
    {
        _LOG( "size: %u\n", heap->size );
        _LOG( "in_use: %u\n", heap->in_use );
        _LOG( "heap: %p\n", heap->heap );
        _LOG( "compare fn %p\n", heap->compare );

        if( heap->heap != NULL )
        {
            for( i = 0; i < heap->in_use; i++ )
            {
                _LOG( "Heap[%u]: %p, %p\n", i, heap->heap[i], heap->heap[i] );

                if( heap->debug != NULL )
                {
                    _LOG( "    VALUE%s\n", ":" );
                    heap->debug( heap->heap[i] );
                }
            }
        }
    }

    _LOG( "============================%s\n", "=" );
    return;
}
#endif // HEAP_DEBUG
