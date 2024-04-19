#include <iostream>
#include <thread>
#include <string>
using namespace std;

void foo(int * x)
{
    if (x != nullptr)
    {
        cout << *x << endl;
    }
}

int main()
{
    int *ptr = new int(1);
    thread t(foo, ptr);
    
    t.join();
    delete ptr;
    return 0;
}