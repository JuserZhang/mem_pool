#include "mem_pool.h"

#define MEMPOOL_COUNT	10

int main()
{
	printf("press any key to start init memory pool\n");
	getchar();
	unsigned int a[MEMPOOL_COUNT] = { 8 ,16 ,32 ,64 ,128 , 256 , 512 , 1024 ,2048, 4096}; //内存池块长度
	unsigned int b[MEMPOOL_COUNT] = { 100 ,500 ,1000 ,1500 ,2000 , 3000 , 4000 , 5000 ,4000 , 2000}; //内存池对于块长度的块数量

	buffer_pool_init(MEMPOOL_COUNT, a , b); //初始化内存池
	buffer_runtime_print();
	printf("press any key to start malloc memory pool\n");
	getchar();
#if 0
	int i = 455;
	memory_pool_node * node = NULL;
	while (i > 0)
	{
		node = buffer_malloc(6);
		if (node != NULL)
		{
			printf("pool no[%d] get memory success!!!\n" , node->column);
			node->data[0] = i;
		}
		else
		{
			printf("malloc fail !!!\n");
			getchar();
		}
		i--;
		Sleep(10);
	}
	getchar();
#endif

	memory_pool_node * node1 = buffer_malloc(6);
	memory_pool_node * node2 = buffer_malloc(172);
	memory_pool_node * node3 = buffer_malloc(186);
	memory_pool_node * node4 = buffer_malloc(1300);
	memory_pool_node * node5 = buffer_malloc(2100);

	buffer_malloc(40);
	buffer_malloc(60);
	buffer_malloc(80);
	buffer_malloc(100);
	buffer_malloc(120);
	buffer_malloc(130);
	buffer_malloc(150);
	buffer_malloc(180);
	buffer_malloc(700);
	buffer_malloc(900);

	buffer_runtime_print();

	printf("press any key to free memory pool\n");
	getchar();
	buffer_free(node5);
	buffer_free(node4);
	buffer_free(node3);
	buffer_runtime_print();
	buffer_free(node2);
	buffer_free(node1);
	buffer_runtime_print();

	printf("press any key to destory memory pool\n");
	getchar();
	buffer_pool_destory();

	printf("press any key to quit\n");
	getchar();
	return 0;
}