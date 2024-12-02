#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Background.h"
#include "Perlin.h"
#include "Level.h"


int main()
{   
    // ----------------- INITIALIZE -----------------
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML game", sf::Style::Default, settings);
    sf::Uint8 *pixels = new sf::Uint8[800 * 600 * 4];
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Player player;
    Level level(player);
    const std::vector<sf::Sprite>& mapGround = level.InitializeGround();
    const std::vector<sf::Sprite>& mapUp = level.InitializeUp();
    sf::Clock timer;
    float deltaTime = 0.0f;
    Background background(player);
    background.Initialize(window);
    player.Initialize(sf::Vector2f(50.f, 502.f));


    // ----------------- INITIALIZE -----------------

    while (window.isOpen())
    {
        //  ----------------- UPDATE -----------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        deltaTime = timer.restart().asSeconds();

        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);

        background.Move(deltaTime);
        background.UpdateView();

        player.Move(moveRight, moveLeft);
        player.Update(deltaTime);
        player.UpdateView(moveRight, moveLeft);

        level.Update();

        
        // ----------------- UPDATE----------------


        // ----------------- DRAW -----------------
        window.clear();
        background.Draw(window);
        player.Draw(window);
        for (const sf::Sprite &block: mapGround)
        {
            window.draw(block);
        }
        for (const sf::Sprite &block: mapUp)
        {
            window.draw(block);
        }
        window.display();
        // ----------------- DRAW -----------------
    }

    return 0;
}