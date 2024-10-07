#include "flatmap.h"
#include <string>
#include <cmath>

FlatMap::FlatMap()
{
}
FlatMap::FlatMap(const FlatMap &b)
{
    flatmap = b.flatmap;
}
FlatMap::FlatMap(FlatMap &&b)
{
    flatmap = std:: move(b.flatmap);
}

// Обменивает значения двух флетмап.
void FlatMap::swap(FlatMap &b)
{
    flatmap.swap(b.flatmap); // я тут разве не копирую?
}

FlatMap &FlatMap::operator=(const FlatMap &b)
{
    if (this == &b)
    {
        return *this;
    }
    flatmap = b.flatmap;
    return *this;
}
FlatMap &FlatMap ::operator=(FlatMap &&b)
{
    if (this == &b)
    {
        return *this;
    }
    flatmap = std ::move(b.flatmap);
    return *this;
}

// Очищает контейнер.
void FlatMap::clear()
{
    flatmap.clear();
}
// Удаляет элемент по заданному ключу.
bool FlatMap::erase(const Key &k)
{
    if (contains(k))
    {
        flatmap.erase(position(k));
        return true;
    }
    return false;
}
// Вставка в контейнер. Возвращаемое значение - успешность вставки.
bool FlatMap::insert(const Key &k, const Value &v)
{
    if (v.age > 100 || v.age < 10 || v.weight > 300 || v.weight < 20 || k.empty())
    {
        return false;
    }
    Pair new_pair(k, v);
    flatmap.insert(position(k), new_pair);
    return true;
}

// Проверка наличия значения по заданному ключу.  //std:: ranges :: binary search
bool FlatMap::contains(const Key &k) const
{
    if (flatmap[position(k)].first == k)
    {
        return true;
    }
    return false;
}

// Возвращает значение по ключу. Небезопасный метод.
// В случае отсутствия ключа в контейнере, следует вставить в контейнер
// значение, созданное конструктором по умолчанию и вернуть ссылку на него.
Value &FlatMap::operator[](const Key &k)
{
    unsigned int k_pos = position(k);
    if (contains(k))
    {
        return flatmap[k_pos].second;
    }
    insert(k, value);
    return value; // вернуть insert!!!
}

// Возвращает значение по ключу. Бросает исключение при неудаче.
Value &FlatMap::at(const Key &k)
{
    if (contains(k))
    {
        return flatmap[position(k)].second;
    }
    throw std ::string("Key doesn't exist");
}
const Value &FlatMap::at(const Key &k) const
{
    if (contains(k))
    {
        return flatmap[position(k)].second;
    }
    throw std ::string("const Key doesn't exist");
}

unsigned int FlatMap::size() const
{
    return flatmap.size();
}
bool FlatMap::empty() const
{
    return flatmap.empty();
}

bool operator==(const FlatMap &a, const FlatMap &b)
{
    if (a.flatmap.size() != b.flatmap.size())
    {
        return false;
    }
    for (unsigned i = 0; i < a.flatmap.size(); ++i)
    {
        if ((a.flatmap[i].first != b.flatmap[i].first) || (a.flatmap[i].second != b.flatmap[i].second))
        {
            return false;
        }
    }
    return true;
}
bool operator!=(const FlatMap &a, const FlatMap &b)
{
    return !(a == b);
}

std ::size_t FlatMap::position(const Key &k) const
{
    unsigned beg = 0;
    unsigned end = flatmap.size();
    int mid = -1;
    for (unsigned char i = 0; i < log(flatmap.size()); ++i)
    {
        mid = (end + beg) / 2;
        if (flatmap[mid].first < k)
        {
            beg = mid;
            continue;
        }
        if (flatmap[mid].first > k)
        {
            end = mid;
            continue;
        }
        if (flatmap[mid].first == k)
        {
            return mid;
        }
    }
    return (unsigned)(mid + 1);
}
