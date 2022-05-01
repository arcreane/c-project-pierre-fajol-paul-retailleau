#include "Malus.h"
Malus::Malus(std::string n) :Item() {
	name = n;
	//color = RED;
	Item::setColor(RED);
}
std::string Malus::getName() {
	return name;
}
void Malus::SetName(std::string n) {
	name = n;
}