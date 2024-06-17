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

LinkedList* LinkedList_new(const size_t _capacity)
{
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->_head     = NULL;
    ll->_last     = NULL;
    ll->_size     = 0;
    ll->_capacity = _capacity;

    semaphore_init(&ll->_mutex, 0, 1, "LinkedList_new");

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

void LinkedList_deletet(LinkedList* _self)
{
    semaphore_wait(&_self->_mutex, "LinkedList_delete");
    LinkedList_delete(_self);
    semaphore_post(&_self->_mutex, "LinkedList_delete");
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

    if (_self->_size + 1 > _self->_capacity)
    {
        // We have reached max capacity. In this case we are going
        // to remove the first element of the list
        struct Node* first = LinkedList_remove(_self, 0);
        Node_delete(first);
    }

    struct Node* _last = _self->_last;
    _last->_next = _node;
    _node->_prev = _last;
    _self->_last = _node;
    _self->_size++;
}

void LinkedList_appendt(LinkedList* _self, struct Node* _node)
{
    semaphore_wait(&_self->_mutex, "LinkedList_append");
    LinkedList_append(_self, _node);
    semaphore_post(&_self->_mutex, "LinkedList_append");
}

void LinkedList_appendv(LinkedList* _self, void* _value, size_t _vsize) 
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_append(_self, node);
}

void LinkedList_appendvt(LinkedList* _self, void* _value, size_t _vsize)
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_appendt(_self, node);
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

    if (_self->_size + 1 > _self->_capacity)
    {
        // We have reached max capacity. In this case we are going
        // to remove the last element of the list
        struct Node* first = LinkedList_pop(_self);
        Node_delete(first);
    }

    struct Node* _head = _self->_head;
    _head->_prev = _node;
    _node->_next = _head;
    _self->_head = _node;
    _self->_size++;
}

void LinkedList_pusht(LinkedList* _self, struct Node* _node)
{
    semaphore_wait(&_self->_mutex, "LinkedList_push");
    LinkedList_push(_self, _node);
    semaphore_post(&_self->_mutex, "LinkedList_push");
}

void LinkedList_pushv(LinkedList* _self, void* _value, size_t _vsize) 
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_push(_self, node);
}

void LinkedList_pushvt(LinkedList* _self, void* _value, size_t _vsize)
{
    struct Node* node = Node_new(_value, _vsize);
    LinkedList_pusht(_self, node);
}

struct Node* LinkedList_popt(LinkedList* _self)
{
    semaphore_wait(&_self->_mutex, "LinkedList_pop");
    struct Node* node = LinkedList_pop(_self);
    semaphore_post(&_self->_mutex, "LinkedList_pop");

    return node;
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

struct Node* LinkedList_remove(LinkedList* _self, int _i)
{
    if (LinkedList_isEmpty(_self))
    {
        fprintf(stderr, "[LinkedList_remove] Cannot remove the item, the list is empty.\n");
        exit(EXIT_FAILURE);
    }

    struct Node* node = NULL;

    for (int i = 0; i < _self->_size; i++)
    {
        node = i == 0 ? _self->_head : node->_next;

        // If it is not the index we are looking for, continue
        if (i != _i) continue;

        // If it is the last element of the list
        if (_self->_size == 1)
        {
            _self->_head = NULL;
            _self->_last = NULL;
            break;
        }
        
        // Otherwise if it is the first element
        if (i == 0)
        {
            _self->_head = node->_next;
            _self->_head->_prev = NULL;
            break;
        }

        // Otherwise we have to take the node and change all
        // the links in the Linked List
        if (node->_prev != NULL)
        {
            node->_prev->_next = node->_next;
        }

        if (node->_next != NULL)
        {
            node->_next->_prev = node->_prev;
        }

        break;
    }

    _self->_size--;

    return node;
}

struct Node* LinkedList_removet(LinkedList* _self, int _i)
{
    semaphore_wait(&_self->_mutex, "LinkedList_remove");
    struct Node* node = LinkedList_remove(_self, _i);
    semaphore_post(&_self->_mutex, "LinkedList_remove");

    return node;
}

bool LinkedList_isEmpty(LinkedList* _self)
{
    return _self->_size == 0;
}

bool LinkedList_isEmptyt(LinkedList* _self)
{
    semaphore_wait(&_self->_mutex, "LinkedList_isEmpty");
    bool isEmpty = LinkedList_isEmpty(_self);
    semaphore_post(&_self->_mutex, "LinkedList_isEmpty");

    return isEmpty;
}

size_t LinkedList_getSize(LinkedList* _self)
{
    return _self->_size;
}

size_t LinkedList_getSizet(LinkedList* _self)
{
    semaphore_wait(&_self->_mutex, "LinkedList_getSize");
    size_t size = _self->_size;
    semaphore_post(&_self->_mutex, "LinkedList_getSize");
    return size;
}