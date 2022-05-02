#include "Item.h"
Item::Item(){

	position.x = 1600;
	position.y = 650;
	radius = 20;
	color = BLACK;
	isValid = true;
	pouvoir=0;
}
int Item::getCenterX() {
	return position.x;
}
int Item::getCenterY() {
	return position.y;
}
int Item::getRadius() {
	return radius;
}
Vector2 Item::getPosition() {
	return position;
}
Color Item::getColor() {
	return color;
}
void Item::setColor(Color col) {
	color=col;
}
bool Item::getIsValid() {
	return isValid;
}

void Item::setIsValid(bool show) {
	isValid = show;
}
void Item::setCenterX(int x) {
	position.x = x;
}

int Item::getPouvoir() {
	return pouvoir;
}
void Item::setPouvoir(int x) {
	pouvoir = x;
}
