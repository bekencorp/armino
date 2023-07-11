#ifndef __RING_BUFFER_NODE_H__
#define __RING_BUFFER_NODE_H__

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>

/**
 * @brief ring buffer node context definition
 */
typedef struct _RingBufferNodeContext
{
    uint8_t *address;   /**< memory base address            */
    uint32_t node_len;  /**< length per node buffer in byte */
    uint32_t node_num;  /**< total nodes                    */
    uint32_t rp;        /**< write point in node            */
    uint32_t wp;        /**< read point in node             */
} RingBufferNodeContext;

/**
 * @brief ring buffer node initialization
 * @param[in] rbn      ring buffer node context pointer
 * @param[in] address  ring buffer node start address
 * @param[in] node_len ring buffer size per node
 * @param[in] nodes    ring buffer nodes count
 * @return void
 */
void ring_buffer_node_init(RingBufferNodeContext *rbn, uint8_t *address, uint32_t node_len, uint32_t nodes);

/**
 * @brief ring buffer node clear
 * @param[in] rbn ring buffer node context pointer
 * @return void
 */
void ring_buffer_node_clear(RingBufferNodeContext *rbn);

/**
 * @brief Get filled nodes from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return filled nodes count
 */
uint32_t ring_buffer_node_get_fill_nodes(RingBufferNodeContext *rbn);

/**
 * @brief Get free nodes from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return free nodes count
 */
uint32_t ring_buffer_node_get_free_nodes(RingBufferNodeContext *rbn);

/**
 * @brief Get a node pointer to read from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return a node pointer to read
 */
uint8_t *ring_buffer_node_get_read_node(RingBufferNodeContext *rbn);

/**
 * @brief Get a node pointer to write from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return a node pointer to write
 */
uint8_t *ring_buffer_node_get_write_node(RingBufferNodeContext *rbn);

/**
 * @brief Peek a node but NOT take from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return a node buffer pointer to peek
 */
uint8_t *ring_buffer_node_peek_read_node(RingBufferNodeContext *rbn);

/**
 * @brief Take a node out from ring buffer node context pointer
 * @param[in] rbn ring buffer node context pointer
 * @return void
 */
void ring_buffer_node_take_read_node(RingBufferNodeContext *rbn);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__RING_BUFFER_NODE_H__

/**
 * @}
 */