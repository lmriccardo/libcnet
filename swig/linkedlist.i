%module linkedlist

%{
    #include "utils/synch.h"
    #include "utils/list.h"
%}

extern struct Node* Node_new(void *_value, size_t _vsize);
extern void Node_delete(struct Node* _self);
extern LinkedList* LinkedList_new(const size_t _capacity);
extern void LinkedList_delete(LinkedList* _self);
extern void LinkedList_deletet(LinkedList* _self);
extern void LinkedList_append(LinkedList* _self, struct Node* _node);
extern void LinkedList_appendt(LinkedList* _self, struct Node* _node);
extern void LinkedList_appendv(LinkedList* _self, void* _value, size_t _vsize);
extern void LinkedList_appendvt(LinkedList* _self, void* _value, size_t _vsize);
extern void LinkedList_push(LinkedList* _self, struct Node* _node);
extern void LinkedList_pusht(LinkedList* _self, struct Node* _node);
extern void LinkedList_pushv(LinkedList* _self, void* _value, size_t _vsize);
extern void LinkedList_pushvt(LinkedList* _self, void* _value, size_t _vsize);
extern struct Node* LinkedList_pop(LinkedList* _self);
extern struct Node* LinkedList_popt(LinkedList* _self);
extern struct Node* LinkedList_remove(LinkedList* _self, int _i);
extern struct Node* LinkedList_removet(LinkedList* _self, int _i);
extern bool LinkedList_isEmpty(LinkedList* _self);
extern bool LinkedList_isEmptyt(LinkedList* _self);
extern size_t LinkedList_getSize(LinkedList* _self);
extern size_t LinkedList_getSizet(LinkedList* _self);