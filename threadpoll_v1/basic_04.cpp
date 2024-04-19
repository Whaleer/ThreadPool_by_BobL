#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int shared_data = 0;
mutex mtx;
void func()
{
    for (int i = 0; i < 100000; i++) 
    {
        mtx.lock();
        shared_data += 1;
        mtx.unlock();
    }
}

int main()
{

    thread t1(func);
    thread t2(func);
    t1.join();
    t2.join();

    cout << shared_data << endl;

    return 0;
}