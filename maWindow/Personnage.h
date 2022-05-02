#pragma once
#include "raylib.h"
//#include "item.h"
#include "Malus.h"
#include "Bonus.h"

class Personnage {
private:
    Vector2 position;
    int radius;
    Color color;
    int vie;
    int vitesse;

public:
    Personnage(Vector2 pos, int rad);
    ~Personnage();

    Vector2 getPosition();

    float getXposition();
    void setXposition(float xpos);

    int getVitesse();
    void setVitesse(int v);

    int getVie();
    void setVie(int v);

    float getYposition();
    void setYposition(float ypos);

    int getRadius();
    void setRadius(int rad);

    Color getColor();
    void setColor(Color col);


    void operator+(Item p);
};