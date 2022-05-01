#pragma once
#include "raylib.h"
#include "item.h"
#include <string>
class Bonus :
    public Item
{
    private :
        std::string name;
        Color color;
    public :
        Bonus(std::string n);
        std::string getName();
        void SetName(std::string n);
};
