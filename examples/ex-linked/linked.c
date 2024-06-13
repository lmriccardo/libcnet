#include <utils/list.h>

int main(int argc, char **argv)
{
    LinkedList* ll = LinkedList_new();
    int j;

    for (int i = 0; i < 4; i++)
    {
        j = i;
        LinkedList_addValue(ll, &j, false);
        printf("Last value %d\n", *(int*)ll->_last->_value);

        if (ll->_last->_prev == NULL)
        {
            printf("Prev value null\n");
        }
        else
        {
            printf("Prev value %d\n", *(int*)((struct Node*)ll->_last->_prev)->_value);
        }
    }

    printf("Linked List Size: %ld\n", ll->_size);

    for (int i = 0; i < ll->_size; i++)
    {
        struct Node* node = LinkedList_pop(ll);
        printf("Node %d value %d\n", i, *(int*)node->_value);
    }

    LinkedList_delete(ll);

    return 0;
}