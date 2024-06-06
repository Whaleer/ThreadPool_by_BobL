#include <iostream>
#include <functional>

// 普通函数
void regularFunction() {
    std::cout << "Regular function called" << std::endl;
}

// 函数对象
struct Functor {
    void operator()() const {
        std::cout << "Functor called" << std::endl;
    }
};

int main() {
    // 使用普通函数
    std::function<void()> func = regularFunction;
    func();

    // 使用lambda表达式
    func = []() {
        std::cout << "Lambda called" << std::endl;
    };
    func();

    // 使用函数对象
    func = Functor();
    func();

    return 0;
}