#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//#define BTREE_DEBUG true

#define _ALLOC(size) calloc(1,size)
#define _FREE(ptr) free(ptr)
#define _LOG(msg,args...) printf(msg,args)

struct btree {
    struct btree_node * root;
    unsigned int number_nodes;
    bool (*compare_function)( void *, void * );
};

struct btree_node {
    void *              value;
    struct btree_node * left;
    struct btree_node * right;
};

extern struct btree * new_btree( bool (*)( void *, void * ), void * );
extern void btree_insert( struct btree *, void * );


static void __btree_insert( struct btree_node *, struct btree_node * );

#endif // BTREE_H
