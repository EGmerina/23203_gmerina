#pragma once
#include <map>
#include <memory>
#include <iostream>

template <class Key, class Product>

class Factory
{
public:
    static Factory *getInstance()
    {
        static Factory f;
        return &f;
    }

    std::shared_ptr<Product> createUnitByName(Key name)
    {
        //std::cout << name << std::endl;

        auto *creator = creators_.at(name);
        std::shared_ptr<Product> p(creator());
        return std::move(p);
    }

    bool registerCreator(const Key &name, Product *(*creator)())
    {
        creators_[name] = creator;
        return true;
    }

private:
    std::map<Key, Product *(*)()> creators_;
};