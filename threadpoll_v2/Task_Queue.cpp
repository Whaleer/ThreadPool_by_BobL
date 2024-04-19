#include "Task_Queue.h"

TaskQueue::TaskQueue()
{
    pthread_mutex_init(&m_mutex, NULL);
}

TaskQueue::~TaskQueue()
{
    pthread_mutex_destroy(&m_mutex);
}

void TaskQueue::addTask(Task task)
{
    pthread_mutex_lock(&m_mutex);

    m_taskQ.push(task);

    pthread_mutex_unlock(&m_mutex);
}

void TaskQueue::addTask(callback f, void *arg)
{
    pthread_mutex_lock(&m_mutex);

    m_taskQ.push(Task(f, arg));

    pthread_mutex_unlock(&m_mutex);
}

Task TaskQueue::obtainTask()
{
    Task t;

    pthread_mutex_lock(&m_mutex);
    if (!m_taskQ.empty())
    {
        // 获取对头的任务
        t = m_taskQ.front();
        // 弹出
        m_taskQ.pop();
    }
    pthread_mutex_unlock(&m_mutex);

    return t;
}