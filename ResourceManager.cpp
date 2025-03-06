#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void ResourceManager::LoadConfigCoin(const std::string &file, Coin &coin)
{
    std::ifstream configFile(file);

    if (!configFile.is_open())
    {
        std::cout << "Failed to open file: " << file << std::endl;
        return;
    }

    json config;
    configFile >> config;

    sf::Vector2f position(
        config["coin"]["position"]["x"],
        config["coin"]["position"]["y"]);

    int frameSize = config["coin"]["frameSize"];
    float animationInterval = config["coin"]["animationInterval"];
    int maxFrames = config["coin"]["maxFrames"];
    int coinOffsetX = config["coin"]["coinOffsetX"];

    //coin.Initialize(position, frameSize, animationInterval, maxFrames, coinOffsetX);

}
