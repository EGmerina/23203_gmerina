#pragma once
#include <iostream>
#include <memory.h>

template <typename T>
class Vector
{
public:
    Vector();
    Vector(const Vector &copied);
    Vector(Vector &&moved);
    ~Vector();

    void push(const T elem);

    T &operator[](const int index);

    T &Vector<T>::operator[](const int index) const;

    Vector &operator=(const Vector &v);

    Vector &operator=(Vector &&v);

    T &back();

    std ::size_t size() const;

    std::size_t capacity();

    void print();

    void Vector<T>::swap(Vector<T> &b);

    void Vector<T>::clear();

    void Vector<T>::erase(const std ::size_t pos);

    void Vector<T>::insert(const std ::size_t pos, const T elem);

    bool Vector<T>::empty() const;

private:
    static constexpr int INITIAL_VEC_CAPACITY = 4;
    T *ptr = nullptr;
    std::size_t currentAmount = 0;
    std::size_t allocatedAmount = 0;

    void expandMemoryIfNeeded();
};
