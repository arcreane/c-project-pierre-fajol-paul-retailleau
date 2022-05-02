#pragma once
#include "raylib.h"
#include "Item.h"
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

        /*void pouvoir(Personnage p)override {
            p.setVitesse(p.getVitesse() + 150);
            p.setColor(GREEN);
            std::cout << "Je suis le malus" << std::endl;
        }*/

};
