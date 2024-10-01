/*
try{
f();
} catch (std:: string s){
}

 */

#include <iostream>

struct A
{
};

void f()
{
    throw std ::runtime_error("error");
}

class SmartPOinterForA // RAII ключевой способ управления ресурсами
{
public:
    SmartPOinterForA(A *a = nullptr) : a_(a) {}
    ~SmartPOinterForA()
    {
        delete a_;
    }
    SmartPOinterForA(SmartPOinterForA &) = delete;
    SmartPOinterForA &operator=(SmartPOinterForA &) = delete;

private:
    A *a_;
};

void g()
{
    SmartPOinterForA s;
    SmartPOinterForA smart(new A());
    //SmartPOinterForA s2 = smart;
}

int main()
{
    try
    {
        g();
    }
    catch (...)
    {
        std::cerr << "exception" << std::endl;
    }
}
