#pragma once
#include "raylib.h"
#include <iostream>
//#include "Personnage.h"
class Personnage;
class Item
{
private:
	Vector2 position;
	bool isValid;
	int radius;
	Color color;
	int pouvoir;
public :
	Item();
	int getCenterX();
	int getCenterY();
	int getRadius();
	Vector2 getPosition();
	Color getColor();
	void setCenterX(int x);
	void setColor(Color col);
	int getPouvoir();
	void setPouvoir(int x);
	bool getIsValid();
	void setIsValid(bool show);
	//void pouvoir(Personnage p){ std::cout << "JE SUIS UN HERO ET J'ATTAQUE" << std::endl; };
};
