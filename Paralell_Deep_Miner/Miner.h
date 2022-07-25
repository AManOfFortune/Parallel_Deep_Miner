#pragma once
#include "World.h"
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

class Miner
{
public:
	//Constructor
	Miner(int x, int y, bool playerControlled);

	//Getters & Setters
	void setName(std::string name);
	void setDescription(std::string description);
	void setHp(int hp);
	void setPosX(int x);
	void setPosY(int y);
	void setPlayerControlled(bool playerControlled);
	void setStunCounter(int stunCounter);
	void setTeleportCounter(int teleportCounter);
	void setScore(int score);
	void setTurnCounter(int counter);
	void setShuffleCounter(int shuffleCounter);
	void setElapsedTime(long long elapsedTime);
	std::string getName();
	std::string getDescription();
	int getHp();
	int getPosX();
	int getPosY();
	bool isPlayerControlled();
	int getStunCounter();
	int getTeleportCounter();
	int getScore();
	int getTurnCounter();
	int getShuffleCounter();
	long long getElapsedTime();

	//Functions
	bool isAlive();
	bool moveUp();
	bool moveDown();
	bool moveLeft();
	bool moveRight();
	void fieldEffect(World* gameWorld);
	void start(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex);
	void doRoundActions(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex);
	std::thread* createThread(World* gameWorld, std::mutex* worldMutex, std::vector<Miner*> allMiners, std::mutex* movementMutex);
	int minersAtCurrentPosition(std::vector<Miner*> allMiners);
	virtual Miner* clone() = 0;
	virtual void drill(World* gameWorld) = 0;

private:
	//Variables
	std::string _name;
	std::string _description;

	int _hp;

	int _x;
	int _y;
	int _score;
	int _turnCounter;

	bool _playerControlled;
	int _shuffleCounter;

	int _stunCounter;
	int _teleportCounter;

	long long _elapsedTime;
};