#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Background.h"

int main()
{
    // ----------------- INITIALIZE -----------------  
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML game", sf::Style::Default, settings);

    sf::Clock timer;
    float deltaTime = 0.0f;
    Player player;
    Background background(player);
    background.Initialize(window);
    player.Initialize();

    // ----------------- INITIALIZE -----------------   

    // ----------------- LOAD ----------------- 

    player.Load(sf::Vector2f(50.f, 520.f));

    // ----------------- LOAD -----------------   


    while (window.isOpen())
    {
        // ----------------- UPDATE -----------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        deltaTime = timer.restart().asSeconds();

        background.Update(deltaTime);
        player.Move(deltaTime);




        // ----------------- UPDATE -----------------



        // ----------------- DRAW -----------------
        window.clear();
        background.Draw(window);
        player.Draw(window);
        window.display();
        // ----------------- DRAW -----------------

    }

    return 0;
}