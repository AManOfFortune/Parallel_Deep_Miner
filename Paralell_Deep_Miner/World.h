#pragma once
#include <vector>

class World
{
public:
	//Constructor
	World();

	//Getter & Setter
	void setWorld(std::vector<std::vector<std::vector<int>>> world);
	std::vector<std::vector<std::vector<int>>>* getWorld();

	//Functions
	void initWorld();
	bool worldEmpty();
	void removeValueAt(int x, int y, int z);
	void shuffle();
	int getSumOfRemainingValues();

private:
	//Variables
	std::vector<std::vector<std::vector<int>>> _world;
};