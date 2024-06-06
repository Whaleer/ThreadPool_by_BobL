#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

/*
 *  C++线程池实现
 *  线程池:vector 保存
 *  任务队列:queue 保存
 */
class ThreadPool
{
public:
    ThreadPool(size_t threads);
    ~ThreadPool();
    template <typename F, typename... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> _workers;        // 线程池
    std::queue<std::function<void()>> _tasks; // 任务队列

    std::mutex _queueMutex;
    std::condition_variable condition;
    bool _stop;
};

inline ThreadPool::ThreadPool(size_t threads) : _stop(false)
{
    for (size_t i = 0; i < threads; ++i)
    {
        // 每个线程的任务
        // 1. 从任务队列获取任务
        // 2. 执行任务
        _workers.emplace_back([this]
                              {
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->_queueMutex);
                    // 等待唤醒
                    this->condition.wait(lock,[this]{return this->_stop || !this->_tasks.empty();});
                    if(this->_stop && this->_tasks.empty())
                        return;
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                }
                task();
            } });
    }
}

template <typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    // 将需要执行的任务函数打包(bind), 转换为参数列表为空的函数对象
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(_queueMutex);

        if (_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        // 最妙的地方，利用 lambda函数 包装线程函数，使其符合 Eunction<void()>的形式
        // 并且返回值可以通过future 获取
        _tasks.emplace([task]()
                       { (*task)(); });
    }

    this->condition.notify_all();
    return res;
}

inline ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(_queueMutex);
}