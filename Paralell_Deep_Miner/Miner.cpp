#include "Miner.h"
#include "Config.h"

Miner::Miner(int x, int y, bool playerControlled)
{
	setName("Miner");
	setDescription("Standard Miner");
	setHp(MAX_HP);
	setPosX(x);
	setPosY(y);
	setPlayerControlled(playerControlled);
	setStunCounter(0);
	setTeleportCounter(0);
	setScore(0);
	setTurnCounter(0);
	setShuffleCounter(0);
}

void Miner::setName(std::string name)
{
	_name = name;
}

void Miner::setDescription(std::string description)
{
	_description = description;
}

void Miner::setHp(int hp)
{
	if (hp < 0)
		_hp = 0;
	else if (hp > MAX_HP)
		_hp = MAX_HP;
	else
		_hp = hp;
}

void Miner::setPosX(int x)
{
	if (x < 0)
		_x = 0;
	else if (x >= COLS)
		_x = COLS - 1;
	else
		_x = x;
}

void Miner::setPosY(int y)
{
	if (y < 0)
		_y = 0;
	else if (y >= ROWS)
		_y = ROWS - 1;
	else
		_y = y;
}

void Miner::setPlayerControlled(bool playerControlled)
{
	_playerControlled = playerControlled;
}

void Miner::setStunCounter(int stunCounter)
{
	if (stunCounter < 0)
		_stunCounter = 0;
	else
		_stunCounter = stunCounter;
}

void Miner::setTeleportCounter(int teleportCounter)
{
	if (teleportCounter < 0)
		_teleportCounter = 0;
	else
		_teleportCounter = teleportCounter;
}

void Miner::setScore(int score)
{
	_score = score;
}

void Miner::setTurnCounter(int counter)
{
	_turnCounter = counter;
}

void Miner::setShuffleCounter(int shuffleCounter)
{
	_shuffleCounter = shuffleCounter;
}

void Miner::setElapsedTime(long long elapsedTime)
{
	_elapsedTime = elapsedTime;
}

std::string Miner::getName()
{
	return _name;
}

std::string Miner::getDescription()
{
	return _description;
}

int Miner::getHp()
{
	return _hp;
}

int Miner::getPosX()
{
	return _x;
}

int Miner::getPosY()
{
	return _y;
}

bool Miner::isPlayerControlled()
{
	return _playerControlled;
}

int Miner::getStunCounter()
{
	return _stunCounter;
}

int Miner::getTeleportCounter()
{
	return _teleportCounter;
}

int Miner::getScore()
{
	return _score;
}

int Miner::getTurnCounter()
{
	return _turnCounter;
}

int Miner::getShuffleCounter()
{
	return _shuffleCounter;
}

long long Miner::getElapsedTime()
{
	return _elapsedTime;
}

bool Miner::isAlive()
{
	return (getHp() > 0);
}

bool Miner::moveUp()
{
	int oldY = getPosY();
	setPosY(getPosY() - 1);

	if (getPosY() == oldY) return false;
	return true;
}

bool Miner::moveDown()
{
	int oldY = getPosY();
	setPosY(getPosY() + 1);

	if (getPosY() == oldY) return false;
	return true;
}

bool Miner::moveLeft()
{
	int oldX = getPosX();
	setPosX(getPosX() - 1);

	if (getPosX() == oldX) return false;
	return true;
}

bool Miner::moveRight()
{
	int oldX = getPosX();
	setPosX(getPosX() + 1);

	if (getPosX() == oldX) return false;
	return true;
}

void Miner::fieldEffect(World* gameWorld)
{
	switch (gameWorld->getWorld()->at(getPosY()).at(getPosX()).at(0))
	{
	case -3: //Loose 30 score
		setScore(getScore() - 30);
		break;
	case -2: //Teleport bot to first lowest field
		setTeleportCounter(getTeleportCounter() + 1);
		break;
	case -1: //Stun the bot for one round (meaning he cannot drill)
		setStunCounter(getStunCounter() + 2);
		break;
	default: //On all other fields increase score
		setScore(getScore() + gameWorld->getWorld()->at(getPosY()).at(getPosX()).at(0));
		break;
	}

	//Remove field
	gameWorld->removeValueAt(getPosX(), getPosY(), 0);
}

