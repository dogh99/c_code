/*
在嵌入式 Linux 系统中，需要实现一个简化版的动态内存分配器。
假设程序已经拥有一块连续的内存区域：
#define POOL_SIZE 4096

static unsigned char memory_pool[POOL_SIZE];


请实现以下接口：
void allocator_init(void);

void *my_malloc(size_t size);

void my_free(void *ptr);
*/
#include <stdio.h>
#include <unistd.h>

#define POOL_SIZE 4096

struct Block{
    size_t size;
    int is_free;
    struct Block* next;
};

_Alignas(struct Block) static unsigned char memory_pool[POOL_SIZE];//满足对齐要求

static struct Block* head = NULL;//链表头


//初始化内存池,初始化为一个大的空闲内存块
void allocator_init(void)
{
   head = (struct Block*)memory_pool;
   head->size = POOL_SIZE - sizeof(struct Block);
   head->is_free = 1;
   head->next = NULL;
}

void *my_malloc(size_t size)
{
    if (size == 0 || size > POOL_SIZE - sizeof(struct Block))
    {
        return;
    }
    size_t align = _Alignof(struct Block);
    size = (size + align - 1) / align * align; //对要分配的大小字节对齐,为了是下一个块头的字节对齐

    struct Block* cur = head;

    while(cur != NULL)
    {
        if (cur->is_free && cur->size >= size)
        {
            //若分配后的剩余的空间可以装下块头和对齐的数据区,就进行拆分
            if(cur->size - size >= sizeof(struct Block) + align)
            {
                struct Block* new_block = (struct Block*) ((unsigned char*)(cur+1) + size);
                new_block->is_free = 1;
                new_block->next = cur->next;
                new_block->size = cur->size - size - sizeof(struct Block);
                
                cur->next = new_block;
                cur->size = size;
            }
            cur->is_free = 0;
            return cur + 1; //返回的是可用空间的首地址
        }
        cur = cur->next;
    }
    return NULL;
}

void my_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    struct Block* block = (struct Block*)ptr - 1; //找到当前块头地址
    block->is_free = 1;

    struct Block* cur = head;

    while (cur != NULL && cur->next != NULL)
    {
        if (cur->is_free && cur->next->is_free)
        {
            cur->size = cur->size + cur->next->size + sizeof(struct Block);
            cur->next = cur->next->next;
        }
        else
        {
            cur = cur->next;
        }
    }   
}

int main()
{

}