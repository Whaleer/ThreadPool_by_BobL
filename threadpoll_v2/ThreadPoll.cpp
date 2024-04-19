#include "ThreadPoll.h"
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

ThreadPool::ThreadPool(int min, int max)
{
    // 实例化任务队列

    do
    {
        taskQ = new TaskQueue;

        threadIDs = new pthread_t[max];
        if (threadIDs == nullptr)
        {
            cout << "new threadsIDs fail..." << endl;
            break;
        }
        memset(threadIDs, 0, sizeof(pthread_t) * max);
        minNum = min;
        maxNum = max;
        busyNum = 0;
        liveNum = min;
        exitNum = 0;

        if (pthread_mutex_init(&mutexPool, NULL) != 0 || pthread_cond_init(&notEmpty, NULL) != 0)
        {
            cout << " mutex or condition init fail...\n";
            break;
        }

        shutdown = false;

        /*
            创建线程
            为什么需要把 this 传递给 manager 方法？
            1. manager 是静态成员方法，只能访问类的静态变量，不能访问类的非静态变量
            2. 必须要给静态方法传递一个实例化的对象来访问非静态的成员数据
        */
        pthread_create(&managerID, nullptr, manager, this);
        for (int i = 0; i < min; i++)
        {
            pthread_create(&threadIDs[i], nullptr, worker, this);
        }
        return;

    } while (0);

    // 释放
    if (threadsIDs)
        delete[] threadIDs;
    if (taskQ)
        delete taskQ;
}

void *ThreadPool::worker(void *arg)
{
    ThreadPool *pool = static_cast<threadPool *>(arg);
    while (true)
    {
        pthread_mutex_lock(&pool->mutexPool);
        // 当前任务队列是否为空
        while (pool->taskQ->taskNumber() = 0 && !pool->shutdown)
        {
            // 阻塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            // 判断是否要销毁线程
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                if (pool->liveNum > pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit(pool);
                }
            }
        }

        // 判断线程池是否被关闭了
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }

        // 从任务队列中取出任务
        Task task = pool->taskQ->obtainTask();

        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexPool);

        cout << "thread " << to_string(pthread_self()) << " start working...\n";

        task.function(task.arg);
        delete task.arg;
        task.arg = nullptr;

        cout << "thread " << to_string(pthread_self()) << " end working...\n";

        pthread_mutex_lock(&pool->mutexPool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);
    }
    return nullptr;
}