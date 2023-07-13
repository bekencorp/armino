#include <string.h>
#include "ring_buffer_node.h"

void ring_buffer_node_init(RingBufferNodeContext *rbn, uint8_t *address, uint32_t node_len, uint32_t nodes)
{
    rbn->address  = address;
    rbn->node_len = node_len;
    rbn->node_num = nodes;
    rbn->rp       = 0;
    rbn->wp       = 0;
}

void ring_buffer_node_clear(RingBufferNodeContext *rbn)
{
    rbn->rp = 0;
    rbn->wp = 0;
}

uint32_t ring_buffer_node_get_fill_nodes(RingBufferNodeContext *rbn)
{
    return rbn->wp >= rbn->rp ? rbn->wp - rbn->rp : rbn->node_num - rbn->rp + rbn->wp;
}

uint32_t ring_buffer_node_get_free_nodes(RingBufferNodeContext *rbn)
{
    uint32_t free_nodes = rbn->wp >= rbn->rp ? rbn->node_num - rbn->wp + rbn->rp : rbn->rp - rbn->wp;

    return free_nodes > 0 ? free_nodes - 1 : 0;
}

uint8_t *ring_buffer_node_get_read_node(RingBufferNodeContext *rbn)
{
    uint8_t *node = rbn->address + rbn->node_len * rbn->rp;

    if (++rbn->rp >= rbn->node_num)
    {
        rbn->rp = 0;
    }

    return node;
}

uint8_t *ring_buffer_node_get_write_node(RingBufferNodeContext *rbn)
{
    uint8_t *node = rbn->address + rbn->node_len * rbn->wp;

    if (++rbn->wp >= rbn->node_num)
    {
        rbn->wp = 0;
    }

    return node;
}

uint8_t *ring_buffer_node_peek_read_node(RingBufferNodeContext *rbn)
{
    return rbn->address + rbn->node_len * rbn->rp;
}

void ring_buffer_node_take_read_node(RingBufferNodeContext *rbn)
{
    if (++rbn->rp >= rbn->node_num)
    {
        rbn->rp = 0;
    }
}
