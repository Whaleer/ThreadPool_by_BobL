#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
using namespace std;

queue<int> g_queue;
condition_variable g_cv;
mutex mtx;
void Producer()
{
    for (int i = 0; i < 10; i++)
    {
        {
            unique_lock<mutex> lock(mtx);
            g_queue.push(i);
            cout << "Producer : " << i << endl;
            // 通知消费者来取任务
            g_cv.notify_one();
        }
        this_thread::sleep_for(chrono::microseconds(100));
    }
}

void Consumer()
{
    while (1)
    {
        unique_lock<mutex> lock(mtx);

        // 如果队列为空，则需等待
        bool isempty = g_queue.empty();
        g_cv.wait(lock, [](){return !g_queue.empty();});
        int value = g_queue.front();
        g_queue.pop();
        cout << "Consumer : " << value << endl;
    }
}

int main()
{
    thread t1(Producer);
    thread t2(Consumer);
    t1.join();
    t2  .join();

    return 0;
}