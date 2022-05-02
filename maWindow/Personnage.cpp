#include "Personnage.h"

Personnage::Personnage(Vector2 pos, int rad) {

    position = pos;
    radius = rad;
    color = DARKGRAY;
    vie = 150;
    vitesse = 3;
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

int Personnage::getVitesse() {
    return vitesse;
}
void Personnage::setVitesse(int v) {
    vitesse = v;
}

int Personnage::getVie() {
    return vie;
}
void Personnage::setVie(int v) {
    vie = v;
}

Color Personnage::getColor() {
    return color;
}
void Personnage::setColor(Color col) {
    color = col;
}

void Personnage::operator+(Item p)
{
    color = p.getColor();
    if (p.getPouvoir() == -2) {
        vitesse = vitesse + 1;
    }
    if (p.getPouvoir() == -3) {
        vie = vie - 1;
    }
    if (p.getPouvoir() == 2) {
        vitesse = vitesse - 1;
    }
    if (p.getPouvoir() == 3) {
        vie = vie + 1;
    }
}


Personnage::~Personnage() {}
