//给定两个已经按照非递减顺序排列的单向链表 list1 和 list2，请编写一个 C 语言函数，将这两个链表合并为一个新的非递减有序链表。
#include <stdio.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2)
{
    struct ListNode* p1 = list1;
    struct ListNode* p2 = list2;
    struct ListNode head = {0, NULL};
    struct ListNode* p = &head;

    while(p1 != NULL && p2 != NULL)
    {   
        if (p1->val <= p2->val)
        {
            p->next = p1;
            p1 = p1->next;
        }
        else if (p2->val < p1->val)
        {
            p->next = p2;
            p2 = p2->next;
        }
        p = p->next;
    }

    if (p1 == NULL)
    {
        p->next = p2;
    }
    else if(p2 == NULL)
    {
        p->next = p1;
    }
    return head.next;
}


int main()
{
    return 0;
}