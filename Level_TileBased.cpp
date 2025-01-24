#include <iostream>
#include "Level_TileBased.h"

Level_TileBased::Level_TileBased(const std::shared_ptr<TextureLoader>& txLoader)
    : txLoader(txLoader),
      obstacleManager(txLoader),
      enemyManager(txLoader) {}


void Level_TileBased::Initialize()
{
    grid.resize(GRID_HEIGHT, std::vector<Tile>(TOTAL_GRID_WIDTH, Tile(Tile::Tile_Type::Empty, sf::RectangleShape())));
    patterns = {defaultPattern};

    int initialWidthPattern = GenerateDefaultTiles();
    GenerateLevel(initialWidthPattern);
    ShowGrid();
}

int Level_TileBased::GenerateDefaultTiles()
{
    int currY = 8;
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

        PlacePattern(0, patternHeight, patternWidth, x, currY);
    }

    UpdateGround(0, 0);

    return patternWidth;
}

void Level_TileBased::GenerateLevel(int startX)
{
    const int numPatterns = patterns.size();
    const int minPlatformHeight = LevelBounds::minY;
    int maxPlatformHeight = GRID_HEIGHT;

    for (const auto &pattern : patterns)
    {
        maxPlatformHeight = std::min(maxPlatformHeight, static_cast<int>(GRID_HEIGHT - pattern.size()));
    }
    const int maxPlatformGap = 2;

    int currentY = prevYFromLevelGen;
    int previousY = prevYFromLevelGen;
    int patternWidth = 0;
    int patternHeight = 0;

    int renderOffset = furthestTileX_totalGrid + TILE_SIZE;

    for (int x = startX; x < GRID_WIDTH + BUFFER_COLUMNS; x += patternWidth)
    {
        const int patternIndex = rand() % numPatterns;
        patternWidth = patterns[patternIndex][0].size();
        patternHeight = patterns[patternIndex].size();

        // cut the width if it exceeds agrid bound
        bool widthExceedsGridBounds = x + patternWidth > GRID_WIDTH + BUFFER_COLUMNS;
        if (widthExceedsGridBounds)
        {
            patternWidth = GRID_WIDTH + BUFFER_COLUMNS - x;
        }

        previousY = currentY;

        // Generate a random platform height
        currentY = rand() % (maxPlatformHeight - minPlatformHeight) + minPlatformHeight;

        // Prevent excessive height gaps between platforms
        currentY = AdjustPlatformHeight(previousY, currentY, maxPlatformGap, minPlatformHeight, maxPlatformHeight);

        PlacePattern(patternIndex, patternHeight , patternWidth, x, currentY);
    }

    prevYFromLevelGen = currentY;

    // Check generated grid to differentiate between different tile types
    UpdateGround(startX, renderOffset);

    obstacleManager.SpawnObstacles(grid, GRID_HEIGHT, GRID_WIDTH, TOTAL_GRID_WIDTH, furthestTileX_gridWidth, TILE_SIZE);
    enemyManager.SpawnEnemies(grid, GRID_HEIGHT, GRID_WIDTH, TOTAL_GRID_WIDTH, furthestTileX_gridWidth, TILE_SIZE);
}

int Level_TileBased::AdjustPlatformHeight(int previousHeight, int currentHeight, int maxGap, int minHeight, int maxHeight) const
{
    bool currentPlatformBelow = currentHeight - previousHeight > maxGap;
    bool currentPlatformTooHigh = previousHeight - currentHeight > maxGap;
    
    if (currentPlatformBelow)
    {
        currentHeight = previousHeight + maxGap;
    }
    else if (currentPlatformTooHigh)
    {
        int diff = previousHeight - currentHeight;
        int amountToLower = std::abs(maxGap - diff);
        currentHeight += amountToLower;
    }

    // make sure the platformdoes not exceed grid height
    currentHeight = std::clamp(currentHeight, minHeight, maxHeight);

    return currentHeight;
}

