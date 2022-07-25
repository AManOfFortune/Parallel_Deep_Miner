#pragma once

#include "Sorter.h"
#include "Deeper.h"
#include "Bomber.h"

class TitleScreen
{
public:
	//Constructor & Destructor
	TitleScreen();
	~TitleScreen();

	//Getter & Setter
	void setTitleOptions(int numOfOptions);
	void setTitleChoice(int choice);
	void setAllCharacters(std::vector<Miner*> allCharacters);
	int getTitleOptions();
	int getTitleChoice();
	std::vector<Miner*> getAllCharacters();

	//Functions
	void show();
	void initAllCharacters();
	Miner* chooseCharacter();
	int chooseNumberOfMiners();

private:
	int _titleOptions;
	int _titleChoice;

	std::vector<Miner*> _allCharacters;
};