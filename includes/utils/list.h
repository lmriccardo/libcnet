#ifndef _LIST_H
#define _LIST_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

__BEGIN_DECLS

struct Node
{

    void* _next;    // A pointer to the next value in the linked list
    void* _prev;    // A pointer to the previous value in the linked list
    void* _value;   // The current value
    bool  _alloc;   // If the data has been allocated or not

};

typedef struct
{

    struct Node* _head; // The first value of the list
    struct Node* _last; // The last value of the list
    size_t       _size; // The number of elements in the list

} LinkedList;

/* Free the memory allocated for the input Node */
extern void Node_delete(struct Node* _self) __attribute__((nonnull));

/* Initialize an empty Linked List. This function dynamically allocate memory, 
   this means that the corresponding object must be deleted using the 
   LinkedList_delete() function.  
*/
extern LinkedList* LinkedList_new() __attribute__((returns_nonnull));

/* Initialize a linked list with the input node as first element. This function 
   dynamically allocate memory, this means that the corresponding object must be 
   deleted using the LinkedList_delete() function. 
*/
extern LinkedList* LinkedList_new_v2(struct Node* _node) __attribute__((returns_nonnull));

/* Free the memory allocated for the entire linked list */
extern void LinkedList_delete(LinkedList* _self) __attribute__((nonnull));

extern struct Node* LinkedList_pop(LinkedList* _self) 
    __attribute__((nonnull)) __attribute__((returns_nonnull));

__END_DECLS

#endif