#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* node_init(int data)
{
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}


//在node后插入节点
struct Node* node_insert_behind(struct Node* node, struct Node* new)
{
    node->next->prev = new;
    new->next = node->next;
    node->next = new;
    new->prev = node;
}


struct Node* find_node_by_value(struct Node* node, int value)
{
    while (node != NULL)
    {
        if (node->data == value)
        {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

int main()
{
    struct Node* node_head = node_init(99);
    return 0;
}