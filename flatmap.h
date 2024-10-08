#pragma once
#include "vector.h"
#include <string>

typedef std::string Key;

struct Value
{
    unsigned age;
    unsigned weight;

    Value(unsigned age, unsigned weight)
    {
        this->age = age;
        this->weight = weight;
    }
    Value()
    {
        this->weight = 0;
        this->age = 0;
    }
    ~Value() {}
    Value(const Value &v)
    {
        this->age = v.age;
        this->weight = v.weight;
    }
    bool operator!=(const Value &a) const
    {
        if (this->age == a.age && this->weight == a.weight)
        {
            return false;
        }
        return true;
    }
};

struct Pair
{
    Key first;
    Value second;

    Pair() {}
    Pair(const Key k, const Value v)
    {
        first = k;
        second = v;
    }
    ~Pair() {}
};

class FlatMap
{
public:
    // default constructor
    FlatMap();

    // copy constructor
    FlatMap(const FlatMap &b);

    // move constructor
    FlatMap(FlatMap &&b);

    // swaps objects of class Flatmap
    void swap(FlatMap &b);

    // copy operator
    FlatMap &operator=(const FlatMap &b);

    // move operator
    FlatMap &operator=(FlatMap &&b);

    // cleans the container
    void clear();

    // removes an element by a  key if such an element exists
    bool erase(const Key &k);

    // inserts an element by key if there is no such element yet
    bool insert(const Key &k, const Value &v);

    // checks by key whether the element is contained in the container
    bool contains(const Key &k) const;

    // Returns the value by key.
    // If the key is missing in the container, it is inserted into the container
    // value created by the default constructor and returns a reference to it.
    Value &operator[](const Key &k);

    // Returns the value by key. Throws an exception on failure.
    Value &at(const Key &k);

    // Returns the value by key. Throws an exception on failure.
    const Value &at(const Key &k) const;

    // returns the number of elements in the container
    unsigned int size() const;

    // checks if the container is empty
    bool empty() const;

    // checking for equality of two containers
    friend bool operator==(const FlatMap &a, const FlatMap &b);

    // checking for inequality between two containers
    friend bool operator!=(const FlatMap &a, const FlatMap &b);

private:
    Vector<Pair> flatmap;
    Value value;

    std ::size_t position(const Key &k) const;
};
