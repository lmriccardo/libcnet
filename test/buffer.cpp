#include <iostream>
#include <Utils/ByteBuffer.hpp>
#include <Utils/Timer.hpp>
#include <Utils/MessageQueue.hpp>
#include <arpa/inet.h>

void test_ByteBuffer()
{
    Utils::ByteBuffer bb(8);
    bb.put(8);
    bb.put(0);
    bb.putShort(0);
    bb.putShort(htons(101));
    bb.putShort(htons(102));

    bb.writeToFile("output.bin");
}

void test_Timer()
{
    Utils::Timer t;
    t.start();

    std::cout << "Timer thread has started ... Waiting for 1 sec Elapsed Time" << std::endl;
    auto eta = t.getElapsedTime();

    while (eta < 2 * NS_PER_SECOND)
    {
        eta = t.getElapsedTime();
    }

    std::cout << "Compute Elapsed Time: " << eta << std::endl;
    std::cout << "Stopping the Timer thread ..." << std::endl;

    t.stop();

    std::cout << "Sleeping for .5 seconds" << std::endl;
    Utils::Timer::sleep(0.5);
    std::cout << "End sleeping" << std::endl;
}

void test_MessageQueue()
{
    Utils::MessageQueue<int> mq(5);
    
    std::cout << "Check if the Queue is Empty: " << mq.isEmpty() << std::endl;
    std::cout << "Maximum capacity: " << mq.getCapacity() << std::endl;

    try
    {
        int val = mq.getFirstElement();
    }
    catch (const std::runtime_error& re)
    {
        std::cout << re.what() << std::endl;
    }

    try
    {
        int val = mq.getLastElement();
    }
    catch (const std::runtime_error& re)
    {
        std::cout << re.what() << std::endl;
    }

    for (int i = 0; i < mq.getCapacity(); i++)
    {
        mq.push(i);
    }

    std::cout << "Current Size: " << mq.getSize() << std::endl;

    while (!mq.isEmpty())
    {
        int element = mq.pop();
        std::cout << "Element: " << element << std::endl;
    }

    std::cout << "Current Size: " << mq.getSize() << std::endl;
}

int main(void)
{
    std::cout << "Testing ByteBuffer ...." << std::endl;
    test_ByteBuffer();
    std::cout << std::endl;

    std::cout << "Testing Timer ...." << std::endl;
    test_Timer();
    std::cout << std::endl;

    std::cout << "Testing MessageQueue ...." << std::endl;
    test_MessageQueue();
    std::cout << std::endl;

    return 0;
}