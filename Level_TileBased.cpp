#include <iostream>
#include "Level_TileBased.h"

Level_TileBased::Level_TileBased(const std::shared_ptr<Player> pl, const std::shared_ptr<Camera> cam, std::shared_ptr<TextureLoader> txLoaderRef)
    : player(pl),
      previousPlayerX(pl->GetPosition().x),
      camera(cam),
      txLoader(txLoaderRef)
{
}

void Level_TileBased::Initialize()
{
    grid.resize(GRID_HEIGHT, std::vector<int>(TOTAL_GRID_WIDTH, 0));
    patterns = {defaultPattern, pattern1, pattern2, pattern3, pattern4, pattern5};

    grassSprite = txLoader->SetSprite(TextureLoader::TextureType::Tile_Grass);
    dirtSprite = txLoader->SetSprite(TextureLoader::TextureType::Tile_Dirt);

    int initialWidthPattern = GenerateDefaultTiles();
    GenerateLevel(initialWidthPattern);
    ShowGrid();
}

void Level_TileBased::PlaceObstacles()
{

    for (int y = 0; y < GRID_HEIGHT - 1; ++y)
    {
        for (int x = GRID_WIDTH; x < TOTAL_GRID_WIDTH - 1; ++x)
        {
            // Check if the current cell is empty and the cell below is ground
            bool noTileCurrent = grid[y][x] == 0;
            bool threeConsecutiveTilesUnderneath = ((grid[y + 1][x] == 2) && (grid[y + 1][x + 1] == 2) && (grid[y + 1][x - 1] == 2));
            bool noTilesAlongPath = (grid[y][x + 1] == 0 && grid[y][x - 1] == 0);
            if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath)
            {
                float globalTileX = lastX_atGridWidthPos + (x - GRID_WIDTH) * TILE_SIZE;
                // Randomly decide to place an obstacle
                if (rand() % 2 == 0)
                {
                    grid[y][x] = 3;

                    std::shared_ptr<Obstacle> obstacle = std::make_shared<Obstacle>(txLoader);
                    obstacle->Initialize({globalTileX, static_cast<float>(y * TILE_SIZE)}, 50.0f, globalTileX - TILE_SIZE, globalTileX + TILE_SIZE);
                    sf::Sprite obstacleSprite = obstacle->GetSprite();
                    CreateBoundRecObstacle(obstacle);
                }
            }
        }
    }
}

void Level_TileBased::UpdateObstacle(float dt)
{
    for (auto &obstacle : obstacles)
    {
        obstacle.first->MoveObstacle(dt);
        obstacle.first->UpdateTexture();
        Tile::TileData &tileData = obstacle.second.tileData;
        tileData.shape.setPosition(obstacle.first->GetPosition());
    }
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
    PlaceObstacles();
}

void Level_TileBased::UpdateLevel(float dt, bool respawn)
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
        GenerateLevel(GRID_WIDTH + offsetBetweenTiles);
    }

    if (obstacles.size() > 0)
    {
        UpdateObstacle(dt);
    }
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
                grid[currentY + y][currentX + x] = 1;
            }
        }
    }
}

void Level_TileBased::CheckGround(int curX, float v)
{
    int globalTileX;
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = curX; x < TOTAL_GRID_WIDTH; ++x)
        {
            if (grid[y][x] == 1)
            {
                // Calculate the global X position for the tile to render it in the correct screen location
                globalTileX = v + (x - curX) * TILE_SIZE;

                // differentiate between top of the ground and underground to render different sprites
                bool nothingAbove = grid[y - 1][x] == 0;
                if (y - 1 >= 0 && nothingAbove || y == 0)
                {
                    grid[y][x] = 2;
                    sf::Sprite grassTile(txLoader->GetTexture(TextureLoader::Tile_Grass));
                    grassTile.setTextureRect(sf::IntRect(TextureLoader::grassX * TILE_SIZE, TextureLoader::grassY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    grassTile.setPosition(globalTileX, y * TILE_SIZE);
                    tiles.push_back(grassTile);
                    CreateBoundRecGround(grassTile.getPosition());
                }
                else if (y - 1 >= 0 && !nothingAbove)
                {
                    grid[y][x] = 1;
                    sf::Sprite dirtTile(txLoader->GetTexture(TextureLoader::Tile_Dirt));
                    dirtTile.setTextureRect(sf::IntRect(TextureLoader::groundX * TILE_SIZE, TextureLoader::groundY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    dirtTile.setPosition(globalTileX, y * TILE_SIZE);
                    tiles.push_back(dirtTile);
                    CreateBoundRecGround(dirtTile.getPosition());
                }

                // Track the global X-coordinate of the farthest tile generated
                // This ensures that new tiles can be generated starting from this position (starting at total grid width)
                if (globalTileX > lastX_atTotalGridWidthPos)
                {
                    lastX_atTotalGridWidthPos = globalTileX;
                }

                // This ensures that obstacles can be generated starting from this position (starting at grid width)
                if(x <= GRID_WIDTH && globalTileX > lastX_atGridWidthPos)
                {
                    lastX_atGridWidthPos = globalTileX;
                }
            }
        }
    }

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
            grid[y][TOTAL_GRID_WIDTH - 1] = 0;
        }

        shiftCounter++;
        hasShifted = true;
        ShowGrid();
    }
}

void Level_TileBased::CreateBoundRecGround(const sf::Vector2f position)
{
    sf::RectangleShape boundingRec;
    boundingRec.setSize(sf::Vector2f(32, 32));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Blue);
    boundingRec.setOutlineThickness(1);
    boundingRec.setPosition(position);
    tilesGround.push_back(Tile(Tile::Ground, boundingRec));
    allTiles.push_back(Tile(Tile::Ground, boundingRec));
}

void Level_TileBased::CreateBoundRecObstacle(const std::shared_ptr<Obstacle> obstacle)
{
    sf::RectangleShape boundingRec;
    boundingRec.setSize(sf::Vector2f(32, 32));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Red);
    boundingRec.setOutlineThickness(1);
    boundingRec.setPosition(obstacle->GetPosition());
    obstacles.insert(std::make_pair(obstacle, Tile(Tile::Obstacle, boundingRec)));
}

void Level_TileBased::ShowGrid() const
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        std::cout << "" << std::endl;
        for (int x = 0; x < TOTAL_GRID_WIDTH; x++)
        {
            std::cout << grid[y][x];
        }
    }
    std::cout << "" << std::endl;
}

std::vector<Tile> &Level_TileBased::GetAllTiles()
{
    allTiles.clear();

    for (auto &ground : tilesGround)
    {
        allTiles.push_back(ground);
    }

    for (auto &obstacle : obstacles)
    {
        allTiles.push_back(obstacle.second);
    }

    if (allTiles.size() > 0)
    {
        return allTiles;
    }

    static std::vector<Tile> emptyVector;
    return emptyVector;
}

void Level_TileBased::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (const sf::Sprite &tile : tiles)
    {
        window->draw(tile);
    }

    for (auto &obstacle : obstacles)
    {
        sf::Sprite sprite = obstacle.first->GetSprite();
        Tile::TileData tileData = obstacle.second.tileData;
        window->draw(sprite);
        window->draw(tileData.shape);
    }

    for (const Tile &tile : tilesGround)
    {

        window->draw(tile.tileData.shape);
    }

    for (const Tile &tile : tilesObstacle)
    {
        window->draw(tile.tileData.shape);
    }
}
