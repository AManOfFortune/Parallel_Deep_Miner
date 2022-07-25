#pragma once
#include "Miner.h"
class Deeper :
    public Miner
{
public:
    Deeper(int x, int y, bool playerControlled);

    void drill(World* gameWorld);
    Miner* clone();
};

