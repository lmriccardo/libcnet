#include "utils/list.h"

struct Node* Node_new(void *_value, bool _alloc)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->_next = NULL;
    node->_prev = NULL;
    node->_value = _value;
    node->_alloc = _alloc;

    return node;
}

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
    if (_self->_last == NULL)
    {
        fprintf(stderr, "[LinkedList_pop] Cannot pop, the list is empty.\n");
        exit(EXIT_FAILURE);
    }

    struct Node* _node = _self->_last;
    _self->_last = _node->_prev;
    
    // It might be the first element of the list, which has _prev = NULL
    if (_self->_last == NULL)
    {
        _self->_head = NULL;
    }
    else 
    {
        _self->_last->_next = NULL;
    }

    _self->_size--;

    return _node;
}

void LinkedList_add(LinkedList* _self, struct Node* _node)
{
    if (_self->_head == NULL)
    {
        _self->_head = _node;
        _self->_last = _node;
        _self->_size++;
        return;
    }

    struct Node* _last = _self->_last;
    _node->_prev = _last;
    _last->_next = _node;
    _self->_last = _node;
    _self->_size++;
}

void LinkedList_addValue(LinkedList* _self, void* _value, bool _alloc) 
{
    struct Node* node = Node_new(_value, _alloc);
    LinkedList_add(_self, node);
}