#include <iostream>
#include <thread>
#include <mutex>
#include <string>

class Log
{
public:
    static Log &getInstance()
    {
        std::call_once(flag, []() {
            instance = new Log();
        });
        return *instance;
    }

    void printLog(const std::string &msg)
    {
        std::cout << __TIME__ << " " << msg << std::endl;
    }

private:
    Log() {}
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;

    static Log *instance;
    static std::once_flag flag;
};

Log *Log::instance = nullptr;
std::once_flag Log::flag;

void printError()
{
    Log::getInstance().printLog("error");
}

int main()
{
    std::thread t1(printError);
    std::thread t2(printError);
    t1.join();
    t2.join();

    return 0;
}
