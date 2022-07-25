#include "Sorter.h"

Sorter::Sorter(int x, int y, bool playerControlled) : Miner(x, y, playerControlled)
{
	setName("Sorter");
	setDescription("Sorts the field he is on from highest to lowest and mines 1 field deep");
}

void Sorter::drill(World* gameWorld)
{
	//Sorts current position
	std::sort(gameWorld->getWorld()->at(getPosY()).at(getPosX()).rbegin(), gameWorld->getWorld()->at(getPosY()).at(getPosX()).rend());

	//Does the field effect
	fieldEffect(gameWorld);
}

Miner* Sorter::clone()
{
	return new Sorter(*this);
}
