# sf::Cat

## Overview

**sf::Cat** is a 2D platformer developed in C++ using SFML. The game follows the journey of a stray cat trying to survive in a challenging world. Players navigate procedurally generated levels, avoid dangers such as dogs and traps, and collect food to sustain the cat. With simple yet engaging mechanics, the game highlights the struggles and resilience of a small cat in a big, unfriendly world.

## Features

Procedural Level Generation: Levels are dynamically generated to ensure a unique experience each playthrough.

Player Movement Mechanics: Implemented smooth running and jumping mechanics.

Obstacle and Enemy Placement: Randomized enemy and obstacle distribution for increased challenge.

Collision Detection: Robust system handling player-environment and player-enemy interactions.

Respawn System: Restart the player at a specific location after death.

User Interface Elements: Health bars, score displays, and menus for better user interaction.

Dynamic Audio: Integrated background music, sound effects, and character actions.

## How to Run the Game
Prerequisites: C++ Compiler and SFML 2.6.1. Follow the SFML installation guide to set it up on your system. [Link to SFML Tutorials](https://www.sfml-dev.org/tutorials/2.6/)


**Steps to Build and Run**

1. Clone the repository:
   
Download or clone the repository to your local machine using Git: 
```bash
git clone https://github.com/MynameisIrina/SFMLGame.git
```
2. Install SFML Dependencies
   
Ensure that SFML is installed on your system. For Linux, use the following command:
```bash
sudo apt-get install libsfml-dev
```

For macOS, you can install SFML using Homebrew:
```bash
brew install sfml
```

On Windows, follow the SFML installation steps provided in the official SFML documentation.

3. Build the Project

  3.1 Compile the Project
  
  Use Clang++ to compile the project. Navigate to your project directory, then run the following commands in your terminal:
  ```bash
  clang++ -c main.cpp HealthBar.cpp Player.cpp Background.cpp Camera.cpp Level_TileBased.cpp TextureLoader.cpp Math.cpp Obstacle.cpp -I<sfml-install-path>/include -std=c++17
  ```
  
  This will compile the source files into object files (main.o, Player.o, etc.).

  3.2 Link the Object Files and Create the Executable
  
  After compiling the object files, link them together into a single executable file:
  ```bash
  clang++ main.o Player.o Background.o Camera.o Level_TileBased.o TextureLoader.o Math.o Obstacle.o HealthBar.o -o sfml-app -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system
  ```
  
  This will generate the executable sfml-app

4. Run the Game

Finally, run the game using the following command:
```bash
./sfml-app
```

## Troubleshooting

Compilation Errors: If you encounter errors during compilation, make sure that SFML is installed correctly and the paths for the SFML include and lib directories are correct.







