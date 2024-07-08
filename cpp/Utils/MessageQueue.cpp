#include "MessageQueue.hpp"

template <class T>
bool Utils::MessageQueue<T>::isEmpty()
{
    return this->_queue.empty();
}

template <class T>
bool Utils::MessageQueue<T>::isFull()
{
    return this->_queue.size() == this->_capacity;
}

template <class T>
std::size_t Utils::MessageQueue<T>::getSize() const
{
    return this->_queue.size();
}

template <class T>
T Utils::MessageQueue<T>::getFirstElement()
{
    return this->_queue.front();
}

template <class T>
T Utils::MessageQueue<T>::getLastElement()
{
    return this->_queue.back();
}

template <class T>
void Utils::MessageQueue<T>::push(const T &_element)
{
    // Aquire the lock and wait to proceed
    std::unique_lock<decltype(this->_mutex)> mlock(this->_mutex);
    this->_cond.wait(mlock);
    
    // Check that the queue is not full
    if (this->isFull())
    {
        // If the queue is full, we need to remove the first element
        this->_queue.pop();
    }

    this->_queue.push(_element);

    // Release the lock and notify all waiting threads
    mlock.unlock();
    this->_cond.notify_all();
}

template <class T>
T Utils::MessageQueue<T>::pop()
{
    // Aquire the lock and wait to proceed
    std::unique_lock<decltype(this->_mutex)> mlock(this->_mutex);
    this->_cond.wait(mlock);

    // We need to check that the queue is not empty
    if (this->isEmpty())
    {
        mlock.unlock();
        this->_cond.notify_all();

        throw std::runtime_error("[MessageQueue_pop] The queue is empty.");
    }

    T element = this->getFirstElement();
    this->_queue.pop();

    // Release the lock and notify all waiting threads
    mlock.unlock();
    this->_cond.notify_all();

    return element;
}
