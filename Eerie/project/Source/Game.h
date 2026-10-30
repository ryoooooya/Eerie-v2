#pragma once

#include "Player.h"
#include "Map.h"
#include "EventManager.h"
#include "Message.h"

#include <vector>
#include <string>
#include "DxLib.h"

enum class GameState
{
    Title,
    TitleTransition,
    Intro,
    Playing,
    Reading
};

enum class ReadType
{
    None,
    ShineObject,
    ShineObjectAfter
};

class Game
{
private:

    int m_currentEvent = -1;

    double BlinkCount = 0.0;

    double m_transitionTimer = 0.0;

    int m_startSE = -1;

    GameState m_state;

    ReadType m_readType;

    Player m_player;

    int m_prevTime;

    Map m_map;

    std::vector<EventManager> m_events;


    int m_textPage;

    char m_currentKey[256];
    char m_oldKey[256];

    const int TitleBg = LoadGraph("data/image/TitleBg.jpg");
    const int Title = LoadGraph("data/image/Title.png");

private:
    std::vector<std::vector<std::vector<ColoredText>>> m_introText;

    std::vector<std::vector<std::string>> m_shineText;

    std::vector<std::vector<std::string>> m_shineAfterText;

public:
    Game();

    bool Initialize();

    void Update();
    void Draw();

private:
    bool IsKeyPressed(int key) const;

    bool IsNearDoor() const;

    void UpdateTitle();
    void UpdateIntro();
    void UpdatePlaying(double deltaTime);
    void UpdateReading();

    void DrawTitle();
    void DrawIntro();
    void DrawPlaying();
    void DrawReading();

    void StartShineEvent();
    void EndReading();

    void RebuildGimmickEvents();

    void DrawInteractionMessage();
    void ChangeMap(const std::string& mapPath, const std::string& objectPath, const std::string& gimmickPath, double playerX, double playerY);
};