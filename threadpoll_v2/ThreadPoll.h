Thre#include "Task_Queue.h"

class ThreadPool
{
public:
    ThreadPool(int min, int max);

    ~ThreadPool();

    // 添加任务
    void addTask(Task task);

    // 获取工作的线程的个数
    int getBusyNum();

    // 获取 live 的线程个数
    int getAliveNum();

private:
    static void *worker(void *arg);
 
    static void *manager(void *arg);

    void threadExit();

private:
    // 任务队列
    TaskQueue *taskQ;

    pthread_t managerID;       // 管理者线程ID
    pthread_t *threadIDs;      // 工作的线程ID
    int minNum;                // 最小线程数量
    int maxNum;                // 最大线程数量
    int busyNum;               // 忙的线程的个数
    int liveNum;               // 存活的线程的个数
    int exitNum;               // 要销毁的线程个数
    pthread_mutex_t mutexPool; // 锁整个线程池
    pthread_cond_t notEmpty;   // 任务队列是不是空了

    bool shutdown; // 是否销毁线程池
};