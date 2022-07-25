#pragma once
#include "Miner.h"
class Bomber :
    public Miner
{
public:
    Bomber(int x, int y, bool playerControlled);

    void drill(World* gameWorld);
    Miner* clone();
};