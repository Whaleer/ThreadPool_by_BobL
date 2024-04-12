#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

class ThreadPool
{
public:
    ThreadPool(int numThreads) : stop(false)
    {
        for (int i = 0; i < numThreads; i++)
        {
            threads.emplace_back([this]
                                 {
                while(1){
                    unique_lock<mutex> lock(mtx);
                    cond.wait(lock, [this] {
                        return !tasks.empty() || stop;
                        });
                    
                    if(stop && tasks.empty()){
                        return;
                    }

                    function<void()> task(move(tasks.front())); 
                    tasks.pop();
                    lock.unlock();
                    task();
                } });
        }
    }

    ~ThreadPool()
    {
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }

        cond.notify_all();
        for (auto &t : threads)
        {
            t.join();
        }
    }

    template <class F, class... Args>
    void enqueue(F &&f, Args &&... args)
    {
        function<void()> task = bind(forward<F>(f), forward<Args>(args)...);
        {
            unique_lock<mutex> lock(mtx);
            tasks.emplace(move(task));
        }
        cond.notify_one();
    }

private:
    vector<thread> threads;        // 需要一个线程数组
    queue<function<void()>> tasks; // 需要一个任务队列

    mutex mtx;               // 需要一个锁
    condition_variable cond; // 线程池符合生产者消费者模型，需要条件变量

    bool stop;

public:
};

int main()
{

    return 0;
}