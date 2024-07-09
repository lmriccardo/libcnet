#include "Timer.hpp"

Utils::Timer::Timer()
{
    this->_running = false;
    this->_start = std::chrono::steady_clock::now();
    this->_previous = this->_start;
}

void Utils::Timer::start()
{
    this->_running = true;
    std::thread timerth(&Timer::run, this);
    timerth.detach();
}

void Utils::Timer::run()
{
    while (this->_running)
    {
        // Update the current value
        this->_current = std::chrono::steady_clock::now();
    }
}

void Utils::Timer::stop()
{
    if (!this->_running) return;
    this->_running = false;
}

int Utils::Timer::getDeltaTime()
{
    nanoseconds dtime = this->_current - this->_previous;
    this->_previous = this->_current;
    return dtime.count();
}

int Utils::Timer::getElapsedTime()
{
    nanoseconds dtime = this->_current - this->_start;
    return dtime.count();
}

void Utils::Timer::reset()
{
    this->_start = std::chrono::steady_clock::now();
}

void Utils::Timer::resetPrevious()
{
    this->_previous = std::chrono::steady_clock::now();
}

void Utils::Timer::sleep(double delay)
{
    auto startTime = std::chrono::steady_clock::now();
    nanoseconds _delay(static_cast<std::uint64_t>(delay * NS_PER_SECOND));

    while (true)
    {
        auto endTime = std::chrono::steady_clock::now();
        nanoseconds elapsed = endTime - startTime;

        if (elapsed > _delay) break;
    }
}
