#ifndef _LIST_H
#define _LIST_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

__BEGIN_DECLS

struct Node
{

    struct Node* _next;    // A pointer to the next value in the linked list
    struct Node* _prev;    // A pointer to the previous value in the linked list
    void*        _value;   // The current value

};

typedef struct
{

    struct Node* _head; // The first value of the list
    struct Node* _last; // The last value of the list
    size_t       _size; // The number of elements in the list

} LinkedList;

/* Construct and return the pointer to a new Node initialized with given input values. 
   Notice that, this function dynamically allocate memory when creating the new Node. 
   Hence, the node should be freed using the Node_delete function. If the node is in
   a Linked List, then all nodes will be freed when the Linked List is deleted. The
   second parameter `_alloc` is a boolean flag that indicates whether `_value` has
   been allocated using any malloc, calloc etc.
*/
extern struct Node* Node_new(void *_value, size_t _vsize) __attribute__((nonnull)) 
    __attribute__((returns_nonnull));

/* Free the memory allocated for the input Node */
extern void Node_delete(struct Node* _self) __attribute__((nonnull));

/* Initialize an empty Linked List. This function dynamically allocate memory, 
   this means that the corresponding object must be deleted using the 
   LinkedList_delete() function.  
*/
extern LinkedList* LinkedList_new() __attribute__((returns_nonnull));

/* Free the memory allocated for the entire linked list (including nodes) */
extern void LinkedList_delete(LinkedList* _self) __attribute__((nonnull));

/* Append the input Node into the Linked List */
extern void LinkedList_add(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/* Create and append a new Node, given the input value and value size, to the Linked List */
extern void LinkedList_addv(LinkedList* _self, void* _value, size_t _vsize) 
    __attribute__((nonnull));

/* Add the input node at the head of the list  */
extern void LinkedList_push(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/* Create and add a new Node at the head of the list */
extern void LinkedList_pushv(LinkedList* _self, void* _value, size_t _vsize) 
    __attribute__((nonnull));

/* Returns the last node of the Linked List */
extern struct Node* LinkedList_pop(LinkedList* _self) __attribute__((nonnull)) 
    __attribute__((returns_nonnull));

extern struct Node* LinkedList_popi(LinkedList* _self, int _i) __attribute__((nonnull)) 
    __attribute__((returns_nonnull));


__END_DECLS

#endif