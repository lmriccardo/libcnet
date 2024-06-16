#include <utils/list.h>
#include <receiver.h>
#include <string.h>

void test_addv_pop(void)
{
    LinkedList* ll = LinkedList_new(10);

    for (int i = 0; i < 4; i++)
    {
        LinkedList_appendv(ll, &i, sizeof(int));
    }

    printf("Linked List Size: %ld\n", ll->_size);

    struct Node* node = NULL;
    for (int i = ll->_size; i > 0; i--)
    {
        node = LinkedList_pop(ll);
        printf("Node %d value %d\n", i, *(int*)node->_value);
        Node_delete(node);
    }

    LinkedList_delete(ll);
}

void test_pushv_popi(void)
{
    LinkedList* ll = LinkedList_new(10);

    for (int i = 0; i < 4; i++)
    {
        LinkedList_pushv(ll, &i, sizeof(int));
    }

    struct Node* node = LinkedList_remove(ll, 0);
    printf("Node value %d\n", *(int*)node->_value);
    Node_delete(node);

    node = ll->_head;
    do {
        printf("Value: %d\n", *(int*)node->_value);
        node = node->_next;
    } while (node != NULL);

    LinkedList_delete(ll);
}

int main(int argc, char **argv)
{
    printf("[*] Testing LinkedList_appendv and LinkedList_pop\n");
    test_addv_pop();

    printf("[*] Testing LinkedList_pushv and LinkedList_remove\n");
    test_pushv_popi();

    return 0;
}