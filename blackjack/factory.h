#pragma once
#include <iostream>
#include <map>
#include <memory>

template <class Key, class Product>

class Factory
{
public:
    Factory(const Factory &b) = delete;
    Factory &operator=(const Factory &b) = delete;

    static Factory *getInstance()
    {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Product> createUnitByName(Key name)
    {
        auto *creator = creators_.at(name);
        std::unique_ptr<Product> p(creator());
        return std::move(p);
    }

    bool registerCreator(const Key &name, Product *(*creator)()) // TODO проверить что не отрабатывает 2 раза //проверить на наличие
    {
        creators_[name] = creator;
        return true;
    }

private:
    Factory() = default;
    ~Factory() = default;
    std::map<Key, Product *(*)()> creators_;
};