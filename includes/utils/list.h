/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name list.h                                                               *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 01 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declarations of a Linked List object struct  *****
 *****   and a number of functions to interact with it.                             *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _LIST_H
#define _LIST_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <utils/synch.h>
#include "utils/version.h"

__BEGIN_DECLS

/**
 * Just a single node inside a Linked List.
 */
struct Node
{

    struct Node* _next;    //!< A pointer to the next value in the linked list
    struct Node* _prev;    //!< A pointer to the previous value in the linked list
    void*        _value;   //!< The current value

};

/**
 * A simple Linked List with following values: `_head`, `_last`, `_size` and `_max` 
 */ 
typedef struct
{

    struct Node* _head;      //!< The first value of the list
    struct Node* _last;      //!< The last value of the list
    size_t       _size;      //!< The number of elements in the list
    size_t       _capacity;  //!< The maximum number of elements of the list
    sem_t        _mutex;     //!< We need to be thread safe

} LinkedList;

/**
 * Construct and return the pointer to a new Node initialized with given input values. 
 * Notice that, this function dynamically allocate memory when creating the new Node. 
 * Hence, the node should be freed using the Node_delete function. If the node is in
 * a Linked List, then all nodes will be freed when the Linked List is deleted. 
 */
extern struct Node* Node_new(void *_value, size_t _vsize) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Free the memory allocated for the input Node
 */
extern void Node_delete(struct Node* _self) __attribute__((nonnull));

/**
 * Initialize an empty Linked List. This function dynamically allocate memory, 
 * this means that the corresponding object must be deleted using the 
 * LinkedList_delete() function.
 */
extern LinkedList* LinkedList_new(const size_t _capacity) __attribute__((returns_nonnull));

/**
 * Free the memory allocated for the entire linked list (including nodes)
 */
extern void LinkedList_delete(LinkedList* _self) __attribute__((nonnull));

/**
 * Free the memory allocated for the entire linked list (including nodes, Thread Safe)
 */
extern void LinkedList_deletet(LinkedList* _self) __attribute__((nonnull));

/**
 * Append the input Node into the Linked List
 */
extern void LinkedList_append(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/**
 * Append the input Node into the Linked List (Thread safe)
 */
extern void LinkedList_appendt(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/**
 * Create and append a new Node, given the input value and value size, to the Linked List
 */
extern void LinkedList_appendv(LinkedList* _self, void* _value, size_t _vsize) __attribute__((nonnull));

/**
 * Create and append a new Node, given the input value and value size, to the Linked List (Thread Safe)
 */
extern void LinkedList_appendvt(LinkedList* _self, void* _value, size_t _vsize) __attribute__((nonnull));

/**
 * Add the input node at the head of the list
 */
extern void LinkedList_push(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/**
 * Add the input node at the head of the list (Thread Safe)
 */
extern void LinkedList_pusht(LinkedList* _self, struct Node* _node) __attribute__((nonnull));

/**
 * Create and add a new Node at the head of the list
 */
extern void LinkedList_pushv(LinkedList* _self, void* _value, size_t _vsize) __attribute__((nonnull));

/**
 * Create and add a new Node at the head of the list (Thread safe)
 */
extern void LinkedList_pushvt(LinkedList* _self, void* _value, size_t _vsize) __attribute__((nonnull));

/**
 * Returns the last node of the Linked List
 */
extern struct Node* LinkedList_pop(LinkedList* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Returns the last node of the Linked List (Thread safe)
 */
extern struct Node* LinkedList_popt(LinkedList* _self) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Returns the element of the list corresponding to input index
 */
extern struct Node* LinkedList_remove(LinkedList* _self, int _i) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Returns the element of the list corresponding to input index (Thread-safe)
 */
extern struct Node* LinkedList_removet(LinkedList* _self, int _i) __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Checks if the linked list is empty or not
 */
extern bool LinkedList_isEmpty(LinkedList* _self) __attribute__((nonnull));

/**
 * Checks if the linked list is empty or not (Thread-safe)
 */
extern bool LinkedList_isEmptyt(LinkedList* _self) __attribute__((nonnull));

/**
 * Returns the actual size of the linked List
 */
extern size_t LinkedList_getSize(LinkedList* _self) __attribute__((nonnull));

/**
 * Returns the actual size of the linked List (Thread safe)
 */
extern size_t LinkedList_getSizet(LinkedList* _self) __attribute__((nonnull));


__END_DECLS

#endif