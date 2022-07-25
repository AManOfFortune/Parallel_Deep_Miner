#pragma once
#include "Miner.h"
class Sorter :
    public Miner
{
public:
    Sorter(int x, int y, bool playerControlled);
    
    void drill (World* gameWorld);
    Miner* clone();
};