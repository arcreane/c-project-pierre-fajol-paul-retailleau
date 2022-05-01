#include "Personnage.h"
Personnage::Personnage(Vector2 pos, int rad) {

    position = pos;
    radius = rad;
    color = DARKGRAY;
}

Vector2 Personnage::getPosition() {
    return position;
}


float Personnage::getXposition() {
    return position.x;
}
void Personnage::setXposition(float xpos) {
    position.x = xpos;
}


float Personnage::getYposition() {
    return position.y;
}
void Personnage::setYposition(float ypos) {
    position.y = ypos;
}


int Personnage::getRadius() {
    return radius;
}
void Personnage::setRadius(int rad) {
    radius = rad;
}


Color Personnage::getColor() {
    return color;
}
void Personnage::setColor(Color col) {
    color = col;
}


Personnage::~Personnage() {}
