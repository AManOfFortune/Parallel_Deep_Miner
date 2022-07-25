#include "Deeper.h"

Deeper::Deeper(int x, int y, bool playerControlled) : Miner(x, y, playerControlled)
{
	setName("Deeper");
	setDescription("Mines 3 fields deep on his current position");
}

void Deeper::drill(World* gameWorld)
{
	//Does field effect 3 times as long as there still is a field to dig & we are not stunned
	for (int i = 0; (i < gameWorld->getWorld()->at(getPosY()).at(getPosX()).size() || i < 3) && getStunCounter() == 0; i++) {
		fieldEffect(gameWorld);
	}
}

Miner* Deeper::clone()
{
	return new Deeper(*this);
}
