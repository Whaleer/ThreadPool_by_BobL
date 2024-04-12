#include <iostream>
#include <thread>
#include <string>
using namespace std;


void foo(int & x){
    x += 1;
}

int main(){
    int a = 1;
    thread th1(foo, ref(a));
    th1.join();
    cout << a << endl;

    return 0;
}