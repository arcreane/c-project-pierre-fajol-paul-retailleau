#include "BaisseVie.h"
BaisseVie::BaisseVie() : Malus("Baisse des points de vie") {

	Item::setPouvoir(-3);
	Item::setColor(DARKBLUE);
}
