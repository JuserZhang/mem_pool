#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MP_OK			 0
#define MP_MALLOC_FIAL  -1
#define MP_NOT_INIT 	-2

typedef struct bpn memory_pool_node;
struct bpn
{
	unsigned int 		column; //内存节点所属内存池组id
	unsigned char 		*data;
	memory_pool_node 	*next;
};

typedef struct
{
	unsigned int 		total_count;
	unsigned int 		used_count;
	unsigned int 		block_len;
	memory_pool_node 	*free_header;
	memory_pool_node 	*used_header; //已使用的内存节点头指针
} memory_pool_colume;

int buffer_pool_init(unsigned int colume_no, unsigned int block_len[], unsigned int block_count[]); //初始化内存池
memory_pool_node *buffer_malloc(unsigned int size); //从内存池分配内存
int buffer_free(memory_pool_node *buffer);	//将内存归还内存池
int buffer_pool_destory(void);	//销毁内存池
int buffer_runtime_print(void); //打印内存池状态报告

#endif