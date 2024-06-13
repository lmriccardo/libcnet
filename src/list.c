#include "utils/list.h"

void Node_delete(struct Node* _self)
{
    if (_self->_alloc) free(_self->_value);
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

LinkedList* LinkedList_new_v2(struct Node* _node)
{
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->_head = _node;
    ll->_last = _node;
    ll->_size = 1;

    return ll;
}

void LinkedList_delete(LinkedList* _self)
{
    if (_self->_last != NULL)
    {
        struct Node* _node = NULL;

        do {

            _node = LinkedList_pop(_self);
            Node_delete(_node);
        
        } while (_node != NULL);
    }

    free(_self);
}

struct Node* LinkedList_pop(LinkedList* _self)
{
    struct Node* _node = _self->_last;
    _self->_last = _node->_prev;
    _self->_last->_next = NULL;

    return _node;
}