#pragma once
#include "Coin.h"

class ResourceManager
{

public:

	ResourceManager();
    void LoadConfigCoin(const std::string& file, Coin& coin);

};