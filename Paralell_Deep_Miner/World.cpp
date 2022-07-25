#include <iostream>
#include <algorithm>
#include "World.h"
#include "Config.h"

World::World()
{
	initWorld();
}

void World::setWorld(std::vector<std::vector<std::vector<int>>> world)
{
	_world = world;
}

std::vector<std::vector<std::vector<int>>>* World::getWorld()
{
	return &_world;
}

void World::initWorld()
{
	//Create a vector3d with set sizes
	std::vector<std::vector<std::vector<int>>> tempWorld (ROWS, std::vector<std::vector <int>> (COLS, std::vector<int> (DEPTH)));

	//Fill each spot with a random number between -3 to 9 (without 0)
	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++) {
		for (int colIndex = 0; colIndex < COLS; colIndex++) {
			for (int depthIndex = 0; depthIndex < DEPTH; depthIndex++) {
				do {
					tempWorld[rowIndex][colIndex][depthIndex] = (rand() % 13) - 3;
				} while (tempWorld[rowIndex][colIndex][depthIndex] == 0);
			}
		}
	}

	setWorld(tempWorld);
}

bool World::worldEmpty()
{
	//If at any point in the map a value is not 0 (so at least 1 value is still there), return false
	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++) {
		for (int colIndex = 0; colIndex < COLS; colIndex++) {
			if (getWorld()->at(rowIndex).at(colIndex).at(0) != 0)
				return false;
		}
	}

	return true;
}

void World::removeValueAt(int x, int y, int z)
{
	//Erases the value at the given position
	getWorld()->at(y).at(x).erase(getWorld()->at(y).at(x).begin() + z);

	//If that erases the last value, then add a 0 so vector is never empty
	if (getWorld()->at(y).at(x).size() == 0)
		getWorld()->at(y).at(x).push_back(0);
}

void World::shuffle()
{
	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++) {
		for (int colIndex = 0; colIndex < COLS; colIndex++) {
			int option = rand() % 3;

			if ((*getWorld())[rowIndex][colIndex].size() > 1) {
				//Do one of three shuffles for each xy-position in the world
				switch (option)
				{
				case 0: //Shuffle randomly
					std::random_shuffle((*getWorld())[rowIndex][colIndex].begin(), (*getWorld())[rowIndex][colIndex].end());
					break;
				case 1: //Sort ascending
					std::sort((*getWorld())[rowIndex][colIndex].begin(), (*getWorld())[rowIndex][colIndex].end());
					break;
				case 2: //Sort descending
					std::sort((*getWorld())[rowIndex][colIndex].rbegin(), (*getWorld())[rowIndex][colIndex].rend());
					break;
				}
			}
		}
	}
}

int World::getSumOfRemainingValues()
{
	int sum = 0;

	for (int rowIndex = 0; rowIndex < ROWS; rowIndex++) {
		for (int colIndex = 0; colIndex < COLS; colIndex++) {
			for (int depthIndex = 0; depthIndex < DEPTH; depthIndex++) {
				if(getWorld()->at(rowIndex).at(colIndex).at(depthIndex) > 0)
					sum += getWorld()->at(rowIndex).at(colIndex).at(depthIndex);

				//Special case because on a -3 field you lose 30 score, meaning it has to be taken into account
				if (getWorld()->at(rowIndex).at(colIndex).at(depthIndex) == -3)
					sum -= 30;
			}
		}
	}

	return sum;
}