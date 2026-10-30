#include "Map.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

//room01 = エントランス

Map::Map()
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        m_tileGraph[i] = -1;
    }
}

bool Map::LoadCSV(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    m_mapData.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::vector<int> row;

        std::stringstream ss(line);

        std::string value;

        while (std::getline(ss, value, ','))
        {
            row.push_back(std::stoi(value));
        }

        m_mapData.push_back(row);
    }

    file.close();

    return true;
}

bool Map::LoadObjectCSV(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    m_objectData.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::vector<int> row;

        std::stringstream ss(line);

        std::string value;

        while (std::getline(ss, value, ','))
        {
            row.push_back(std::stoi(value));
        }

        m_objectData.push_back(row);
    }

    file.close();

    return true;
}

bool Map::LoadGimmickCSV(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return false;
    }

    m_gimmickData.clear();

    std::string line;

    while (std::getline(file, line))
    {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ','))
        {
            row.push_back(std::stoi(value));
        }

        m_gimmickData.push_back(row);
    }

    file.close();

    return true;
}

bool Map::LoadTexture()
{
    m_tileSheet = LoadGraph("data/image/map.png");

    if (m_tileSheet == -1)
    {
        return false;
    }

    TileRect rects[TILE_COUNT] =
    {
        // x, y, 横幅, 縦幅

        { 0, 0, 98, 114 },   // 0壁ひびわれ
        { 104, 0, 98, 114 },   // 1壁ひび割れ少なめ
        { 818, 0, 98,  114 },   // 2ドア
        { 4, 118, 94, 100 },   // 3床
        { 408, 230, 98, 100 },   // 4畳
        { 0, 460, 98, 112 },   // 5机
        { 99, 460, 104, 112 },   // 6tukue

    };

    for (int i = 0; i < TILE_COUNT; i++)
    {
        m_tileGraph[i] = DerivationGraph(rects[i].x, rects[i].y, rects[i].width, rects[i].height, m_tileSheet);

        if (m_tileGraph[i] == -1)
        {
            return false;
        }
    }

    return true;
}

std::vector<std::pair<int, int>> Map::FindGimmickTiles(int gimmickID) const
{
    std::vector<std::pair<int, int>> result;

    for (int y = 0; y < static_cast<int>(m_gimmickData.size());y++)
    {
        for (int x = 0; x < static_cast<int>(m_gimmickData[y].size()); x++)
        {
            if (m_gimmickData[y][x] == gimmickID)
            {
                result.push_back({ x, y });
            }
        }
    }

    return result;
}

std::vector<std::pair<int, int>>
Map::FindObjectTiles(int objectID) const
{
    std::vector<std::pair<int, int>> result;

    for (int y = 0; y < static_cast<int>(m_objectData.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(m_objectData[y].size()); x++)
        {
            if (m_objectData[y][x] == objectID)
            {
                result.push_back({ x, y });
            }
        }
    }

    return result;
}

void Map::Draw(double cameraX, double cameraY) const
{
    // 地形
    for (int y = 0; y < static_cast<int>(m_mapData.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(m_mapData[y].size()); x++)
        {
            int tileID = m_mapData[y][x];

            if (tileID < 0 || tileID >= TILE_COUNT)
            {
                continue;
            }

            int drawX = static_cast<int>( x * TILE_SIZE - cameraX);

            int drawY = static_cast<int>( y * TILE_SIZE - cameraY);

            DrawExtendGraph(drawX, drawY, drawX + TILE_SIZE, drawY + TILE_SIZE, m_tileGraph[tileID], TRUE);
        }
    }

    // オブジェクト
    for (int y = 0; y < static_cast<int>(m_objectData.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(m_objectData[y].size()); x++)
        {
            int tileID = m_objectData[y][x];

            if (tileID < 0 || tileID >= TILE_COUNT)
            {
                continue;
            }

            int drawX = static_cast<int>( x * TILE_SIZE - cameraX);

            int drawY = static_cast<int>( y * TILE_SIZE - cameraY);

            DrawExtendGraph( drawX, drawY, drawX + TILE_SIZE, drawY + TILE_SIZE, m_tileGraph[tileID], TRUE);
        }
    }
}

bool Map::IsWallPoint(double x, double y) const
{
    int tileX = static_cast<int>(x) / TILE_SIZE;

    int tileY = static_cast<int>(y) / TILE_SIZE;

    if (tileY < 0 || tileY >= static_cast<int>(m_mapData.size()))
    {
        return true;
    }

    if (tileX < 0 || tileX >= static_cast<int>(m_mapData[tileY].size()))
    {
        return true;
    }

    int tile = m_mapData[tileY][tileX];

    return tile == 1 || tile == 2 || tile == -1 || tile == 5 || tile == 6;
}

bool Map::IsDoorTile(int tileX, int tileY) const
{
    if (tileY < 0 || tileY >= static_cast<int>(m_objectData.size()))
    {
        return false;
    }

    if (tileX < 0 || tileX >= static_cast<int>(m_objectData[tileY].size()))
    {
        return false;
    }

    return m_objectData[tileY][tileX] == 2;
}

void Map::SetTile(int x, int y, int value)
{
    if (y < 0 || y >= static_cast<int>(m_mapData.size()))
    {
        return;
    }

    if (x < 0 || x >= static_cast<int>(m_mapData[y].size()))
    {
        return;
    }

    m_mapData[y][x] = value;
}

int Map::GetTile(int x, int y) const
{
    if (y < 0 || y >= static_cast<int>(m_mapData.size()))
    {
        return -1;
    }

    if (x < 0 || x >= static_cast<int>(m_mapData[y].size()))
    {
        return -1;
    }

    return m_mapData[y][x];
}

int Map::GetWidth() const
{
    if (m_mapData.empty())
        return 0;

    return static_cast<int>(m_mapData[0].size());
}

int Map::GetHeight() const
{
    return static_cast<int>(m_mapData.size());
}

void Map::SetObjectTile(int x, int y, int value)
{
    if (y < 0 || y >= static_cast<int>(m_objectData.size()))
    {
        return;
    }

    if (x < 0 || x >= static_cast<int>(m_objectData[y].size()))
    {
        return;
    }

    m_objectData[y][x] = value;
}

int Map::GetObjectTile(int x, int y) const
{
    if(y < 0 || y >= static_cast<int>(m_objectData.size()))
    {
        return -1;
    }

    if (x < 0 || x >= static_cast<int>(m_objectData[y].size()))
    {
        return -1;
    }

    return m_objectData[y][x];
}

void Map::ReplaceObjectTile(int oldID, int newID)
{
    for (int y = 0; y < static_cast<int>(m_objectData.size()); y++)
    {
        for (int x = 0; x < static_cast<int>(m_objectData[y].size()); x++)
        {
            if (m_objectData[y][x] == oldID)
            {
                m_objectData[y][x] = newID;
            }
        }
    }
}

void Map::SetGimmickTile(int x, int y, int value)
{
    if (y < 0 || y >= static_cast<int>(m_gimmickData.size()))
    {
        return;
    }

    if (x < 0 || x >= static_cast<int>(m_gimmickData[y].size()))
    {
        return;
    }

    m_gimmickData[y][x] = value;
}

int Map::GetGimmickTile(int x, int y) const
{
    if (y < 0 || y >= static_cast<int>(m_gimmickData.size()))
    {
        return -1;
    }

    if (x < 0 || x >= static_cast<int>(m_gimmickData[y].size()))
    {
        return -1;
    }

    return m_gimmickData[y][x];
}

