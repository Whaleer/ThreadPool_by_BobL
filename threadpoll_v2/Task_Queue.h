#include <queue>
#include <pthread.h>
using callback = void (*)(void *arg);

// 任务的结构体
struct Task
{
    Task()
    {
        function = nullptr;
        arg = nullptr;
    }
    Task(callback f, void *arg)
    {
        this->arg = arg;
        function = f;
    }
    callback function;
    void *arg;
} Task;

// 任务队列类
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task Task);
    void addTask(callback f, void*arg);
    // 取出一个任务
    Task obtainTask();

    // 获取当前任务的个数
    inline int taskNumber()
    {
        return m_taskQ.size();
    }

private:
    pthread_mutex_t m_mutex;
    std::queue<Task> m_taskQ;
};