/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name LinkedList.hpp                                                       *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 08 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declarations of a Linked List object struct  *****
 *****   and a number of functions to interact with it.                             *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <queue>

namespace Utils
{
    /**
     * @class MessageQueueu
     * 
     * This class represents a wrapper around a classical C++ queue
     * that implements thread-safe push and pop.
     */
    template <class T>
    class MessageQueue
    {
        private:
            std::queue<T>           _queue;    //!< The actual queue container
            std::size_t             _capacity; //!< The maximum number of elements in the queue
            std::mutex              _mutex;    //!< A Mutex to be thread-safe
            std::condition_variable _cond;     //!< A condition variable to synchronize over a mutex

        public:
            MessageQueue(const std::size_t _capacity) : _capacity(_capacity) {};
            ~MessageQueue() = default;

            /**
             * @brief Check if the queue is empty or not
             * @return True if the queue is empty, False otherwise
             */
            bool isEmpty();

            /**
             * @brief Check if the queue is full of elements or not
             * @return True if the queue is full, False otherwise
             */
            bool isFull();

            /**
             * @brief Get the current size of the Message Queue
             * @returns The size of the queue
             */
            std::size_t getSize() const;

            /**
             * @brief Access the first element of the queue. Do not remove it.
             * @return A reference to the first element.
             */
            T getFirstElement();

            /**
             * @brief Access the last element of the queue. Do not remove it.
             * @return A reference to the last element.
             */
            T getLastElement();

            /**
             * @brief Add an element at the end of the queue
             * 
             * If the queue is full, meaning that the current size of the queue
             * match the given capacity, the first element of the queue is removed.
             * 
             * @param _element The element to be added
             */
            void push(const T& _element);

            /**
             * @brief Remove and returns the first element of the queue.
             * @return A reference to the first element of the queue.
             * @throw std::runtime_error if the queue is empty
             */
            T pop();
    };
}

#endif