void Miner::start(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex)
{
	//Saves the start time
	auto startTime = std::chrono::steady_clock::now();

	//Move and do Actions as long as World isn't empty and miner is alive
	while (isAlive()) {

		worldMutex->lock();
			if (gameWorld->worldEmpty()) {
				worldMutex->unlock();
				break; 
			}
		worldMutex->unlock();

		//Choose a random direction (5th direction is when the robot stands still)
		int randomDir = (rand() % 5) + 1;

		//Lock movement mutex
		movementMutex->lock();

			//Walk in specified direction if possible
			if (randomDir == 1) { //Up
				if (!moveUp()) {
					movementMutex->unlock();
					continue;
				}
			}
			else if (randomDir == 2) { //Down
				if (!moveDown()) {
					movementMutex->unlock();
					continue;
				}
			}
			else if (randomDir == 3) { //Left
				if (!moveLeft()) {
					movementMutex->unlock();
					continue;
				}
			}
			else if (randomDir == 4) { //Right
				if (!moveRight()) {
					movementMutex->unlock();
					continue;
				}
			}

		//Unlock movement mutex
		movementMutex->unlock();

		//Do round actions
		doRoundActions(gameWorld, worldMutex, allMiners, movementMutex);

		//TurnCounter++
		setTurnCounter(getTurnCounter() + 1);
	}

	//Saves the end time
	auto endTime = std::chrono::steady_clock::now();
	//Saves the elapsed time
	setElapsedTime(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());
}

void Miner::doRoundActions(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex)
{
	//Lock for checking miner positions
	movementMutex->lock();
		int numOfMinersAtCurrPos = minersAtCurrentPosition(allMiners);
	movementMutex->unlock();

	//If more than one miner is at current position (always at least 1 bc it finds itself) take damage
	if (numOfMinersAtCurrPos > 1)
		setHp(getHp() - (numOfMinersAtCurrPos - 1));

	//Lock for world manipulation
	worldMutex->lock();
		//Drill
		drill(gameWorld);

		//If score reaced another number divisible by 50
		if (getShuffleCounter() < getScore() / 50) {
			//Shuffle the world
			gameWorld->shuffle();
			//Increase the shuffle counter for that miner
			setShuffleCounter(getShuffleCounter() + 1);
		}
	//Unlock world
	worldMutex->unlock();

	//Decrease stun counter
	setStunCounter(getStunCounter() - 1);

	//If miner is getting teleported, find first lowest position and teleport him there
	if (getTeleportCounter() != 0) {
		int lowest = 10;
		int lowestPosX = 0;
		int lowestPosY = 0;

		//Lock world for finding lowest field
		worldMutex->lock();

			for (int rowIndex = 0; rowIndex < ROWS; rowIndex++) {
				for (int colIndex = 0; colIndex < COLS; colIndex++) {
					//If the value at the given xy-position is greater than 0 and lower than the current lowest, save position & value
					if (gameWorld->getWorld()->at(rowIndex).at(colIndex).at(0) > 0 && gameWorld->getWorld()->at(rowIndex).at(colIndex).at(0) < lowest) {
						lowest = gameWorld->getWorld()->at(rowIndex).at(colIndex).at(0);
						lowestPosX = colIndex;
						lowestPosY = rowIndex;
					}
				}
			}

		//Unlock world
		worldMutex->unlock();

		//Lock movement to teleport
		movementMutex->lock();

			//Set miner to new position
			setPosX(lowestPosX);
			setPosY(lowestPosY);

		//Unlock movement
		movementMutex->unlock();

		//Decrease teleport counter
		setTeleportCounter(getTeleportCounter() - 1);
	}
}

std::thread* Miner::createThread(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex)
{
	//The problem is that if I give gameWorld like this std::ref(gameWorld), it creates a reference to the local pointer variable
	//Actually I want a reference to the world-object behind that variable though, so my idea: std::ref(*gameWorld)
	//This does not compile for some reason though, so idk what else to do (same goes for the mutex pointer)
	//The problem might even be somewhere else in that std::thread call, since I don't understand why this (&Miner::start, this, ...) seems to work
	return new std::thread(&Miner::start, this, gameWorld, worldMutex, allMiners, movementMutex);
}

int Miner::minersAtCurrentPosition(std::vector<Miner*> allMiners)
{
	int minersAtPosition = 0;

	for (int i = 0; i < allMiners.size(); i++) {
		if (allMiners.at(i)->isAlive() && allMiners.at(i)->getPosX() == getPosX() && allMiners.at(i)->getPosY() == getPosY())
			minersAtPosition++;
	}

	return minersAtPosition;
}
