#include "btree.h"

struct btree * new_btree( bool (*compare_function)( void *, void * ), void * data )
{
    struct btree *      new_btree = NULL;
    struct btree_node * new_node = NULL;

    if( compare_function == NULL )
    {
        return NULL;
    }

    new_btree = ( struct btree * ) _ALLOC( sizeof( struct btree ) );

    if( new_btree == NULL )
    {
        _LOG( "Failed to allocate memory for btree: got %p", new_btree );
        return NULL;
    }

    new_btree->compare_function = compare_function;
    new_btree->number_nodes     = 0;
    new_btree->root             = NULL;

    if( data == NULL )
    {
        return new_btree;
    }

    new_node = ( struct btree_node * ) _ALLOC( sizeof( struct btree_node ) );

    if( new_node == NULL )
    {
        _LOG( "Failed to allocate memory for btree node: got %p", new_node );
        return new_btree; 
    }

    new_node->left  = NULL;
    new_node->right = NULL;
    new_node->value = data;

    new_btree->number_nodes = 1;
    new_btree->root         = new_node;

    return new_btree;
}

static void __btree_insert( struct btree_node * root, struct btree_node * child )
{

}

void btree_insert( struct btree * btree, void * data )
{
    struct btree_node * new_node = NULL;

    if( btree == NULL || data == NULL )
    {
        return;
    }

    new_node = ( struct btree_node * ) _ALLOC( sizeof( struct btree_node ) );

    if( new_node == NULL )
    {
        _LOG( "Failed to allocate memory for btree node during insert, got %p", new_node );
        return;
    }

    new_node->left  = NULL;
    new_node->right = NULL;
    new_node->value = data;

    if( btree->root == NULL )
}
