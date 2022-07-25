#include <iostream>
#include <Windows.h>
#include "TitleScreen.h"
#include "Config.h"

TitleScreen::TitleScreen()
{
    setTitleOptions(2);
    setTitleChoice(0);
    setAllCharacters(std::vector<Miner*>());
    initAllCharacters();
}

TitleScreen::~TitleScreen()
{
    while (_allCharacters.size() > 0) {
        _allCharacters.erase(_allCharacters.begin());
    }
}

void TitleScreen::setTitleOptions(int numOfOptions)
{
    _titleOptions = numOfOptions;
}

void TitleScreen::setTitleChoice(int choice)
{
    if (choice < 1)
        _titleChoice = 1;
    else if (choice > getTitleOptions())
        _titleChoice = getTitleOptions();
    else
        _titleChoice = choice;
}

void TitleScreen::setAllCharacters(std::vector<Miner*> allCharacters)
{
    _allCharacters = allCharacters;
}

int TitleScreen::getTitleOptions()
{
    return _titleOptions;
}

int TitleScreen::getTitleChoice()
{
    return _titleChoice;
}

std::vector<Miner*> TitleScreen::getAllCharacters()
{
    return _allCharacters;
}

void TitleScreen::show()
{
    setTitleChoice(1);
    
    //Necessary to remove any enter input before the title is displayed
    while (GetAsyncKeyState(VK_RETURN)) {};

    bool someThingChanged = true;
    while (true) {
        if (someThingChanged) {
            someThingChanged = false;
            system("cls");

            std::cout << "  _____                    __  __ _                 " << std::endl;
            std::cout << " |  __ \\                  |  \\/  (_)                " << std::endl;
            std::cout << " | |  | | ___  ___ _ __   | \\  / |_ _ __   ___ _ __ " << std::endl;
            std::cout << " | |  | |/ _ \\/ _ \\ '_ \\  | |\\/| | | '_ \\ / _ \\ '__|" << std::endl;
            std::cout << " | |__| |  __/  __/ |_) | | |  | | | | | |  __/ |   " << std::endl;
            std::cout << " |_____/ \\___|\\___| .__/  |_|  |_|_|_| |_|\\___|_|   " << std::endl;
            std::cout << "                  | |                               " << std::endl;
            std::cout << "                  |_|                               " << std::endl;

            if (getTitleChoice() == 1) {
                std::cout << "\033[;93m" << "\n\n  Play" << "\033[m" << std::endl;
                std::cout << "  Quit" << std::endl;
            }
            else if (getTitleChoice() == 2) {
                std::cout << "\n\n  Play" << std::endl;
                std::cout << "\033[;93m" << "  Quit" << "\033[m" << std::endl;
            }
        }

        if (GetAsyncKeyState(VK_UP)) {
            setTitleChoice(getTitleChoice() - 1);
            someThingChanged = true;
        }
        else if (GetAsyncKeyState(VK_DOWN)) {
            setTitleChoice(getTitleChoice() + 1);
            someThingChanged = true;
        }
        else if (GetAsyncKeyState(VK_RETURN))
            break;
    }
}

void TitleScreen::initAllCharacters()
{
    _allCharacters.push_back(new Sorter(0, 0, false));
    _allCharacters.push_back(new Deeper(0, 0, false));
    _allCharacters.push_back(new Bomber(0, 0, false));
}

Miner* TitleScreen::chooseCharacter()
{
    //A random character is chosen
    int randomChar = rand() % getAllCharacters().size();

    //Character is not player controlled
    getAllCharacters().at(randomChar)->setPlayerControlled(false);

    //Randomize starting position
    getAllCharacters().at(randomChar)->setPosX(rand() % COLS);
    getAllCharacters().at(randomChar)->setPosY(rand() % ROWS);

    //Return a clone of the chosen character
    return getAllCharacters().at(randomChar)->clone();
}

int TitleScreen::chooseNumberOfMiners()
{
    int choice = 0;

    do {
        system("cls");
        std::cout << "How many miners do you want? (Max " << MAX_NUM_OF_MINERS << ")" << std::endl;
        std::cin >> choice;
    } while (choice <= 0 || choice > MAX_NUM_OF_MINERS);

    return choice;
}
