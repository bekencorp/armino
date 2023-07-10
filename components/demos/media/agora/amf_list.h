#ifndef __AMF_LIST_H__
#define __AMF_LIST_H__

#include <os/os.h>
#include <os/mem.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t frame_length;
} video_frame_data_t;

typedef struct video_frame_node_t {
	video_frame_data_t data;
	struct video_frame_node_t *next;
} video_frame_node_t;

/* create new node */
video_frame_node_t *video_fram_list_node_malloc(video_frame_data_t data);

/* insert tail */
void video_fram_list_push_back(video_frame_node_t **list_head, video_frame_node_t *new);

/* insert front */
void video_fram_list_push_front(video_frame_node_t **list_head, video_frame_node_t *new);

/* delete tail */
void video_fram_list_pop_back(video_frame_node_t **list_head);

/* delete front */
void video_fram_list_pop_front(video_frame_node_t **list_head);

/* destory list */
void video_fram_list_destory(video_frame_node_t *list_head);

/* get node number */
uint8_t video_fram_list_get_node_count(video_frame_node_t *list_head);

/* get all node frame length */
uint32_t video_fram_list_get_all_length(video_frame_node_t *list_head, uint8_t num);


#ifdef __cplusplus
}
#endif
#endif /* __AMF_RINGBUF_H__ */


