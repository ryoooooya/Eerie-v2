#pragma once

#include <vector>
#include <string>
#include <utility>

class Map
{
public:
    static constexpr int TILE_SIZE = 128;
    static constexpr int TILE_COUNT = 130;

private:
    struct TileRect
    {
        int x;
        int y;
        int width;
        int height;
    };

    // 地形
    std::vector<std::vector<int>> m_mapData;

    // 家具・扉・箱など
    std::vector<std::vector<int>> m_objectData;

    // 調べる場所・イベント
    std::vector<std::vector<int>> m_gimmickData;

    int m_tileSheet = -1;
    int m_tileGraph[TILE_COUNT];

public:
    Map();

    bool LoadCSV(const std::string& filePath);
    bool LoadObjectCSV(const std::string& filePath);
    bool LoadGimmickCSV(const std::string& filePath);

    bool LoadTexture();

    // gimmick IDを全部探す
    std::vector<std::pair<int, int>>
        FindGimmickTiles(int gimmickID) const;

    // object IDを全部探す
    std::vector<std::pair<int, int>>
        FindObjectTiles(int objectID) const;

    // objectを書き換える
    void SetObjectTile(int x, int y, int value);

    int GetObjectTile(int x, int y) const;

    void ReplaceObjectTile(int oldID, int newID);

    // gimmickを書き換える
    void SetGimmickTile(int x, int y, int value);

    int GetGimmickTile(int x, int y) const;

    void Draw(double cameraX, double cameraY) const;

    bool IsWallPoint(double x, double y) const;

    bool IsDoorTile(int tileX, int tileY) const;

    void SetTile(int x, int y, int value);
    int GetTile(int x, int y) const;

    int GetWidth() const;
    int GetHeight() const;
};