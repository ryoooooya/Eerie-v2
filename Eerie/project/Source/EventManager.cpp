#include "EventManager.h"
#include "Player.h"
#include "DxLib.h"

#include <cmath>

EventManager::EventManager(int x, int y, int gimmickID, const std::vector<std::vector<std::string>>& text)
    : m_x(x)
    , m_y(y)
    , m_gimmickID(gimmickID)
    , m_completed(false)
    , m_text(text)
{
}

void EventManager::Update(double deltaTime)
{
    m_blinkTimer += deltaTime;
}

void EventManager::Draw(double cameraX, double cameraY) const
{
    int drawX = static_cast<int>(m_x - cameraX);
    int drawY = static_cast<int>(m_y - cameraY);

    // 0.0 ～ 1.0 をゆっくり往復
    double glow = (std::sin(m_blinkTimer * 3.0) + 1.0) * 0.5;

    // 光の大きさ
    int glowRadius = static_cast<int>(4 + 8 * glow);

    // 半透明描画をON
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

    // 一番外側のぼんやりした光
    DrawCircle(drawX, drawY, glowRadius + 4, GetColor(255, 255, 200), TRUE);

    // 少し内側
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);

    DrawCircle(drawX, drawY, glowRadius, GetColor(255, 255, 210), TRUE);

    // さらに内側
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);

    DrawCircle(drawX, drawY, 4, GetColor(255, 255, 230), TRUE);

    // 中心は強い白
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    DrawCircle(drawX, drawY, 2, GetColor(255, 255, 255), TRUE);

    // 必ず通常描画に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool EventManager::IsNear(const Player& player, double distance) const
{
    double dx = player.GetCenterX() - m_x;

    double dy = player.GetCenterY() - m_y;

    double length = std::sqrt(dx * dx + dy * dy);

    return length < distance;
}

bool EventManager::IsCompleted() const
{
    return m_completed;
}

void EventManager::Complete()
{
    m_completed = true;
}

void EventManager::SetPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

int EventManager::GetX() const
{
    return m_x;
}

int EventManager::GetY() const
{
    return m_y;
}

int EventManager::GetGimmickID() const
{
    return m_gimmickID;
}

const std::vector<std::vector<std::string>>&
EventManager::GetText() const
{
    return m_text;
}