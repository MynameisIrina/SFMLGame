#include <iostream>
#include "Level.h"
#include "Utilities.h"

Level::Level(std::shared_ptr<ObstacleManager> obstacleManager, std::shared_ptr<EnemyManager> enemyManager, std::shared_ptr<CollectibleManager> collectibleManager, const std::shared_ptr<TextureLoader> &txLoader)
    : txLoader(txLoader),
      obstacleManager(obstacleManager),
      enemyManager(enemyManager),
      collectibleManager(collectibleManager) {}

void Level::Initialize(const int firstPlatformHeight)
{
    this->firstPlatformHeight = firstPlatformHeight;

    grid.resize(gridHeight, std::vector<Tile>(totalGridWidth, Tile(Tile::Tile_Type::Empty, sf::RectangleShape())));
    patterns = {defaultPattern, pattern2, pattern3, pattern5, pattern6, pattern7};

    const int initialWidthPattern = GenerateDefaultTiles();
    GenerateLevel(initialWidthPattern);
    ShowGrid();
}

int Level::GenerateDefaultTiles()
{
    int currY = firstPlatformHeight;
    int patternWidth = defaultPattern[0].size();
    int patternHeight = defaultPattern.size();

    for (int x = 0; x < patternWidth; x += patternWidth)
    {
        int newX = x + patternWidth;
        bool inBounds = newX < gridWidth + bufferColumns;
        if (!inBounds)
            break;

        PlacePattern(0, patternHeight, patternWidth, x, currY);
    }

    UpdateGround(0, 0);

    return patternWidth;
}

void Level::GenerateLevel(const int startX)
{
    const int numPatterns = patterns.size();
    const int minPlatformHeight = LevelBounds::minY;
    int maxPlatformHeight = gridHeight - 3;

    for (const auto &pattern : patterns)
    {
        maxPlatformHeight = std::min(maxPlatformHeight, static_cast<int>(gridHeight - pattern.size()));
    }
    const int maxPlatformGap = 2;

    int currentY = prevYFromLevelGen;
    int previousY = prevYFromLevelGen;
    int patternWidth = 0;
    int patternHeight = 0;

    int renderOffset = furthestTileX_totalGrid + tileSize;

    for (int x = startX; x < gridWidth + bufferColumns; x += patternWidth)
    {
        const int patternIndex = rand() % numPatterns;
        patternWidth = patterns[patternIndex][0].size();
        patternHeight = patterns[patternIndex].size();

        // cut the width if it exceeds agrid bound
        bool widthExceedsGridBounds = x + patternWidth > gridWidth + bufferColumns;
        if (widthExceedsGridBounds)
        {
            patternWidth = gridWidth + bufferColumns - x;
        }

        previousY = currentY;

        // Generate a random platform height
        currentY = rand() % (maxPlatformHeight - minPlatformHeight) + minPlatformHeight;

        // Prevent excessive height gaps between platforms
        currentY = AdjustPlatformHeight(previousY, currentY, maxPlatformGap, minPlatformHeight, maxPlatformHeight);

        PlacePattern(patternIndex, patternHeight, patternWidth, x, currentY);
    }

    prevYFromLevelGen = currentY;

    // Check generated grid to differentiate between different tile types
    UpdateGround(startX, renderOffset);

    obstacleManager->SpawnObstacles(grid, gridHeight, nextGridColumn, totalGridWidth, furthestTileX_gridWidth, tileSize);
    enemyManager->SpawnEnemies(grid, gridHeight, nextGridColumn, totalGridWidth, furthestTileX_gridWidth, tileSize);
}

int Level::AdjustPlatformHeight(const int previousHeight, int currentHeight, const int maxGap, const int minHeight, const int maxHeight) const
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

void Level::SpawnTree(const int startX, const float renderOffset, const int x, const int y)
{
    sf::Sprite tree = txLoader->SetSprite(TextureLoader::Tree);
    tree.setOrigin(tree.getLocalBounds().width * 0.5f, tree.getLocalBounds().height);
    const float globalTileX = renderOffset + static_cast<float>((x - startX) * tileSize) + (tileSize * 0.5f);
    const float globalTileY = static_cast<float>(y * tileSize);
    tree.setPosition(globalTileX, globalTileY);
    treeSprites.emplace_back(std::move(tree));

    grid[y][x] = Tile(Tile::Tile_Type::Tree, sf::RectangleShape());
}

void Level::UpdateLevel(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    // Track when the player has moved a full tile
    const int playerPosInGameUnits = (int)(player->GetPosition().x) % tileSize;
    const bool playerAtThreshold = player->GetPosition().x >= camera->GetView().getCenter().x;
    const bool playerHasReturned = player->GetPosition().x >= player->GetMaxPosition().x;
    const bool shouldShiftGrid = playerAtThreshold && playerPosInGameUnits == 0 && playerHasReturned;
    const bool shouldGenerateNewTiles = furthestTileX_totalGrid <= camera->CalculateRightBound();

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
    if (shouldGenerateNewTiles && playerHasReturned)
    {
        shiftCounter = 0;
        GenerateLevel(nextGridColumn);
    }

    obstacleManager->UpdateObstacles(dt);
    enemyManager->UpdateEnemies(player, camera, collectibleManager, dt);
    collectibleManager->UpdateCollectibles(player, dt);
}

