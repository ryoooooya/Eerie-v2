#pragma once
#include <string>
#include <vector>

class Player;

class EventManager
{
private:
    int m_x;
    int m_y;

    bool m_completed;

    double m_blinkTimer = 0.0;

    // このオブジェクトを調べた時の文章


private:
    int m_gimmickID;
    std::vector<std::vector<std::string>> m_text;

public:
    EventManager(int x, int y, int gimmickID, const std::vector<std::vector<std::string>>& text);

    void Update(double deltaTime);
    void Draw(double cameraX, double cameraY) const;

    bool IsNear(const Player& player, double distance) const;

    bool IsCompleted() const;
    void Complete();

    void SetPosition(int x, int y);

    const std::vector<std::vector<std::string>> & GetText() const;

    int GetX() const;
    int GetY() const;

    int GetGimmickID() const;
};