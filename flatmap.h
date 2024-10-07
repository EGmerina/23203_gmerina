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
    FlatMap();

    FlatMap(const FlatMap &b);

    FlatMap(FlatMap &&b);

    void swap(FlatMap &b);

    FlatMap &operator=(const FlatMap &b);

    FlatMap &operator=(FlatMap &&b);

    void clear();

    bool erase(const Key &k);

    bool insert(const Key &k, const Value &v);

    bool contains(const Key &k) const;

    Value &operator[](const Key &k);

    Value &at(const Key &k);

    const Value &at(const Key &k) const;

    unsigned int size() const;

    bool empty() const;

    friend bool operator==(const FlatMap &a, const FlatMap &b);

    friend bool operator!=(const FlatMap &a, const FlatMap &b);

private:
    Vector<Pair> flatmap;
    Value value;

    std ::size_t position(const Key &k) const;
};
