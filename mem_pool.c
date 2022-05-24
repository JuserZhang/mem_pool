#include "mem_pool.h"

#define HEAP_ID	9999

memory_pool_colume *g_mem_pool = NULL;
int g_buffer_pool_no = 0;

//内存池初始化
int buffer_pool_init(unsigned int colume_no, unsigned int block_len[], unsigned int block_count[])
{
	g_mem_pool = calloc(colume_no, sizeof(memory_pool_colume)); //初始化colume_no个内存池
	if (g_mem_pool == NULL)
		return MP_MALLOC_FIAL;

	g_buffer_pool_no = colume_no;

	memory_pool_node *curr_node = NULL;
	memory_pool_node *new_node = NULL;
	for (int i = 0; i < colume_no; i++) {
		g_mem_pool[i].block_len = block_len[i];
		g_mem_pool[i].total_count = block_count[i];
		for (int j = 0; j < block_count[i]; j++){
			new_node = calloc(1,sizeof(memory_pool_node));
			new_node->column = i;
			new_node->data = calloc(1,block_len[i]);
			if (new_node == NULL || new_node->data == NULL)
				return MP_MALLOC_FIAL;

			new_node->next = NULL;

			if (j == 0){
				g_mem_pool[i].free_header = new_node;
				curr_node = g_mem_pool[i].free_header;
			}
			else{
				curr_node->next = new_node;
				curr_node = curr_node->next;
			}
		}
	}

	return MP_OK;
}

//从内存池中分配出内存，如果内存从中没有足够的内存则从堆中分配
memory_pool_node *buffer_malloc(unsigned int size)
{
	memory_pool_node *node = NULL;
	/*分配的空间太大，内存池中没有这么大的空间，则从堆中分配*/
	if (size > g_mem_pool[g_buffer_pool_no - 1].block_len){
		printf("malloc size[%d] so big ,need new from heap!!\n", size);
		goto END;
	}

	for (int i = 0; i < g_buffer_pool_no; i++){
		/*查找到足够大的内存池可用*/
		if (size > g_mem_pool[i].block_len)
			continue;

		if (g_mem_pool[i].total_count - g_mem_pool[i].used_count == 0){ 
			printf("warning!!!!  size[%d]pool use up!!!! \n", g_mem_pool[i].block_len);
			continue; /*内存池空间耗尽，继续相向更高的内存池查找内存*/
		}

		/*查找可用内存池*/
		node = g_mem_pool[i].free_header; //从当前内存池中获取到空闲内存节点
		g_mem_pool[i].free_header = g_mem_pool[i].free_header->next; //空闲内存头指针向后移
		g_mem_pool[i].used_count++; //已使用内存数量+1

		/*将要分配的节点头插法插入使用链表*/
		node->next = g_mem_pool[i].used_header;
		g_mem_pool[i].used_header = node;

		return node;
	}

	/*所有内存池都没有足够大的空闲内存可分配*/
	printf("warning!!!!  all of pool used up!!!! \n");

END:
	node = calloc(1,sizeof(memory_pool_node));
	if(node == NULL)
		return NULL;
	node->column = HEAP_ID;
	node->data = calloc(1,size);
	if (node->data == NULL){
		free(node);
		return NULL;
	}
	node->next = NULL;

	return node;
}

//释放内存
int buffer_free(memory_pool_node *buffer)
{
	memory_pool_node *node_cur = g_mem_pool[buffer->column].used_header; //将当前节点指向所属内存池使用链表头
	memory_pool_node *node_pre = NULL;

	//从堆中分配的内存直接释放，返回给操作系统
	if (buffer->column == HEAP_ID){
		free(buffer->data);
		buffer->data = NULL;
		free(buffer);
		buffer = NULL;

		return MP_OK;
	}

	while (node_cur != NULL){
		if (node_cur != buffer){
			node_pre = node_cur;
			node_cur = node_cur->next;
			continue;
		}

		if (node_pre == NULL){ //当前要释放的内存节点为使用链表的头节点
			g_mem_pool[buffer->column].used_header = g_mem_pool[buffer->column].used_header->next; //直接将使用链表头下移一节
		}
		else{
			node_pre->next = node_cur->next;
		}

		g_mem_pool[buffer->column].used_count--; //使用数量-1
		/*将内存节点从当前池的使用链表移动到未使用链表头*/
		node_cur->next = g_mem_pool[buffer->column].free_header;
		g_mem_pool[buffer->column].free_header = node_cur;

		break;
	}

	return MP_OK;
}

//销毁内存池
int buffer_pool_destory(void)
{
	memory_pool_node *node_cur = NULL;
	memory_pool_node *node_del = NULL;

	if (g_mem_pool == NULL)
		return MP_NOT_INIT;

	for (int i = 0; i < g_buffer_pool_no; i++){
		node_cur = g_mem_pool[i].used_header;
		while (node_cur != NULL){
			node_del = node_cur;
			node_cur = node_cur->next;
			free(node_del->data);
			free(node_del);
		}
		node_cur = g_mem_pool[i].free_header;
		while (node_cur != NULL){
			node_del = node_cur;
			node_cur = node_cur->next;
			free(node_del->data);
			free(node_del);
		}
	}

	free(g_mem_pool);
	g_mem_pool = NULL;
	g_buffer_pool_no = 0;

	return MP_OK;
}

//内存池状态打印
int buffer_runtime_print(void)
{
	if (g_mem_pool == NULL){
		printf("buffer pool not init yet!!!\n");
		return MP_NOT_INIT;
	}

	printf("\n------------------------------ memory pool runtime report start ------------------------------\n");
	for (int i = 0; i < g_buffer_pool_no; i++){
		printf("pool no[%d] blocksize[%d] blockTotalCount[%d] usedBlock[%d] used percentage[%d%%]\n", 
			    i, g_mem_pool[i].block_len, g_mem_pool[i].total_count, g_mem_pool[i].used_count, 
			    g_mem_pool[i].used_count * 100 / g_mem_pool[i].total_count);
	}
	printf("------------------------------ memory pool runtime report end ------------------------------\n");

	return MP_OK;
}