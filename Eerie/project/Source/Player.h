#pragma once

class Map;

class Player
{
private:
    double m_x;
    double m_y;

    int m_graph[12];

    // 0=下 1=左 2=右 3=上
    int m_direction;

    int m_animFrame;
    int m_animCount;

    // 最後に押した方向
    // 0=下 1=左 2=右 3=上
    int m_lastDirection;

    // 前フレームのキー状態
    bool m_oldUp;
    bool m_oldDown;
    bool m_oldLeft;
    bool m_oldRight;

    int m_prevTime;

    bool m_debugUp;
    bool m_debugDown;
    bool m_debugLeft;
    bool m_debugRight;

private:
    double m_debugMoveX;
    double m_debugMoveY;

public:
    static constexpr int SIZE = 64;
    static constexpr double SPEED = 150.0; 

public:
    Player();

    bool Load();

    void Update(const Map& map);
    void Draw(double cameraX, double cameraY) const;

    void SetPosition(double x, double y);

    double GetX() const;
    double GetY() const;

    double GetCenterX() const;
    double GetCenterY() const;

private:
    bool IsHit(const Map& map, double x, double y) const;
};