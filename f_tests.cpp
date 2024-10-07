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
    try
    {
        EXPECT_NE(f.at("Vasya").age, 20);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
}

TEST(Operators, copy)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1;
    f1 = f;
    EXPECT_EQ(f1.at("Vasya").age, 20);
}

TEST(Operators, move)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1;
    f1 = std::move(f);
    EXPECT_EQ(f1.at("Vasya").age, 20);
    try
    {
        EXPECT_NE(f.at("Vasya").age, 20);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
}

TEST(Operators, equal)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1;
    f1.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f1 == f, true);
    FlatMap f2;
    f2.insert("Lena", Value(30, 60));
    EXPECT_EQ(f2 == f, false);
}

TEST(Operators, not_equal)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1;
    f1.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f1 != f, false);
    FlatMap f2;
    f2.insert("Lena", Value(30, 60));
    EXPECT_EQ(f2 != f, true);
}

TEST(Operators, get_elem)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    f.insert("Lena", Value(30, 60));
    EXPECT_EQ(f["Vasya"].age, 20);
    EXPECT_EQ(f["Vasya"].weight, 70);
    EXPECT_EQ(f["Lena"].age, 30);
    EXPECT_EQ(f["Lena"].weight, 60);
    EXPECT_EQ(f["Yana"].age, 0);
    EXPECT_EQ(f["Yana"].weight, 0);
}

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
