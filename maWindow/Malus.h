#pragma once
#include "item.h"
#include <string>
class Malus :
    public Item
{
private:
	std::string name;
    //Color color;
public:
    Malus(std::string n);
    std::string getName();
    void SetName(std::string n);
};

