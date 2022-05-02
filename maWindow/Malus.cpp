#include "Malus.h"
Malus::Malus(std::string n) :Item() {
	name = n;
	//pouvoir = -1;
	//color = RED;
	Item::setPouvoir(-1);
	Item::setColor(RED);
}
std::string Malus::getName() {
	return name;
}
void Malus::SetName(std::string n) {
	name = n;
}
/*void Malus::operator+(Personnage p) {
	p.setVitesse(p.getVitesse() + 150);
	p.setColor(RED);
	std::cout << "Je suis le malus" << std::endl;
};*/