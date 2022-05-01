
#include "Bonus.h"
Bonus::Bonus(std::string n):Item() {
	name = n;
	//color = GREEN;
	Item::setColor(GREEN);
}
std::string Bonus::getName() {
	return name;
}
void Bonus::SetName(std::string n) {
	name = n;
}
