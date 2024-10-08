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
    f1 = f1;
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
    f1 = std::move(f1);
    EXPECT_EQ(f1.at("Vasya").age, 20);
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

TEST(Functions, swap)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    FlatMap f1;
    f1.insert("Ilya", Value(40, 80));
    f.swap(f1);
    EXPECT_EQ(f1["Vasya"].age, 20);
    EXPECT_EQ(f1["Vasya"].weight, 70);
    EXPECT_EQ(f["Ilya"].age, 40);
    EXPECT_EQ(f["Ilya"].weight, 80);
}

TEST(Functions, clear)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    f.insert("Lena", Value(30, 60));
    f.clear();
    try
    {
        EXPECT_NE(f.at("Lena").age, 30);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
    try
    {
        EXPECT_NE(f.at("Vasya").age, 20);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
    EXPECT_EQ(f.size(), 0);
}

TEST(Functions, erase)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    f.insert("Lena", Value(30, 60));
    EXPECT_EQ(f.size(), 2);
    f.erase("Lena");
    EXPECT_EQ(f.size(), 1);
    EXPECT_EQ(f.at("Vasya").age, 20);
    EXPECT_EQ(f.at("Vasya").weight, 70);
    try
    {
        EXPECT_NE(f.at("Lena").age, 30);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
}

TEST(Functions, insert) 

{
    FlatMap f;
    for (std::size_t i = 1; i < 50; i++)
    {
        f.insert("Vasya" + std::to_string(i), Value(10 + i, 50 + i));
    }
    for (std::size_t i = 1; i < 50; i++)
    {
        EXPECT_EQ(f.at("Vasya" + std::to_string(i)).age, 10 + i);
        EXPECT_EQ(f.at("Vasya" + std::to_string(i)).weight, 50 + i);
    }
}

TEST(Functions, contains)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.contains("Vasya"), true);
    EXPECT_EQ(f.contains("Lena"), false);
}

TEST(Functions, at)
{
    FlatMap f;
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.at("Vasya").age, 20);
    try
    {
        EXPECT_NE(f.at("Lena").age, 30);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "Key doesn't exist");
    }
    const FlatMap f1(f);
    EXPECT_EQ(f1.at("Vasya").age, 20);
    try
    {
        EXPECT_NE(f1.at("Lena").age, 30);
    }
    catch (std ::string const &e)
    {
        EXPECT_EQ(e, "const Key doesn't exist");
    }
}

TEST(Functions, size)
{
    FlatMap f;
    EXPECT_EQ(f.size(), 0);
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.size(), 1);
}

TEST(Functions, empty)
{
    FlatMap f;
    EXPECT_EQ(f.empty(), true);
    f.insert("Vasya", Value(20, 70));
    EXPECT_EQ(f.empty(), false);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
