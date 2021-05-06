#include <iostream>
using namespace std;

class Singleton
{
public:
    // 注意返回的是引用
    static Singleton& getInstance()
    {
        static Singleton value;  //静态局部变量
        return value;
    }

private:
    Singleton() = default;
    Singleton(const Singleton& other) = delete; //禁止使用拷贝构造函数
    Singleton& operator=(const Singleton&) = delete; //禁止使用拷贝赋值运算符
};

int main()
{
    Singleton& s1 = Singleton::getInstance();
    cout << &s1 << endl;

    Singleton& s2 = Singleton::getInstance();
    cout << &s2 << endl;

    return 0;
}
