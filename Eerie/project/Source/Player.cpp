#include "Player.h"
#include "Map.h"
#include "DxLib.h"

Player::Player()
    : m_x(256.0)
    , m_y(256.0)
    , m_direction(0)
    , m_animFrame(1)
    , m_animCount(0)
    , m_lastDirection(0)
    , m_oldUp(false)
    , m_oldDown(false)
    , m_oldLeft(false)
    , m_oldRight(false)
    , m_prevTime(0)
    , m_debugMoveX(0.0)
    , m_debugMoveY(0.0)
    , m_debugUp(false)
    , m_debugDown(false)
    , m_debugLeft(false)
    , m_debugRight(false)
{
    for (int i = 0; i < 12; i++)
    {
        m_graph[i] = -1;
    }
}

void Player::Update(const Map& map)
{
    int nowTime = GetNowCount();

    if (m_prevTime == 0)
    {
        m_prevTime = nowTime;
        return;
    }

    double deltaTime = (nowTime - m_prevTime) / 1000.0;

    m_prevTime = nowTime;

    if (deltaTime > 0.05)
    {
        deltaTime = 0.05;
    }

    char keyState[256] = {};

    GetHitKeyStateAll(keyState);

    bool up = (keyState[KEY_INPUT_W] == 1);
    bool down = (keyState[KEY_INPUT_S] == 1);
    bool left = (keyState[KEY_INPUT_A] == 1);
    bool right = (keyState[KEY_INPUT_D] == 1);

    m_debugUp = up;
    m_debugDown = down;
    m_debugLeft = left;
    m_debugRight = right;

    if (up && !m_oldUp)
    {
        m_lastDirection = 3;
    }

    if (down && !m_oldDown)
    {
        m_lastDirection = 0;
    }

    if (left && !m_oldLeft)
    {
        m_lastDirection = 1;
    }

    if (right && !m_oldRight)
    {
        m_lastDirection = 2;
    }

    double moveX = 0.0;
    double moveY = 0.0;

    bool isMoving = false;

    switch (m_lastDirection)
    {
    case 0: // 下
        if (down)
        {
            moveY = SPEED * deltaTime;
            m_direction = 0;
            isMoving = true;
        }
        break;

    case 1: // 左
        if (left)
        {
            moveX = -SPEED * deltaTime;
            m_direction = 1;
            isMoving = true;
        }
        break;

    case 2: // 右
        if (right)
        {
            moveX = SPEED * deltaTime;
            m_direction = 2;
            isMoving = true;
        }
        break;

    case 3: // 上
        if (up)
        {
            moveY = -SPEED * deltaTime;
            m_direction = 3;
            isMoving = true;
        }
        break;
    }

    if (!up && !down)
    {
        moveY = 0.0;
    }

    if (!left && !right)
    {
        moveX = 0.0;
    }

    m_debugMoveX = moveX;
    m_debugMoveY = moveY;

    if (moveX != 0.0)
    {
        if (!IsHit(map, m_x + moveX, m_y))
        {
            m_x += moveX;
        }
    }

    if (moveY != 0.0)
    {
        if (!IsHit(map, m_x, m_y + moveY))
        {
            m_y += moveY;
        }
    }

    if (!isMoving)
    {
        if (up)
        {
            moveY = -SPEED * deltaTime;
            m_direction = 3;
            m_lastDirection = 3;
            isMoving = true;
        }
        else if (down)
        {
            moveY = SPEED * deltaTime;
            m_direction = 0;
            m_lastDirection = 0;
            isMoving = true;
        }
        else if (left)
        {
            moveX = -SPEED * deltaTime;
            m_direction = 1;
            m_lastDirection = 1;
            isMoving = true;
        }
        else if (right)
        {
            moveX = SPEED * deltaTime;
            m_direction = 2;
            m_lastDirection = 2;
            isMoving = true;
        }
    }

    // デバッグ用
    m_debugMoveX = moveX;
    m_debugMoveY = moveY;

    bool actuallyMoved = false;

    if (moveX != 0.0)
    {
        if (!IsHit(map, m_x + moveX, m_y))
        {
            m_x += moveX;
            actuallyMoved = true;
        }
    }

    if (moveY != 0.0)
    {
        if (!IsHit(map, m_x, m_y + moveY))
        {
            m_y += moveY;
            actuallyMoved = true;
        }
    }

    if (actuallyMoved)
    {
        m_animCount++;

        if (m_animCount >= 24)
        {
            m_animCount = 0;

            m_animFrame++;

            if (m_animFrame >= 3)
            {
                m_animFrame = 0;
            }
        }
    }
    else
    {
        m_animFrame = 1;
        m_animCount = 0;
    }

    m_oldUp = up;
    m_oldDown = down;
    m_oldLeft = left;
    m_oldRight = right;
}

void Player::Draw(double cameraX, double cameraY) const
{
    int graphIndex = m_direction * 3 + m_animFrame;

    constexpr int DRAW_SIZE = 128;

    int offsetX = (DRAW_SIZE - SIZE) / 2;
    int offsetY = DRAW_SIZE - SIZE;

    int drawX = static_cast<int>(m_x - cameraX);

    int drawY = static_cast<int>(m_y - cameraY);

    DrawExtendGraph(drawX - offsetX, drawY - offsetY, drawX - offsetX + DRAW_SIZE, drawY - offsetY + DRAW_SIZE, m_graph[graphIndex], TRUE);
}

void Player::SetPosition(double x, double y)
{
    m_x = x;
    m_y = y;
}

double Player::GetX() const
{
    return m_x;
}

double Player::GetY() const
{
    return m_y;
}

double Player::GetCenterX() const
{
    return m_x + SIZE / 2.0;
}

double Player::GetCenterY() const
{
    return m_y + SIZE / 2.0;
}

bool Player::IsHit(const Map& map, double x, double y) const
{
    constexpr int margin = 3;

    double left = x + margin;
    double right = x + SIZE - margin;
    double top = y + margin;
    double bottom = y + SIZE - margin;

    if (map.IsWallPoint(left, top))
        return true;

    if (map.IsWallPoint(right, top))
        return true;

    if (map.IsWallPoint(left, bottom))
        return true;

    if (map.IsWallPoint(right, bottom))
        return true;

    return false;
}

bool Player::Load()
{
    int result = LoadDivGraph("data/image/player.png", 12, 3, 4, 64, 64, m_graph);
    return result == 0;
}