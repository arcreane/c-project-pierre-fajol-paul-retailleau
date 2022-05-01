#pragma once
#include "raylib.h"
class Item
{
private:
	Vector2 position;
	bool isValid;
	int radius;
	Color color;
public :
	Item();
	int getCenterX();
	int getCenterY();
	int getRadius();
	Vector2 getPosition();
	Color getColor();
	void setCenterX(int x);
	void setColor(Color col);
	bool getIsValid();
	void setIsValid(bool show);
};
