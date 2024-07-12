/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name MessageQueue.hpp                                                     *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 08 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declarations of a MessageQueue object class  *****
 *****   and a number of functions to interact with it.                             *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/


#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#pragma once

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
            bool isEmpty()
            {
                return this->_queue.empty();
            }

            /**
             * @brief Check if the queue is full of elements or not
             * @return True if the queue is full, False otherwise
             */
            bool isFull()
            {
                return this->_queue.size() == this->_capacity;
            }

            /**
             * @brief Get the current size of the Message Queue
             * @returns The size of the queue
             */
            std::size_t getSize() const
            {
                return this->_queue.size();
            }

            /**
             * @brief Get the maximum Capacity of the Message Queue
             * @returns The maximum capacity (read-only)
             */
            std::size_t getCapacity() const
            {
                return this->_capacity;
            }

            /**
             * @brief Access the first element of the queue. Do not remove it.
             * @return A reference to the first element.
             */
            T getFirstElement()
            {
                if (this->isEmpty())
                {
                    throw std::runtime_error(
                        "[MessageQueue::getFirstElement] The queue is empty.");
                }

                return this->_queue.front();
            }

            /**
             * @brief Access the last element of the queue. Do not remove it.
             * @return A reference to the last element.
             */
            T getLastElement()
            {
                if (this->isEmpty())
                {
                    throw std::runtime_error(
                        "[MessageQueue::getLastElement] The queue is empty.");
                }

                return this->_queue.back();
            }

            /**
             * @brief Add an element at the end of the queue
             * 
             * If the queue is full, meaning that the current size of the queue
             * match the given capacity, the first element of the queue is removed.
             * 
             * @param _element The element to be added
             */
            void push(const T& _element)
            {
                // Check that the queue is not full
                if (this->isFull())
                {
                    // If the queue is full, we need to remove the first element
                    this->_queue.pop();
                }

                this->_queue.push(_element);
            }

            /**
             * @brief Add an element at the end of the queue (Thread-safe)
             * 
             * Perform the MessageQueue::push method.
             * 
             * @param _element The element to be added
             */
            void pusht(const T& _element)
            {
                // Aquire the lock and wait to proceed
                std::unique_lock<decltype(this->_mutex)> mlock(this->_mutex);
                this->_cond.wait(mlock);

                this->push(_element);

                // Release the lock and notify all waiting threads
                mlock.unlock();
                this->_cond.notify_all();
            }

            /**
             * @brief Remove and returns the first element of the queue.
             * @return A reference to the first element of the queue.
             * @throw std::runtime_error if the queue is empty
             */
            T pop()
            {

                T element = this->getFirstElement();
                this->_queue.pop();

                return element;
            }

            /**
             * @brief Remove and returns the first element of the queue (Thread-safe).
             * @return A reference to the first element of the queue.
             * @throw std::runtime_error if the queue is empty
             */
            T popt()
            {
                // Aquire the lock and wait to proceed
                std::unique_lock<decltype(this->_mutex)> mlock(this->_mutex);
                this->_cond.wait(mlock);

                // We need to check that the queue is not empty
                if (this->isEmpty())
                {
                    mlock.unlock();
                    this->_cond.notify_all();

                    throw std::runtime_error("[MessageQueue::pop] The queue is empty.");
                }

                T retval = this->pop();

                // Release the lock and notify all waiting threads
                mlock.unlock();
                this->_cond.notify_all();

                return retval;
            }
    };
}

#endif