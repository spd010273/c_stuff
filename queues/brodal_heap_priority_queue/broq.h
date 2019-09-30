// Brodal queue acheives much faster O(1) insertion, meld/merge, and decrease with O(logn) delete without resorting to ammortization like fibq
// This is sometimes referred to as a skew-binomial queue

#ifndef BROQ_H
#define BROQ_H

#include "postgres.h"

#define BROQ_DEBEG DEBUG

#define _ALLOC(size) palloc0(size)
#define _FREE(ptr) pfree(ptr)
#define _LOG(msg,args,...) elog(DEBUG1,msg,args)

struct broq_node {
    void *              value;
    unsigned int        rank;
    struct broq_node ** children;
    unsigned int        num_children;
    struct broq_node *  parent;
};

struct broq {

};

struct broq_node * new_broq_node(
    void *,
    unsigned int,
    struct broq_node **,
    unsigned int
);


#endif // BROQ_H
