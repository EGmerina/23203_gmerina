#include "gtest/gtest.h"
#include <iostream>
#include <cstdio>

// тесты на правильную раздачу карт
// возможно тесты на каждую функцию hand

// тест на ожидаемый вывод программы (результат) на каждый режим
// тесты для каждой стратегии (что они действуют как надо)

TEST(Constructors, init)
{
    /* FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.at("Vasya").age, 20); */
}

int main(int argc, char **argv)
{
    if (!freopen("output.txt", "w", stdout))
    {
        perror("Unable to reopen stdout");
        return 1;
    }
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}