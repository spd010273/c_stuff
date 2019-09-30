
#include "broq.h";

struct broq_node * new_broq_node(
    void *              data,
    unsigned int        rank,
    struct broq_node ** children,
    unsigned int        num_children
)
{
    struct broq_node * new_node = NULL;
    unsigned int       i        = 0;

    if( data == NULL )
    {
        return NULL;
    }

    new_node = ( struct broq_node * ) _ALLOC( sizeof( struct broq_node ) );

    if( new_node == NULL )
    {
        return NULL;
    }

    new_node->data = data;
    new_node->rank = rank;

    if( children == NULL || num_children == 0 )
    {
        new_node->children     = NULL;
        new_node->num_children = 0;
    }
    else
    {
        for( i = 0; i < num_children; i++ )
        {
            if( children[i] == NULL )
            {
                _LOG( "NULL child at list index %lu, array %p", i, children );
                _FREE( new_node );
                return NULL;
            }

            children[i]->parent = new_node;
        }

        new_node->children     = children;
        new_node->num_children = num_children;
    }

    return new_node;
}


