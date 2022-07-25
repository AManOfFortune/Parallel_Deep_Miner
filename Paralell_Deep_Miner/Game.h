#pragma once
#include "TitleScreen.h"
#include "World.h"

class Game
{
public:
	//Constructor & Destructor
	Game();
	~Game();

	//Functions
	void start();

private:
	World* _gameWorld;
	std::vector<Miner*> _miners;
	std::vector<std::thread*> _threads;
	std::mutex* _worldMutex;
	std::mutex* _movementMutex;

	//Getter & Setter
	void setWorld(World* newWorld);
	void setMiners(std::vector<Miner*> miners);
	void setThreads(std::vector<std::thread*> threads);
	void setWorldMutex(std::mutex* worldMutex);
	void setMovementMutex(std::mutex* movementMutex);
	World* getWorld();
	std::vector<Miner*> getMiners();
	std::vector<std::thread*>* getThreads();
	std::mutex* getWorldMutex();
	std::mutex* getMovementMutex();

	//Functions
	void addMiner(Miner* miner);
	void addThread(std::thread* addedThread);
	void printFinalInformation(int initialSum, long long elapsedTime);
};