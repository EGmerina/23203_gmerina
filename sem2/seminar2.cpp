#include <iostream>
struct A
{
    A() { std::cout << "ctor" << std::endl; }
    A(const A &) { std ::cout << "copy" << std ::endl; }
    A &operator=(const A &)
    {
        std ::cout << "operator=" << std::endl;
        return *this;
    }
    ~A() { std::cout << "dtor" << std::endl; } // endl освобождает буфер

    int getField() const
    {
        return field;
    }

private:
    int field = 100500;
};
// why calloc is better then malloc?
int main() // new вызывает конструктор, malloc нет
{
    A a;

    // A *arr = (A *)calloc(10, sizeof(A));
    A *arr = new A[10];
    delete[] arr; // вызывает деструктор и освобождает память //[] !!!!!
}