void Level_TileBased::UpdateLevel(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt)
{
    // Track when the player has moved a full tile
    int playerPosInGameUnits = (int)(player->GetPosition().x) % TILE_SIZE;
    bool playerAtThreshold = player->GetPosition().x >= camera->GetView().getCenter().x;
    bool playerHasReturned = player->GetPosition().x >= player->GetMaxPosition().x;
    bool shouldShiftGrid = playerAtThreshold && playerPosInGameUnits == 0 && playerHasReturned;

    if (shouldShiftGrid)
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

void Level_TileBased::PlacePattern(int patternIndex, int height, int width, int currentX, int currentY)
{
    std::vector<std::vector<int>> pattern = patterns[patternIndex];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (pattern[y][x] == 1)
            {
                grid[currentY + y][currentX + x] = Tile(Tile::Tile_Type::Dirt, sf::RectangleShape());
            }
        }
    }
}

void Level_TileBased::TrackLastGeneratedPositions(int worldPositionX, int x)
{
    if (worldPositionX > furthestTileX_totalGrid)
    {
        furthestTileX_totalGrid = worldPositionX;
    }

    if (x <= GRID_WIDTH && worldPositionX > furthestTileX_gridWidth)
    {
        furthestTileX_gridWidth = worldPositionX;
    }
}

void Level_TileBased::UpdateGrid(Tile tile, int x, int y)
{
    grid[y][x] = tile;
}

void Level_TileBased::UpdateGround(int startX, float renderOffset)
{
    int worldPositionX;
    for (int y = 0; y < GRID_HEIGHT; ++y)
    {
        for (int x = startX; x < TOTAL_GRID_WIDTH; ++x)
        {
            if (grid[y][x].GetType() == 1)
            {
                // Calculate the global X position for the tile to render it in the correct screen location
                worldPositionX = renderOffset + (x - startX) * TILE_SIZE;

                TextureLoader::TextureType textureType = DetermineTextureType(x, y);
                sf::Vector2i spriteCoordinates = txLoader->GetSpriteCoordinates(textureType);
                Tile::Tile_Type tileType = DetermineTileType(x, y);
                sf::Sprite sprite = CreateSprite(textureType, spriteCoordinates.x, spriteCoordinates.y, x, y, worldPositionX);
                Tile tile = Tile(tileType, CreateBoundRec(sf::Vector2f(worldPositionX, y * TILE_SIZE)));

                UpdateGrid(tile, x, y);
                groundTiles.push_back(tile);
                tileSprites.push_back(sprite);
                TrackLastGeneratedPositions(worldPositionX, x);
            }
        }
    }
}



sf::Sprite Level_TileBased::CreateSprite(TextureLoader::TextureType textureType, int spriteCoord_X, int spriteCoord_Y, int x, int y, int worldPositionX)
{
    sf::Sprite sprite(txLoader->SetSprite(textureType));
    sprite.setTextureRect(sf::IntRect(spriteCoord_X * TILE_SIZE, spriteCoord_Y * TILE_SIZE, TILE_SIZE, TILE_SIZE));
    sprite.setPosition(worldPositionX, y * TILE_SIZE);
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

std::vector<Tile> &Level_TileBased::GetAllTiles()
{
    // Keep the vector updated due to new generated tiles
    allTiles.clear();

    // Combine ground and obstacle tiles to check collision
    for(const auto& groundTile: groundTiles)
    {
        allTiles.push_back(groundTile);
    }

    for (const auto &obstacle : obstacleManager.GetObstacles())
    {
        allTiles.push_back(Tile(Tile::Obstacle, obstacle.GetBoundingBox()));
    }

    return allTiles;
}

void Level_TileBased::Draw(const std::shared_ptr<sf::RenderWindow>& window) const
{
    for (const sf::Sprite &sprite : tileSprites)
    {
        window->draw(sprite);
    }

    for(const auto& groundTile: groundTiles)
    {
        window->draw(groundTile.GetShape());
    }

    obstacleManager.Draw(window);
    enemyManager.Draw(window);
}

TextureLoader::TextureType Level_TileBased::DetermineTextureType(int x, int y) const
{
    bool nothingAbove = grid[y - 1][x].GetType() == Tile::Tile_Type::Empty;
    if (y - 1 >= 0 && nothingAbove || y == 0)
    {
        return TextureLoader::Tile_Grass;
    }

    return TextureLoader::Tile_Dirt;
}

Tile::Tile_Type Level_TileBased::DetermineTileType(int x, int y) const
{
    bool nothingAbove = grid[y - 1][x].GetType() == Tile::Tile_Type::Empty;
    if (y - 1 >= 0 && nothingAbove || y == 0)
    {
        return Tile::Tile_Type::Grass;
    }

    return Tile::Tile_Type::Dirt;
}