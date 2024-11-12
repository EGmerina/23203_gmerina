#pragma once
#include <vector>

struct Card
{
    unsigned char num;
};

struct Hand
{
    std::vector<Card> hand; //лицевая карта под индексом 0, либо добавить поле, либо что лучше написать функцию которая дает эту карту
    unsigned char points;
};
