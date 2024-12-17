#include <iostream>
#include "Level_TileBased.h"

Level_TileBased::Level_TileBased(const std::shared_ptr<Player> pl, const std::shared_ptr<Camera> cam)
    : player(pl),
      previousPlayerX(pl->GetPosition().x),
      camera(cam)
{}

void Level_TileBased::Initialize()
{
    grid.resize(GRID_HEIGHT, std::vector<int>(TOTAL_GRID_WIDTH, 0));
    patterns = {pattern1, pattern2, pattern3, pattern4, pattern5, pattern6, pattern7, pattern8, pattern9 ,pattern10, pattern11, pattern12};

    // ground without grass
    groundTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png");
    groundSprite.setTexture(groundTexture);
    groundSprite.setTextureRect(sf::IntRect(TileCoordinates::groundX * TILE_SIZE, TileCoordinates::groundY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    // ground with grass
    groundWithGrassTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png");
    groundWithGrassSprite.setTexture(groundWithGrassTexture);
    groundWithGrassSprite.setTextureRect(sf::IntRect(TileCoordinates::grassX * TILE_SIZE, TileCoordinates::grassY * TILE_SIZE, TILE_SIZE, TILE_SIZE));

    GenerateLevel(0);
    ShowGrid();
}

void Level_TileBased::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (const sf::Sprite &tile : tiles)
    {
        window->draw(tile);
    }
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

void Level_TileBased::GenerateLevel(int startX)
{
    int currY = 0;
    int patternWidth = 0;
    int patternHeight = 0;

    //generate platforms on invisible level area (or visible + invisible for initial method call)
    for (int x = startX; x < GRID_WIDTH + BUFFER_COLUMNS; x += patternWidth)
    {

        // Check if current pattern does not go out of bounds when placing it
        int newX = x + patternWidth;
        if(newX >= GRID_WIDTH + BUFFER_COLUMNS)
        {
            break;
        }

        // Randomly choose the pattern
        int patternIndex = rand() % patterns.size();

        // Get the dimensions of the chosen pattern
        patternWidth = patterns[patternIndex][0].size();
        patternHeight = patterns[patternIndex].size();

        // Ensure that the pattern does not exceed the grid height and is above certain level
        currY = rand() % GRID_HEIGHT;
        currY = std::clamp(currY, static_cast<int>(LevelBounds::minY), GRID_HEIGHT - patternHeight);

        PlacePattern(patternIndex, x, currY);
    }

    float currentBound = (startX == 0) ? currentBound = camera->CalculateLeftBound() : currentBound = camera->CalculateRightBound();
    CheckGround(startX, currentBound);
}

void Level_TileBased::UpdateLevel()
{
    // This helps us track when the player has moved a full tile.
    int deltaXInGameUnits = (int)(player->GetPosition().x) % TILE_SIZE;

    // This ensures that we only trigger the tile updates when the camera is moving
    if (player->GetPosition().x >= camera->GetView().getCenter().x && deltaXInGameUnits == 0)
    {
        if (!tiles.empty())
        {
            float leftBound = camera->CalculateLeftBound();

            // This helps to avoid rendering and memory usage for tiles that are no longer needed.
            tiles.erase(
                std::remove_if(tiles.begin(), tiles.end(), [this, leftBound](sf::Sprite &tile)
                               { return tile.getPosition().x + TILE_SIZE < leftBound; }),
                tiles.end());
        }

        // Simulate the world moving, which allows to place new tiles at the right of the grid
        ShiftGridLeft();
    }
    else
    {
        hasShifted = false;
    }

    // Place new tiles when grid buffer becomes empty
    if (shiftCounter == BUFFER_COLUMNS)
    {
        shiftCounter = 0;
        GenerateLevel(GRID_WIDTH);
    }
}

void Level_TileBased::PlacePattern(int patternIndex, int currentX, int currentY)
{
    std::vector<std::vector<int>> pattern = patterns[patternIndex];
    int patternHeight = pattern.size();
    int patternWidth = pattern[0].size();

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

void Level_TileBased::CheckGround(int curX, float v)
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = curX; x < TOTAL_GRID_WIDTH; ++x)
        {
            if (grid[y][x] == 1)
            {

                // Calculate the global X position for the tile to render it in the correct screen location
                float globalTileX = v + (x - curX) * TILE_SIZE;

                // differentiate between top of the ground and underground to render different sprites
                if ((y - 1 >= 0 && grid[y - 1][x] == 0) || y == 0)
                {
                    grid[y][x] = 2;
                    sf::Sprite grassTile(groundWithGrassTexture);
                    grassTile.setTextureRect(sf::IntRect(TileCoordinates::grassX * TILE_SIZE, TileCoordinates::grassY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    grassTile.setPosition(globalTileX, y * TILE_SIZE);
                    tiles.push_back(grassTile);
                }
                else if (y - 1 >= 0 && grid[y - 1][x] != 0)
                {
                    grid[y][x] = 1;
                    sf::Sprite groundTile(groundTexture);
                    groundTile.setTextureRect(sf::IntRect(TileCoordinates::groundX * TILE_SIZE, TileCoordinates::groundY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
                    groundTile.setPosition(globalTileX, y * TILE_SIZE);
                    tiles.push_back(groundTile);
                }
            }
        }
    }
}
