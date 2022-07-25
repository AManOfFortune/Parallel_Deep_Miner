#include "Game.h"
#include <Windows.h>
#include <iostream>
#include "Config.h"

Game::Game()
{
	setMiners(std::vector<Miner*>());
	setThreads(std::vector<std::thread*>());
	setWorldMutex(new std::mutex());
	setMovementMutex(new std::mutex());
}

Game::~Game()
{
	delete getMovementMutex();
	delete getWorldMutex();
}

void Game::setWorld(World* newWorld)
{
	_gameWorld = newWorld;
}

void Game::setMiners(std::vector<Miner*> miners)
{
	_miners = miners;
}

void Game::setThreads(std::vector<std::thread*> threads)
{
	_threads = threads;
}

void Game::setWorldMutex(std::mutex* worldMutex)
{
	_worldMutex = worldMutex;
}

void Game::setMovementMutex(std::mutex* movementMutex)
{
	_movementMutex = movementMutex;
}

World* Game::getWorld()
{
	return _gameWorld;
}

//One thing to note on this function:
//It returns a COPY of the miners vector, so if we want to do anything to the vector itself and not the miners,
//we need to refer to the actual variable _miners and cannot use getMiners()
std::vector<Miner*> Game::getMiners()
{
	return _miners;
}

std::vector<std::thread*>* Game::getThreads()
{
	return &_threads;
}

std::mutex* Game::getMovementMutex()
{
	return _movementMutex;
}

std::mutex* Game::getWorldMutex()
{
	return _worldMutex;
}

void Game::start()
{
	TitleScreen gameTitleScreen;

	while (gameTitleScreen.getTitleChoice() != gameTitleScreen.getTitleOptions()) {

		gameTitleScreen.show();

		//If player chooses play
		if (gameTitleScreen.getTitleChoice() == 1) {

			//Initialize game variables
			setWorld(new World());

			int numOfMiners = gameTitleScreen.chooseNumberOfMiners();

			//Add x miners
			for (int i = 0; i < numOfMiners; i++) {
				addMiner(gameTitleScreen.chooseCharacter());
			}

			//Saves initial sum of world values
			int initialSum = getWorld()->getSumOfRemainingValues();

			//Saves the start time
			auto startTime = std::chrono::steady_clock::now();

			//Then create a thread for each miner
			for (int i = 0; i < numOfMiners; i++) {
				addThread(getMiners().at(i)->createThread(getWorld(), getWorldMutex(), getMiners(), getMovementMutex()));
			}

			//Join all threads
			for (int i = 0; i < numOfMiners; i++) {
				getThreads()->at(i)->join();
			}

			//Saves the end time
			auto endTime = std::chrono::steady_clock::now();

			//Print the final stats
			printFinalInformation(initialSum, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

			//Free the world object
			delete getWorld();

			//Free the thread objects
			//We do not need to delete them because vectors to that automatically to all objects stored in them
			//As long as the miner vector is not size 0
			while (_threads.size() > 0) {
				//Erase the first position of the vector
				_threads.erase(_threads.begin());
			}

			//Free the miner objects
			//We do not need to delete them because vectors to that automatically to all objects stored in them
			//As long as the miner vector is not size 0
			while (_miners.size() > 0) {
				//Erase the first position of the vector
				_miners.erase(_miners.begin());
			}
		}
	}
}

void Game::addMiner(Miner* miner)
{
	_miners.push_back(miner);
}

void Game::addThread(std::thread* addedThread)
{
	getThreads()->push_back(addedThread);
}

void Game::printFinalInformation(int initialSum, long long elapsedTime)
{
	system("cls");

	int winningMinerIndex = 0;
	int winningMinerScore = 0;
	int totalScore = 0;

	for (int i = 0; i < getMiners().size(); i++) {
		//Print miners with final Score and Elapsed Time
		std::cout << "\n" << getMiners().at(i)->getName() << " " << i + 1 << " (" << getMiners().at(i)->getTurnCounter() << " Turns): " << std::endl;

		std::cout << "Score [" << getMiners()[i]->getScore() << "]" << std::endl;
		std::cout << "Hp    [" << getMiners()[i]->getHp() << "]" << std::endl;
		std::cout << "Time  [" << getMiners().at(i)->getElapsedTime() << " ms]" << std::endl;

		//Add score to total sum
		totalScore += getMiners().at(i)->getScore();

		//Save the winning miners score and index
		if (getMiners().at(i)->getScore() > winningMinerScore) {
			winningMinerScore = getMiners().at(i)->getScore();
			winningMinerIndex = i;
		}
	}

	std::cout << "\n" << getMiners().at(winningMinerIndex)->getName() << " " << winningMinerIndex + 1 << " (" << getMiners().at(winningMinerIndex)->getTurnCounter() << " Turns) won with a score of " << winningMinerScore << "!\n" << std::endl;

	std::cout << "\nInitial World total: " << initialSum << std::endl;
	std::cout << "All miner score sum: " << totalScore << std::endl;
	std::cout << "Difference of:       " << initialSum - totalScore << "\n" << std::endl;

	std::cout << "Total elapsed time: " << elapsedTime << " ms\n" << std::endl;

	system("pause");
}