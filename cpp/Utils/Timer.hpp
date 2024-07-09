/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name Timer.hpp                                                            *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 08 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains the declaration of the Timer class and other     *****
 *****   functions to interact with it. This timer is able to compute with double   *****
 *****   precision an elapsed time, and to sleep for floating-point values.         *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

#ifndef _TIMER_H
#define _TIMER_H

#include <thread>

#define NS_PER_SECOND 1000000000

namespace Utils
{
    /**
     * @class Timer
     * 
     * This class represents a simple timer. It can be used to measure
     * the elapsed time between the message being sent and the response
     * being received by the receiver. It uses nanoseconds precision.
     * The timer starts by calling the `start` function. 
     */
    class Timer
    {
        private:
            using nanoseconds = std::chrono::nanoseconds;
            using time_point = std::chrono::time_point<std::chrono::steady_clock>;

            
            nanoseconds _elapsed;  //!< The total elapsed time from the start
            bool        _running;  //!< Is true if the corresponding thread is running
            time_point  _start;    //!< The time when the timer has started
            time_point  _current;  //!< The current timer position wrt to the start
            time_point  _previous; //!< The time when the `getDeltaTime` function is called

        public:
            Timer();
            ~Timer()
            {
                this->stop();
            }

            /**
             * @brief Starts the timer as a detached thread.
             */
            void start();

            /**
             * @brief The function executed when the thread has started
             */
            void run();

            /**
             * @brief Stop the current running thread of the timer
             */
            void stop();

            /**
             * @brief Returns, in nanoseconds, the time passed between the
             * start of the timer and the time this function is called. Every
             * time this method is used the start time is overwritten with
             * the current time.
             * 
             * @return A time in nanoseconds
             */
            int getDeltaTime();

            /**
             * @brief Returns the elapsed time, in nanoseconds.
             * 
             * @return A time in nanoseconds
             */
            int getElapsedTime();

            /**
             * @brief Reset the start time of the timer to current time.
             */
            void reset();
            
            /**
             * @brief Reset the previous time of the timer to current time.
             */
            void resetPrevious();

            /**
             * @brief Sleep for a given number of seconds (input can be floating point values)
             */
            static void sleep(double delay);
    };
};

#endif