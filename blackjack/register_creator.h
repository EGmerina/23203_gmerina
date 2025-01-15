#pragma once
#include "factory.h"

template <typename Class, typename Subclass>
Class *creator(int N = void)
{
    return new Subclass(N);
}

template <typename Class, typename Subclass>
struct RegisterCreator
{
    RegisterCreator() = delete;
    RegisterCreator(std::string name)
    {
        Factory<std::string, Class>::getInstance()->registerCreator(name, creator<Class, Subclass>);
    }
};
