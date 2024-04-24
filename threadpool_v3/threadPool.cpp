#include <mutex>
#include <queue>

template <typename T>
class SafeQueue
{
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;

public:
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) {}
    ~SafeQueue() {}

    bool empty() // 返回队列是否为空
    {
        // 互斥信号变量加锁，防止m_queue被改变
        std::unique_lock<std::mutex> lock(m_mutex); // 这里是自动锁定
        return m_queue.empty();
    }

    int size()
    {
        // 互斥信号变量加锁，防止m_queue被改变
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    // 队列添加任务
    void enqueue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(t);
    }

    // 队列取出任务
    bool dequeue(T &t)
    {
        std::unique_lock<std::mutex> lock(m_mutex); // 队列加锁

        if (m_queue.empty())
            return false;
        t = std::move(m_queue.front()); // 取出队首任务，返回队首元素值，并进行右值引用
        m_queue.pop();                  // 弹出入队的第一个元素

        return true;
    }
}

class ThreadPool
{
private:
public:
public:
    /*
     *  typename... Args：这是一个可变参数模板，表示函数可以接受任意数量的参数，
     *  且这些参数的类型可以不同。Args... 是一个模板参数包，表示零个或多个额外的参数
     */
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        // Create a function with bounded parameter ready to execute
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // 连接函数和参数定义，特殊函数类型，避免左右值错误

        // Encapsulate it into a shared pointer in order to be able to copy construct
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        // Warp packaged task into void function
        std::function<void()> warpper_func = [task_ptr]()
        {
            (*task_ptr)();
        };

        // 队列通用安全封包函数，并压入安全队列
        m_queue.enqueue(warpper_func);

        // 唤醒一个等待中的线程
        m_conditional_lock.notify_one();

        // 返回先前注册的任务指针
        return task_ptr->get_future();
    }
}