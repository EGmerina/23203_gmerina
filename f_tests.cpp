#include "flatmap.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(MyTests, test_1)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    ASSERT_EQ(f.at("Vasya").age, 20);
    std::cout << "insert is successful" << std::endl;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
