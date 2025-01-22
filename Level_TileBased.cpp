#include <iostream>
#include "Level_TileBased.h"

Level_TileBased::Level_TileBased(const std::shared_ptr<Camera> cam, std::shared_ptr<TextureLoader> txLoaderRef)
    : camera(cam),
      txLoader(txLoaderRef),
      obstacleManager(txLoaderRef),
      enemyManager(txLoaderRef)
{
}

void Level_TileBased::Initialize()
{
    grid.resize(GRID_HEIGHT, std::vector<Tile>(TOTAL_GRID_WIDTH, Tile(Tile::Tile_Type::Empty, sf::RectangleShape())));
    patterns = {defaultPattern, pattern1, pattern2, pattern3, pattern4, pattern5, pattern15, pattern16};

    int initialWidthPattern = GenerateDefaultTiles();
    GenerateLevel(initialWidthPattern);
    ShowGrid();
}

int Level_TileBased::GenerateDefaultTiles()
{
    int currY = 2;
    int patternWidth = defaultPattern[0].size();
    int patternHeight = defaultPattern.size();

    // generate platforms on invisible level area (or visible + invisible for initial method call)
    for (int x = 0; x < patternWidth; x += patternWidth)
    {
        int newX = x + patternWidth;
        bool inBounds = newX < GRID_WIDTH + BUFFER_COLUMNS;
        if (!inBounds)
        {
            break;
        }

        PlacePattern(0, x, currY);
    }

    CheckGround(0, 0);

    return patternWidth;
}

void Level_TileBased::GenerateLevel(int startX)
{
    int currY = prevYFromLevelGen;
    int prevY = prevYFromLevelGen;
    int patternWidth = 0;
    int patternHeight = 0;

    int startingPositionX = lastX_atTotalGridWidthPos + TILE_SIZE;

    // generate platforms on invisible level area (or visible + invisible for initial method call)
    for (int x = startX; x < GRID_WIDTH + BUFFER_COLUMNS; x += patternWidth)
    {
        // Randomly choose the pattern
        int patternIndex = rand() % patterns.size();

        // Get the dimensions of the chosen pattern
        patternWidth = patterns[patternIndex][0].size();
        patternHeight = patterns[patternIndex].size();

        // if there is no enough space for a whole pattern -> cut the pattern
        if (x + patternWidth > GRID_WIDTH + BUFFER_COLUMNS)
        {
            int diff = GRID_WIDTH + BUFFER_COLUMNS - x;
            patternWidth = diff;
        }

        // Ensure that the pattern does not exceed the grid height and is above certain level
        prevY = currY;
        currY = rand() % GRID_HEIGHT;

        // ensute that the patern is not too high from the player
        bool newYTooHigh = currY < prevY - 2;
        if (newYTooHigh)
        {
            currY = prevY - 1;
        }

        currY = std::clamp(currY, static_cast<int>(LevelBounds::minY), GRID_HEIGHT - patternHeight);
        PlacePattern(patternIndex, x, currY);
    }

    prevYFromLevelGen = currY;

    CheckGround(startX, startingPositionX);
    obstacleManager.SpawnObstacles(grid, GRID_HEIGHT, GRID_WIDTH, TOTAL_GRID_WIDTH, lastX_atGridWidthPos, TILE_SIZE);
    //enemyManager.SpawnEnemies(grid, GRID_HEIGHT, GRID_WIDTH, TOTAL_GRID_WIDTH, lastX_atGridWidthPos, TILE_SIZE);
    
}

void Level_TileBased::UpdateLevel(const std::shared_ptr<Player> player, float dt, bool respawn)
{
    // This helps us track when the player has moved a full tile
    int playerPosInGameUnits = (int)(player->GetPosition().x) % TILE_SIZE;

    bool playerAtThreshold = player->GetPosition().x >= camera->GetView().getCenter().x;
    bool playerHasReturned = player->GetPosition().x >= player->GetMaxPosition().x;

    if (playerAtThreshold && playerPosInGameUnits == 0 && playerHasReturned)
    {
        // Simulate the world moving, which allows to place new tiles at the right of the grid
        ShiftGridLeft();
    }
    else
    {
        hasShifted = false;
    }

    // Place new tiles when grid buffer becomes empty
    if (shiftCounter == BUFFER_COLUMNS && playerHasReturned)
    {
        shiftCounter = 0;
        GenerateLevel(GRID_WIDTH);
    }

    obstacleManager.MoveObstacles(dt);
    enemyManager.MoveEnemies(player, dt);
}

