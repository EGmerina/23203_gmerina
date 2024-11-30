#pragma once

template <typename Class, typename Subclass>
struct RegisterCreator
{
    RegisterCreator() = delete;
    RegisterCreator(std::string name)
    {
        Factory<std::string, Class>::getInstance()->registerCreator(name, []() -> auto
                                                                    { return new Subclass(); });
    }
};
