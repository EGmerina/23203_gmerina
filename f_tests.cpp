#include "flatmap.h"
#include "gtest/gtest.h"
#include <iostream>
// flag -lgtest !!!!!!

TEST(Constructors, init)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.at("Vasya").age, 20);
}

TEST(Constructors, copy)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.at("Vasya").age, 20);
    FlatMap f1(f);
    EXPECT_EQ(f1.at("Vasya").age, 20);
}

TEST(Constructors, move)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1(std::move(f));
    EXPECT_EQ(f1.at("Vasya").age, 20);
    EXPECT_NE(f.at("Vasya").age, 20);
}

/*TEST(Operators, copy)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1(std::move(f));
    EXPECT_EQ(f1.at("Vasya").age, 20);
    EXPECT_NE(f.at("Vasya").age, 20);
}

TEST(Operators, move)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1(std::move(f));
    EXPECT_EQ(f1.at("Vasya").age, 20);
    EXPECT_NE(f.at("Vasya").age, 20);
}
*/
/*TEST(MyTests, test_1)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.at("Vasya").age, 20);
    std::cout << "insert is successful" << std::endl;
}

struct Constructor : public testing::Test
{
    FlatMap f;

    void SetUp();    // аналог конструктора
    void TearDown(); // аналог деструктора
};

TEST_F(Constructor, test_1)
{
} */

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
