
#ifndef BINQ_H
#define BINQ_H

#include "postgres.h"

#define BINQ_DEBUG DEBUG

#define BINQ_HEAP_CHUNK_SIZE 8

#define _ALLOC(size) palloc0(size)
#define _REALLOC(ptr,size) repalloc(ptr,size)
#define _FREE(ptr) pfree(ptr)
#define _LOG(msg,args...) elog(DEBUG1,msg,args)

struct binq_node {
    void * data;
    struct binq_node * parent;
    struct binq_node * prev; // AKA left
    struct binq_node * next; // AKA right
};

struct binq {
    bool (*compare)(void *, void *);
#ifdef BINQ_DEBUG
    void (*debug)(void *);
#endif // BINQ_DEBUG
    struct binq_node * root
}
#endif // BINQ_H
