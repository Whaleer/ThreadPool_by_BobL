// lock_guard

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int shared_data = 0;
mutex mtx;

void func()
{
    for (int i = 0; i < 10000; i++)
    {   
        // 在构造 lg 对象时,就对 mtx 进行了加锁
        // 在每次循环迭代结束时，lg对象的析构函数会被调用,锁被释放
        lock_guard<mutex> lg(mtx);
        shared_data++;
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