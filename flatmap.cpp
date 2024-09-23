#include <string>
#include <vector>
#include <utility>
#include <cmath>

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
        this->age = 0;
        this->weight = 0;
    }
    Value(const Value &v)
    {
        this->age = v.age;
        this->weight = v.weight;
    }
    bool operator==(const Value &a)
    {
        if (this->age == a.age && this->weight == a.weight)
        {
            return true;
        }
        return false;
    }
};

class FlatMap
{
public:
    FlatMap();
    ~FlatMap()
    {
        delete &flatmap;
    }
    FlatMap(const FlatMap &b)
    {
        flatmap = b.flatmap;
    }
    FlatMap(FlatMap &&b)
    {
        flatmap.assign(b.flatmap.begin(), b.flatmap.end());
    }

    // Обменивает значения двух флетмап.
    void swap(FlatMap &b)
    {
        flatmap.swap(b.flatmap);
    }

    FlatMap &operator=(const FlatMap &b)
    {
        flatmap = b.flatmap;
        return *this;
    }
    FlatMap &operator=(FlatMap &&b)
    {
        flatmap.assign(b.flatmap.begin(), b.flatmap.end());
        return *this;
    }

    // Очищает контейнер.
    void clear()
    {
        flatmap.clear();
    }
    // Удаляет элемент по заданному ключу.
    bool erase(const Key &k)
    {
        unsigned int prev_size = flatmap.size();
        flatmap.erase(flatmap.begin() + position(k));
        if (prev_size == flatmap.size() - 1)
        {
            return true;
        }
        return false;
    }
    // Вставка в контейнер. Возвращаемое значение - успешность вставки.
    bool insert(const Key &k, const Value &v)
    {
        if (v.age > 100 || v.age < 10 || v.weight > 300 || v.weight < 20 || k.empty())
        {
            return false;
        }
        std ::pair<Key, Value> new_pair(k, v);
        flatmap.insert(flatmap.begin() + position(k), new_pair);
        return true;
    }

    // Проверка наличия значения по заданному ключу.
    bool contains(const Key &k) const
    {
        if (flatmap.at(position(k)).first == k)
        {
            return true;
        }
        return false;
    }

    // Возвращает значение по ключу. Небезопасный метод.
    // В случае отсутствия ключа в контейнере, следует вставить в контейнер
    // значение, созданное конструктором по умолчанию и вернуть ссылку на него.
    Value &operator[](const Key &k)
    {
        unsigned int k_pos = position(k);
        if (contains(k))
        {
            return flatmap.at(k_pos).second;
        }
        Value new_value = value;
        insert(k, new_value);
        return new_value;
    }

    // Возвращает значение по ключу. Бросает исключение при неудаче.
    Value &at(const Key &k)
    {
        if (contains(k))
        {
            return flatmap.at(position(k)).second;
        }
        throw "Key doesn't exist";
    }
    const Value &at(const Key &k) const
    {
        if (contains(k))
        {
            return flatmap.at(position(k)).second;
        }
        throw "const Key doesn't exist";
    }

    unsigned int size() const
    {
        return flatmap.size();
    }
    bool empty() const
    {
        return flatmap.empty();
    }

    friend bool operator==(const FlatMap &a, const FlatMap &b)
    {
        if (a.flatmap.size() != b.flatmap.size())
        {
            return false;
        }
        for (unsigned i = 0; i < a.flatmap.size(); ++i)
        {
            if (a.flatmap[i] != b.flatmap[i])
            {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const FlatMap &a, const FlatMap &b)
    {
        return !(a == b);
    }

private:
    std::vector<std ::pair<Key, Value>> flatmap;
    Value value;
    unsigned int position(const Key &k) const
    {
        unsigned beg = 0;
        unsigned end = flatmap.size();
        unsigned mid;
        for (unsigned char i = 0; i < log(flatmap.size()); ++i)
        {
            mid = (end + beg) / 2;
            if (flatmap.at(mid).first < k)
            {
                beg = mid;
                continue;
            }
            if (flatmap.at(mid).first > k)
            {
                end = mid;
                continue;
            }
            if (flatmap.at(mid).first == k)
            {
                return mid;
            }
        }
        return mid + 1;
    }
};
