#include "Bomber.h"

Bomber::Bomber(int x, int y, bool playerControlled) : Miner(x, y, playerControlled)
{
	setName("Bomber");
	setDescription("Mines all adjacent fields");
}

void Bomber::drill(World* gameWorld)
{
	//If we are not stunned, check if movement is possible, if so do the field effect, then move back to original position
	if (getStunCounter() == 0) {
		//Field above bot
		if (moveUp()) {
			fieldEffect(gameWorld);
			moveDown();
		}

		//Field below bot
		if (moveDown()) {
			fieldEffect(gameWorld);
			moveUp();
		}

		//Field left of bot
		if (moveLeft()) {
			fieldEffect(gameWorld);
			moveRight();
		}

		//Field right of bot
		if (moveRight()) {
			fieldEffect(gameWorld);
			moveLeft();
		}
	}
}

Miner* Bomber::clone()
{
	return new Bomber(*this);
}