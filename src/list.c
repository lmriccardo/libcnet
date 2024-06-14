#include "utils/list.h"

struct Node* Node_new(void *_value, size_t _vsize)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->_next = NULL;
    node->_prev = NULL;

    node->_value = malloc(_vsize);
    memcpy(node->_value, _value, _vsize);

    return node;
}

void Node_delete(struct Node* _self)
{
    free(_self->_value);
    free(_self);
}

LinkedList* LinkedList_new()
{
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->_head = NULL;
    ll->_last = NULL;
    ll->_size = 0;

    return ll;
}

void LinkedList_delete(LinkedList* _self)
{
    if (_self->_last != NULL)
    {
        for (int i = _self->_size; i > 0; i--)
        {
            struct Node* _node = LinkedList_pop(_self);
            Node_delete(_node);
        }
    }

    free(_self);
}

void LinkedList_append(LinkedList* _self, struct Node* _node)
{
    if (_self->_head == NULL)
    {
        _self->_head = _node;
        _self->_last = _node;
        _self->_size++;
        return;
    }

    struct Node* _last = _self->_last;
    _last->_next = _node;
    _node->_prev = _last;
    _self->_last = _node;
    _self->_size++;
}

void LinkedList_appendv(LinkedList* _self, void* _value, size_t _vsize) 
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_append(_self, node);
}

void LinkedList_push(LinkedList* _self, struct Node* _node)
{
    if (_self->_head == NULL)
    {
        _self->_head = _node;
        _self->_last = _node;
        _self->_size++;
        return;
    }

    struct Node* _head = _self->_head;
    _head->_prev = _node;
    _node->_next = _head;
    _self->_head = _node;
    _self->_size++;
}

void LinkedList_pushv(LinkedList* _self, void* _value, size_t _vsize) 
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_push(_self, node);
}

struct Node* LinkedList_pop(LinkedList* _self)
{
    if (_self->_last == NULL)
    {
        fprintf(stderr, "[LinkedList_pop] Cannot pop, the list is empty.\n");
        exit(EXIT_FAILURE);
    }

    struct Node* _node = _self->_last;
    _self->_last = _node->_prev;
    
    // It might be the first element of the list, which has _prev = NULL
    if (_self->_last == NULL) {
        _self->_head = NULL;
    } else {
        _self->_last->_next = NULL;
    }

    _self->_size--;

    return _node;
}

struct Node* LinkedList_popi(LinkedList* _self, int _i)
{
    if (_self->_last == NULL)
    {
        fprintf(stderr, "[LinkedList_popi] Cannot remove the item, the list is empty.\n");
        exit(EXIT_FAILURE);
    }

    struct Node* node = NULL;

    for (int i = 0; i < _self->_size; i++)
    {
        node = i == 0 ? _self->_head : node->_next;

        // If it is not the index we are looking for, continue
        if (i != _i) continue;

        // Otherwise we have to take the node and change all
        // the links in the Linked List
        if (node->_prev != NULL)
        {
            node->_prev->_next = node->_next;
            
            if (node->_next != NULL)
            {
                node->_next->_prev = node->_prev;
            }
        }

        break;
    }

    _self->_size--;
    
    return node;
}