void Level_TileBased::PlacePattern(int patternIndex, int currentX, int currentY)
{
    std::vector<std::vector<int>> pattern = patterns[patternIndex];
    int patternHeight = pattern.size();
    int patternWidth = pattern[0].size();

    // if there is no enough space for a whole pattern -> cut the pattern
    if (currentX + patternWidth > GRID_WIDTH + BUFFER_COLUMNS)
    {
        int diff = GRID_WIDTH + BUFFER_COLUMNS - currentX;
        patternWidth = diff;
    }

    for (int y = 0; y < patternHeight; y++)
    {
        for (int x = 0; x < patternWidth; x++)
        {
            if (pattern[y][x] == 1)
            {
                grid[currentY + y][currentX + x] = Tile(Tile::Tile_Type::Dirt, sf::RectangleShape());
            }
        }
    }
}

void Level_TileBased::CheckGround(int curX, float v)
{
    int globalPositionX;
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = curX; x < TOTAL_GRID_WIDTH; ++x)
        {
            if (grid[y][x].GetType() == 1)
            {
                // Calculate the global X position for the tile to render it in the correct screen location
                globalPositionX = v + (x - curX) * TILE_SIZE;

                // differentiate between top of the ground and underground to render different sprites
                bool nothingAbove = grid[y - 1][x].GetType() == 0;
                if (y - 1 >= 0 && nothingAbove || y == 0)
                {
                    sf::Vector2f position(globalPositionX, y * TILE_SIZE);
                    sf::RectangleShape grassShape = CreateBoundRec(position);
                    sf::Sprite grassSprite = CreateSprite(TextureLoader::TextureType::Tile_Grass, TextureLoader::grassX, TextureLoader::grassY, x, y, globalPositionX);
                    tiles.push_back(grassSprite);
                    grid[y][x] = Tile(Tile::Tile_Type::Grass, grassShape);
                }
                else if (y - 1 >= 0 && !nothingAbove)
                {
                    sf::Vector2f position(globalPositionX, y * TILE_SIZE);
                    sf::RectangleShape dirtShape = CreateBoundRec(position);
                    sf::Sprite dirtSprite = CreateSprite(TextureLoader::TextureType::Tile_Dirt, TextureLoader::groundX, TextureLoader::groundY, x, y, globalPositionX);
                    tiles.push_back(dirtSprite);
                    grid[y][x] = Tile(Tile::Tile_Type::Dirt, dirtShape);
                }

                // Track the global X-coordinate of the farthest tile generated
                // This ensures that new tiles can be generated starting from this position (starting at total grid width)
                if (globalPositionX > lastX_atTotalGridWidthPos)
                {
                    lastX_atTotalGridWidthPos = globalPositionX;
                }

                // This ensures that obstacles can be generated starting from this position (starting at grid width)
                if (x <= GRID_WIDTH && globalPositionX > lastX_atGridWidthPos)
                {
                    lastX_atGridWidthPos = globalPositionX;
                }
            }
        }
    }
}

sf::Sprite Level_TileBased::CreateSprite(TextureLoader::TextureType type, int coordX, int coordY, int x, int y, int globalPositionX)
{
    sf::Sprite sprite(txLoader->SetSprite(type));
    sprite.setTextureRect(sf::IntRect(coordX * TILE_SIZE, coordY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    sprite.setPosition(globalPositionX, y * TILE_SIZE);
    return sprite;
}

void Level_TileBased::ShiftGridLeft()
{
    if (!hasShifted)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            for (int x = 0; x < TOTAL_GRID_WIDTH - 1; ++x)
            {
                grid[y][x] = grid[y][x + 1];
            }
            grid[y][TOTAL_GRID_WIDTH - 1] = Tile(Tile::Tile_Type::Empty, sf::RectangleShape());
        }

        shiftCounter++;
        hasShifted = true;
        ShowGrid();
    }
}

sf::RectangleShape Level_TileBased::CreateBoundRec(sf::Vector2f position)
{
    sf::RectangleShape boundingRec;
    boundingRec.setSize(sf::Vector2f(32, 32));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Blue);
    boundingRec.setOutlineThickness(1);
    boundingRec.setPosition(position);
    return boundingRec;
}

void Level_TileBased::ShowGrid() const
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        std::cout << "" << std::endl;
        for (int x = 0; x < TOTAL_GRID_WIDTH; x++)
        {
            std::cout << grid[y][x].GetType();
        }
    }
    std::cout << "" << std::endl;
}

std::vector<Tile>& Level_TileBased::GetAllTiles()
{
    allTiles.clear();

    for(const auto& row: grid)
    {
        for(const auto& tile: row)
        {
            allTiles.push_back(tile);
        }
    }

    for(const auto& obstacle: obstacleManager.GetObstacles())
    {
        allTiles.push_back(Tile(Tile::Obstacle, obstacle->GetBoundingRec()));
    }

    return allTiles;
}

void Level_TileBased::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (const sf::Sprite &tile : tiles)
    {
        window->draw(tile);
    }

    
    for(const auto& row: grid)
    {
        for(const auto& tile: row)
        {
            window->draw(tile.GetShape());
        }
    }

    obstacleManager.Draw(window);
    enemyManager.Draw(window);
}
