#include <iostream>
#include <thread>
#include <string>
using namespace std;

void print(string msg)
{
    cout << msg << endl;
}

int main()
{

    thread thread_one(print, "thread created.");
    thread_one.join(); // 等待线程 thread_one 结束并清理资源（会阻塞）
    thread_one.detach(); // 将thread_one线程与主线程分离，彼此独立执行（此函数必须在线程创建时立即调用，且调用此函数会使其不能被join）
    thread_one.joinable(); // 返回一个 bool , 判断线程是否可以执行join函数
    return 0;
}
