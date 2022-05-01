#pragma once
#include <string>

class Obstacle :
    public Rectangle
{
	private:
		std::string image;
		Color color;

	public:
		//Obstacle(float posX, float posY, float height, float width);
		//void setImage();
};


