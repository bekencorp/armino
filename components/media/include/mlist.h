#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef void *(*alloc_fn)(size_t size);
typedef void (*free_fn)(void *ptr);

typedef struct
{
	alloc_fn alloc;
	free_fn free;
} allocator_t;

extern const allocator_t allocator_malloc;
extern const allocator_t allocator_calloc;


struct mlist_node_t;
typedef struct mlist_node_t mlist_node_t;

struct mlist_t;
typedef struct mlist_t mlist_t;

typedef void (*mlist_free_cb)(void *data);
typedef bool (*mlist_iter_cb)(void *data, void *context);
mlist_t *mlist_new(mlist_free_cb callback);
void mlist_free(mlist_t *mlist);
bool mlist_is_empty(const mlist_t *mlist);
bool mlist_contains(const mlist_t *mlist, const void *data);
size_t mlist_length(const mlist_t *mlist);
void *mlist_front(const mlist_t *mlist);
void *mlist_back(const mlist_t *mlist);
mlist_node_t *mlist_back_node(const mlist_t *mlist);
bool mlist_insert_after(mlist_t *mlist, mlist_node_t *prev_node, void *data);
bool mlist_prepend(mlist_t *mlist, void *data);
bool mlist_append(mlist_t *mlist, void *data);
bool mlist_remove(mlist_t *mlist, void *data);
void mlist_clear(mlist_t *mlist);
mlist_node_t *mlist_foreach(const mlist_t *mlist, mlist_iter_cb callback,
                            void *context);
mlist_node_t *mlist_begin(const mlist_t *mlist);
mlist_node_t *mlist_end(const mlist_t *mlist);
mlist_node_t *mlist_next(const mlist_node_t *node);
void *mlist_node(const mlist_node_t *node);
void *mlist_foreach_pop(mlist_t *mlist, mlist_iter_cb callback,
                        void *context);