void Level::PlacePattern(const int patternIndex, const int height, const int width, const int currentX, const int currentY)
{
    std::vector<std::vector<int>> pattern = patterns[patternIndex];

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (pattern[y][x] == 1)
            {
                grid[currentY + y][currentX + x] = Tile(Tile::Tile_Type::Dirt, sf::RectangleShape());
            }
        }
    }
}

void Level::TrackLastGeneratedPositions(const int worldPositionX, const int x)
{
    if (worldPositionX > furthestTileX_totalGrid)
    {
        furthestTileX_totalGrid = worldPositionX;
    }

    if (x <= nextGridColumn && worldPositionX > furthestTileX_gridWidth)
    {
        furthestTileX_gridWidth = worldPositionX;
    }
}

void Level::UpdateGrid(const Tile &tile, const int x, const int y)
{
    grid[y][x] = tile;
}

void Level::UpdateGround(const int startX, const float renderOffset)
{
    int worldPositionX;
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = startX; x < totalGridWidth; ++x)
        {
            if (grid[y][x].GetType() == 1)
            {

                // Calculate the global X position for the tile to render it in the correct screen location
                worldPositionX = renderOffset + (x - startX) * tileSize;

                TextureLoader::TextureType textureType = DetermineTextureType(x, y);
                sf::Vector2i spriteCoordinates = txLoader->GetSpriteCoordinates(textureType);
                Tile::Tile_Type tileType = DetermineTileType(x, y);
                sf::Sprite sprite = CreateSprite(textureType, spriteCoordinates.x, spriteCoordinates.y, x, y, worldPositionX);
                sf::RectangleShape bb = Utilities::CreateBoundingBox(sprite, sf::Vector2f(worldPositionX, y * tileSize));
                bb.setOrigin(sprite.getOrigin().x, sprite.getOrigin().y);
                Tile tile = Tile(tileType, bb);

                UpdateGrid(tile, x, y);
                groundTiles.emplace_back(tile);
                tileSprites.emplace_back(sprite);
                TrackLastGeneratedPositions(worldPositionX, x);

                if (tileType == Tile::Grass && rand() % treeGenerationProbability == 0)
                {
                    SpawnTree(startX, renderOffset, x, y);
                }
            }
        }
    }
}

sf::Sprite Level::CreateSprite(const TextureLoader::TextureType textureType, const int spriteCoord_X, const int spriteCoord_Y, const int x, const int y, const int worldPositionX)
{
    sf::Sprite sprite(txLoader->SetSprite(textureType));
    sprite.setTextureRect(sf::IntRect(spriteCoord_X * tileSize, spriteCoord_Y * tileSize, tileSize, tileSize));
    sprite.setPosition(worldPositionX, y * tileSize);
    return sprite;
}

void Level::ShiftGridLeft()
{
    if (!hasShifted)
    {
        for (int y = 0; y < gridHeight; ++y)
        {
            for (int x = 0; x < totalGridWidth - 1; ++x)
            {
                grid[y][x] = grid[y][x + 1];
            }
            grid[y][totalGridWidth - 1] = Tile(Tile::Tile_Type::Empty, sf::RectangleShape());
        }

        shiftCounter++;
        hasShifted = true;
        ShowGrid();
    }
}

void Level::ShowGrid() const
{
    for (int y = 0; y < gridHeight; y++)
    {
        std::cout << "" << std::endl;
        for (int x = 0; x < totalGridWidth; x++)
        {
            std::cout << grid[y][x].GetType();
        }
    }
    std::cout << "" << std::endl;
}

std::vector<sf::RectangleShape> &Level::GetAllTiles()
{
    // Keep the vector updated due to new generated tiles
    allTiles.clear();

    // Combine all tiles to check collision
    for (const auto &groundTile : groundTiles)
    {
        allTiles.push_back(groundTile.GetShape());
    }

    return allTiles;
}

void Level::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (const auto &treeSprite : treeSprites)
    {
        window->draw(treeSprite);
    }

    for (const auto &sprite : tileSprites)
    {
        window->draw(sprite);
    }

    obstacleManager->Draw(window);
    enemyManager->Draw(window);
    collectibleManager->Draw(window);
}

TextureLoader::TextureType Level::DetermineTextureType(const int x, const int y) const
{
    bool nothingAbove = grid[y - 1][x].GetType() == Tile::Tile_Type::Empty;
    if (y - 1 >= 0 && nothingAbove || y == 0)
    {
        return TextureLoader::TileGrass;
    }

    return TextureLoader::TileDirt;
}

Tile::Tile_Type Level::DetermineTileType(const int x, const int y) const
{
    bool nothingAbove = grid[y - 1][x].GetType() == Tile::Tile_Type::Empty;
    if (y - 1 >= 0 && nothingAbove || y == 0)
    {
        return Tile::Tile_Type::Grass;
    }

    return Tile::Tile_Type::Dirt;
}
