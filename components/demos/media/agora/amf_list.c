
#include "amf_list.h"

#define VIDEO_FRAME_NUM  25

/* create new node */
video_frame_node_t *video_fram_list_node_malloc(video_frame_data_t data)
{
	video_frame_node_t *new_node = (video_frame_node_t *)os_malloc(sizeof(video_frame_node_t));
	if (new_node == NULL)
	{
		os_printf("malloc new_node fail\n");
	}
	else
	{
		new_node->data = data;
		new_node->next = NULL;
	}
	return new_node;
}

/* get node number */
uint8_t video_fram_list_get_node_count(video_frame_node_t *list_head)
{
	video_frame_node_t *cur = list_head;
	uint8_t count = 0;

	while (cur)
	{
		++count;
		cur = cur->next;
	}

	return count;
}


/* insert tail */
void video_fram_list_push_back(video_frame_node_t **list_head, video_frame_node_t *new)
{
	if (video_fram_list_get_node_count(*list_head) >= VIDEO_FRAME_NUM) {
		os_printf("list is full, count: %d \n", VIDEO_FRAME_NUM);
		os_free(new);
		new = NULL;
		return;
	}

	if (*list_head == NULL)
	{
		*list_head = new;
	}
	else
	{
		video_frame_node_t *cur=*list_head;
		while (cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next = new;
	}
}

/* insert front */
void video_fram_list_push_front(video_frame_node_t **list_head, video_frame_node_t *new)
{
	if (*list_head == NULL)
	{
		*list_head = new;
	}
	else
	{
		new->next = *list_head;
		*list_head = new;
	}
}

/* delete tail */
void video_fram_list_pop_back(video_frame_node_t **list_head)
{
	if (*list_head == NULL)
	{
		os_printf("[%s] list is empty \n", __func__);
		return;
	}

	video_frame_node_t* cur = (*list_head)->next;
	video_frame_node_t* perv = *list_head;
	if (cur == NULL)
	{
		os_free(*list_head);
		*list_head = NULL;
	}
	else
	{
		while (cur->next)
		{
			if (cur->next != NULL)
			{
				cur = cur->next;
				perv = perv->next;
			}
		}
		perv->next = NULL;
		os_free(cur);
	}
}

/* delete front */
void video_fram_list_pop_front(video_frame_node_t **list_head)
{
	if (*list_head == NULL)
	{
		os_printf("[%s] list is empty, head: %p \n", __func__, *list_head);
		return;
	}
	else
	{
		video_frame_node_t *cur = (*list_head)->next;
		os_free(*list_head);
		*list_head = cur;
	}
}

/* destory list */
void video_fram_list_destory(video_frame_node_t *list_head)
{
	video_frame_node_t *cur = list_head;
	video_frame_node_t *nex = list_head;
	while (cur)
	{
		cur = cur->next;
		os_free(nex);
		nex = cur;
	}
}


/* get all node frame length */
uint32_t video_fram_list_get_all_length(video_frame_node_t *list_head)
{
	video_frame_node_t *cur = list_head;
	uint32_t total = 0;

	while (cur)
	{
		//os_printf("length: %d, total: %d \n", cur->data.frame_length, total);
		total += cur->data.frame_length;
		cur = cur->next;
	}

	return total;
}

