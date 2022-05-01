#pragma once
#include "raylib.h"
class Personnage {
private:
    Vector2 position;
    int radius;
    Color color;

public:
    Personnage(Vector2 pos, int rad);
    ~Personnage();

    Vector2 getPosition();

    float getXposition();
    void setXposition(float xpos);

    float getYposition();
    void setYposition(float ypos);

    int getRadius();
    void setRadius(int rad);

    Color getColor();
    void setColor(Color col);


};