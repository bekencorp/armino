#include <os/os.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_assert.h>

#include "utils.h"
#include "mlist.h"

struct mlist_node_t
{
	struct mlist_node_t *next;
	void *data;
};

typedef struct mlist_t
{
	mlist_node_t *head;
	mlist_node_t *tail;
	size_t length;
	mlist_free_cb free_cb;
	const allocator_t *allocator;
} mlist_t;

void *osi_malloc(size_t size);
void osi_free(void *ptr);


const allocator_t allocator_malloc = {osi_malloc, osi_free};

static mlist_node_t *mlist_free_node(mlist_t *mlist, mlist_node_t *node);

// Hidden constructor, only to be used by the hash map for the allocation
// tracker.
// Behaves the same as |mlist_new|, except you get to specify the allocator.
mlist_t *mlist_new_internal(mlist_free_cb callback,
                            const allocator_t *zeroed_allocator)
{
	mlist_t *mlist = (mlist_t *)zeroed_allocator->alloc(sizeof(mlist_t));
	if (!mlist)
	{
		return NULL;
	}

	mlist->free_cb = callback;
	mlist->allocator = zeroed_allocator;
	return mlist;
}

mlist_t *mlist_new(mlist_free_cb callback)
{
	return mlist_new_internal(callback, &allocator_malloc);
}

void mlist_free(mlist_t *mlist)
{
	if (!mlist)
	{
		return;
	}

	mlist_clear(mlist);
	mlist->allocator->free(mlist);
}

bool mlist_is_empty(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	return (mlist->length == 0);
}

bool mlist_contains(const mlist_t *mlist, const void *data)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(data != NULL);

	for (const mlist_node_t *node = mlist_begin(mlist); node != mlist_end(mlist);
	     node = mlist_next(node))
	{
		if (mlist_node(node) == data)
		{
			return true;
		}
	}

	return false;
}

size_t mlist_length(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	return mlist->length;
}

void *mlist_front(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(!mlist_is_empty(mlist));

	return mlist->head->data;
}

void *mlist_back(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(!mlist_is_empty(mlist));

	return mlist->tail->data;
}

mlist_node_t *mlist_back_node(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(!mlist_is_empty(mlist));

	return mlist->tail;
}

bool mlist_insert_after(mlist_t *mlist, mlist_node_t *prev_node, void *data)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(prev_node != NULL);
	BK_ASSERT(data != NULL);

	mlist_node_t *node = (mlist_node_t *)mlist->allocator->alloc(sizeof(mlist_node_t));
	if (!node)
	{
		return false;
	}

	node->next = prev_node->next;
	node->data = data;
	prev_node->next = node;
	if (mlist->tail == prev_node)
	{
		mlist->tail = node;
	}
	++mlist->length;
	return true;
}

bool mlist_prepend(mlist_t *mlist, void *data)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(data != NULL);

	mlist_node_t *node = (mlist_node_t *)mlist->allocator->alloc(sizeof(mlist_node_t));
	if (!node)
	{
		return false;
	}
	node->next = mlist->head;
	node->data = data;
	mlist->head = node;
	if (mlist->tail == NULL)
	{
		mlist->tail = mlist->head;
	}
	++mlist->length;
	return true;
}

bool mlist_append(mlist_t *mlist, void *data)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(data != NULL);

	mlist_node_t *node = (mlist_node_t *)mlist->allocator->alloc(sizeof(mlist_node_t));
	if (!node)
	{
		return false;
	}
	node->next = NULL;
	node->data = data;
	if (mlist->tail == NULL)
	{
		mlist->head = node;
		mlist->tail = node;
	}
	else
	{
		mlist->tail->next = node;
		mlist->tail = node;
	}
	++mlist->length;
	return true;
}

bool mlist_remove(mlist_t *mlist, void *data)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(data != NULL);

	if (mlist_is_empty(mlist))
	{
		return false;
	}

	if (mlist->head->data == data)
	{
		mlist_node_t *next = mlist_free_node(mlist, mlist->head);
		if (mlist->tail == mlist->head)
		{
			mlist->tail = next;
		}
		mlist->head = next;
		return true;
	}

	for (mlist_node_t *prev = mlist->head, *node = mlist->head->next; node;
	     prev = node, node = node->next)
		if (node->data == data)
		{
			prev->next = mlist_free_node(mlist, node);
			if (mlist->tail == node)
			{
				mlist->tail = prev;
			}
			return true;
		}

	return false;
}

void mlist_clear(mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	for (mlist_node_t *node = mlist->head; node;)
	{
		node = mlist_free_node(mlist, node);
	}
	mlist->head = NULL;
	mlist->tail = NULL;
	mlist->length = 0;
}

mlist_node_t *mlist_foreach(const mlist_t *mlist, mlist_iter_cb callback,
                            void *context)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(callback != NULL);

	for (mlist_node_t *node = mlist->head; node;)
	{
		mlist_node_t *next = node->next;
		if (!callback(node->data, context))
		{
			return node;
		}
		node = next;
	}
	return NULL;
}


void *mlist_foreach_pop(mlist_t *mlist, mlist_iter_cb callback,
                        void *context)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(callback != NULL);
	void *data = NULL;

	if (mlist_is_empty(mlist))
	{
		return false;
	}

	if (callback(mlist->head->data, context))
	{
		data = mlist->head->data;

		mlist_node_t *next = mlist_free_node(mlist, mlist->head);
		if (mlist->tail == mlist->head)
		{
			mlist->tail = next;
		}
		mlist->head = next;

		return data;
	}

	for (mlist_node_t *prev = mlist->head, *node = mlist->head->next; node;
	     prev = node, node = node->next)
		if (callback(node->data, context))
		{
			data = node->data;
			prev->next = mlist_free_node(mlist, node);
			if (mlist->tail == node)
			{
				mlist->tail = prev;
			}
			return data;
		}

	return NULL;
}

mlist_node_t *mlist_begin(const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	return mlist->head;
}

mlist_node_t *mlist_end(UNUSED_ATTR const mlist_t *mlist)
{
	BK_ASSERT(mlist != NULL);
	return NULL;
}

mlist_node_t *mlist_next(const mlist_node_t *node)
{
	BK_ASSERT(node != NULL);
	return node->next;
}

void *mlist_node(const mlist_node_t *node)
{
	BK_ASSERT(node != NULL);
	return node->data;
}

static mlist_node_t *mlist_free_node(mlist_t *mlist, mlist_node_t *node)
{
	BK_ASSERT(mlist != NULL);
	BK_ASSERT(node != NULL);

	mlist_node_t *next = node->next;

	if (mlist->free_cb)
	{
		mlist->free_cb(node->data);
	}
	mlist->allocator->free(node);
	--mlist->length;

	return next;
}

void *osi_malloc(size_t size)
{
	return os_malloc(size);
}

void osi_free(void *ptr)
{
	os_free(ptr);
}
