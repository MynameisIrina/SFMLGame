#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameManager.h"

int main()
{
    GameManager gameManager;
    gameManager.InitializeWindow();
    gameManager.InitializeGame();
    gameManager.Run();
    return 0;
}