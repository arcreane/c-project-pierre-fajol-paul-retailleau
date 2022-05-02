#pragma once
#include "Item.h"

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

    /*void pouvoir(Personnage p)override {
        p.setVitesse(p.getVitesse() + 150);
        p.setColor(RED);
        std::cout << "Je suis le malus" << std::endl;
    }*/

};

