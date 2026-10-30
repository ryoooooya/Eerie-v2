#include "Game.h"
#include "Message.h"
#include "Constants.h"
#include "DxLib.h"

#include <cstring>

Game::Game()
    : m_state(GameState::Title)
    , m_readType(ReadType::None)
    , m_player()
    , m_prevTime(0)
    , m_textPage(0)
{
    memset(m_currentKey, 0, sizeof(m_currentKey));
    memset(m_oldKey, 0, sizeof(m_oldKey));

    m_introText =
    {
        {
            {
                {
                    "15年前、民宿「しろつばき」で",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "宿泊客が９人殺害される事件が起きた。",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "遺体は９人、だがその日の宿泊名簿には",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "１０人の名前があった。",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "「□□　□□　11歳」",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "犯人は結局見つからず、",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "",
                    GetColor(255, 255, 255)
                }
            },

            {
                {
                    "しろつばきは事件後封鎖された。",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "だが...",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "一昨日。宛名のない手紙が届いた。",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "中にはしろつばきのカギと",
                    GetColor(255, 255, 255)
                }
            }
        },

        {
            {
                {
                    "「あの夜、死んだのは",
                    GetColor(255, 50, 50)
                },

                {
                    "９人",
                    GetColor(255, 50, 50)
                },

                {
                    "じゃない」",
                    GetColor(255, 50, 50)
                }
            }
        },

        {
            {
                {
                    "という紙が入っていた",
                    GetColor(255, 255, 255)
                }
            }
        }
    };

    m_shineText =
    {
        {
            "床に古い紙切れが落ちている。",
            "",
            "何か書かれているようだ。"
        },

        {
            "「東の扉は、館の時計が止まった時に開く。」",
            "",
            "紙にはそれだけが書かれている。"
        },

        {
            "その瞬間...",
            "",
            "館の奥から大きな音がした。"
        }
    };

    m_shineAfterText =
    {
        {
            "古い紙切れだ。",
            "",
            "もう変わったことはなさそうだ。"
        }
    };
}

bool Game::Initialize()
{
    if (!m_map.LoadTexture())
    {
        return false;
    }

    // 地形
    if (!m_map.LoadCSV("data/maps/room01.csv"))
    {
        return false;
    }

    // オブジェクト
    if (!m_map.LoadObjectCSV("data/maps/room01_object.csv"))
    {
        return false;
    }

    // ギミック
    if (!m_map.LoadGimmickCSV("data/maps/room01_gimmick.csv"))
    {
        return false;
    }

    // ギミックCSVからイベント生成
    RebuildGimmickEvents();

    if (!m_player.Load())
    {
        return false;
    }

    m_startSE =
        LoadSoundMem("data/sound/StartSound.mp3");

    return true;
}

void Game::Update()
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

    BlinkCount += deltaTime;
    if (BlinkCount > 1.0)
    {
        BlinkCount = 0;
    }

    GetHitKeyStateAll(m_currentKey);

    switch (m_state)
    {
    case GameState::Title:

        UpdateTitle();

        break;
    
    case GameState::TitleTransition:

        m_transitionTimer += deltaTime;

        if (m_transitionTimer >= 1.5)
        {
            m_transitionTimer = 0.0;

            m_textPage = 0;

            m_state = GameState::Intro;
        }

        break;

    case GameState::Intro:

        UpdateIntro();

        break;

    case GameState::Playing:

        UpdatePlaying(deltaTime);

        break;

    case GameState::Reading:

        UpdateReading();

        break;

    }

    memcpy(m_oldKey, m_currentKey, sizeof(m_currentKey));
}

void Game::Draw()
{
    switch (m_state)
    {
    case GameState::Title:

        DrawTitle();

        break;

    case GameState::Intro:

        DrawIntro();

        break;

    case GameState::Playing:

        DrawPlaying();

        break;

    case GameState::Reading:

        DrawReading();

        break;
    }
}

bool Game::IsKeyPressed(int key) const
{
    return
        m_currentKey[key] != 0 &&
        m_oldKey[key] == 0;
}

bool Game::IsNearDoor() const
{
    int playerTileX = static_cast<int>(m_player.GetX()) / Map::TILE_SIZE;

    int playerTileY = static_cast<int>(m_player.GetY()) / Map::TILE_SIZE;

    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1, 1, 0, 0 };

    for (int i = 0; i < 4; i++)
    {
        int checkX = playerTileX + dx[i];
        int checkY = playerTileY + dy[i];

        if (m_map.IsDoorTile(checkX, checkY))
        {
            return true;
        }
    }

    return false;
}

void Game::UpdateTitle()
{
    if (IsKeyPressed(KEY_INPUT_SPACE) ||
        IsKeyPressed(KEY_INPUT_RETURN))
    {
        // 暗転状態へ
        m_state = GameState::TitleTransition;

        m_transitionTimer = 0.0;

        // 効果音
        if (m_startSE != -1)
        {
            PlaySoundMem(m_startSE, DX_PLAYTYPE_BACK);
        }
    }
}

void Game::UpdateIntro()
{
    if (!IsKeyPressed(KEY_INPUT_SPACE) &&
        !IsKeyPressed(KEY_INPUT_RETURN))
    {
        return;
    }

    m_textPage++;

    if (m_textPage >=static_cast<int>(m_introText.size()))
    {
        m_textPage = 0;

        m_state = GameState::Playing;
    }
}

void Game::UpdatePlaying(double deltaTime)
{
    for (auto& event : m_events)
    {
        event.Update(deltaTime);
    }
    m_player.Update(m_map);

    int playerTileX = static_cast<int>(m_player.GetX()) / Map::TILE_SIZE;

    int playerTileY = static_cast<int>(m_player.GetY()) / Map::TILE_SIZE;

    // プレイヤーの上下左右
    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1, 1, 0, 0 };

    for (int i = 0; i < 4; i++)
    {
        int checkX = playerTileX + dx[i];
        int checkY = playerTileY + dy[i];

        // タイル2ならドア
        if (m_map.IsDoorTile(checkX, checkY))
        {
            if (IsKeyPressed(KEY_INPUT_SPACE) ||
                IsKeyPressed(KEY_INPUT_RETURN))
            {
                ChangeMap("data/maps/room02.csv", "data/maps/room02_object.csv", "data/maps/room02_gimmick.csv",
                        // room02に入った時の位置
                        Map::TILE_SIZE * 3 + Map::TILE_SIZE / 2,
                        Map::TILE_SIZE * 8 + Map::TILE_SIZE / 2
                        );

                return;
            }
        }
    }

    // 光るオブジェクト
    for (int i = 0; i < static_cast<int>(m_events.size()); i++)
    {
        if (m_events[i].IsNear(m_player, 50.0))
        {
            if (IsKeyPressed(KEY_INPUT_SPACE) || IsKeyPressed(KEY_INPUT_RETURN))
            {
                m_currentEvent = i;

                m_shineText = m_events[i].GetText();

                StartShineEvent();

                return;
            }
        }
    }
}


void Game::UpdateReading()
{
    if (!IsKeyPressed(KEY_INPUT_SPACE) && !IsKeyPressed(KEY_INPUT_RETURN))
    {
        return;
    }

    const std::vector<std::vector<std::string>>*texts = nullptr;

    switch (m_readType)
    {
    case ReadType::ShineObject:

        texts = &m_shineText;

        break;

    case ReadType::ShineObjectAfter:

        texts = &m_shineAfterText;

        break;

    default:
        break;
    }

    if (texts == nullptr)
        return;

    m_textPage++;

    if (m_textPage >= static_cast<int>(texts->size()))
    {
        EndReading();
    }
}

void Game::StartShineEvent()
{
    m_textPage = 0;

    if (m_currentEvent < 0 || m_currentEvent >= static_cast<int>(m_events.size()))
    {
        return;
    }

    if (m_events[m_currentEvent].IsCompleted())
    {
        m_readType = ReadType::ShineObjectAfter;
    }
    else
    {
        m_readType = ReadType::ShineObject;
    }

    m_state = GameState::Reading;
}

void Game::EndReading()
{
    if (m_readType == ReadType::ShineObject)
    {
        if (m_currentEvent >= 0 && m_currentEvent < static_cast<int>(m_events.size()))
        {
            m_events[m_currentEvent].Complete();

            // 101を読んだら102を103に変更
            int eventID = m_events[m_currentEvent].GetGimmickID();

            if (eventID == 101)
            {
                m_map.ReplaceObjectTile(102, 103);
            }
        }
    }

    m_readType = ReadType::None;
    m_textPage = 0;
    m_currentEvent = -1;
    m_state = GameState::Playing;
}

void Game::RebuildGimmickEvents()
{
    m_events.clear();
    m_currentEvent = -1;

    auto AddEvents = [this](int gimmickID, const std::vector<std::vector<std::string>>& text)
        {
            auto positions = m_map.FindGimmickTiles(gimmickID);

            for (const auto& pos : positions)
            {
                int worldX = pos.first * Map::TILE_SIZE + Map::TILE_SIZE / 2;

                int worldY = pos.second * Map::TILE_SIZE + Map::TILE_SIZE / 2;

                m_events.emplace_back(worldX, worldY, gimmickID, text);
            }
        };

    // gimmick ID

    AddEvents
    (
        201,
        {
            {
                "床に古びた写真が落ちている。",
                "",
                "かなり昔に撮られたもののようだ。"
            },

            {
                "写真には、この民宿の前で",
                "",
                "十人ほどの人間が並んで写っている。"
            },

            {
                "その中の一人だけ、",
                "",
                "顔の部分が黒く塗り潰されている……。"
            }
        }
    );
}

void Game::DrawTitle()
{
    DrawBox(0, 0, 800, 600, GetColor(0, 0, 0), TRUE);

    SetFontSize(128);

    DrawExtendGraph(0, 0, Constants::SCREEN_WIDTH / 1, Constants::SCREEN_HEIGHT / 1, TitleBg, TRUE);
    DrawExtendGraph(0, 0, Constants::SCREEN_WIDTH / 1, Constants::SCREEN_HEIGHT / 1, Title, TRUE);

    SetFontSize(48);

    if (BlinkCount < 0.5)
    {
        DrawString(Constants::SCREEN_WIDTH / 2.4, Constants::SCREEN_HEIGHT / 1.4, "Spaceで開始", GetColor(180, 180, 180));
    }
}

void Game::DrawIntro()
{
    // 背景を黒にする
    DrawBox(0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);

    if (m_textPage >= 0 && m_textPage < static_cast<int>(m_introText.size()))
    {
        Message::DrawColoredIntro(m_introText[m_textPage]);
    }
}

void Game::DrawPlaying()
{
    double cameraX = m_player.GetX() - Constants::SCREEN_WIDTH / 2.0;

    double cameraY = m_player.GetY() - Constants::SCREEN_HEIGHT / 2.0;

    m_map.Draw(cameraX, cameraY);

    for (const auto& event : m_events)
    {
        event.Draw(cameraX, cameraY);
    }

    m_player.Draw(cameraX, cameraY);

    DrawInteractionMessage();
}

void Game::DrawReading()
{
    double cameraX = m_player.GetX() - Constants::SCREEN_WIDTH / 2.0;

    double cameraY = m_player.GetY() - Constants::SCREEN_HEIGHT / 2.0;

    // 背景としてゲーム画面を描く
    m_map.Draw(cameraX, cameraY);

    for (const auto& event : m_events)
    {
        event.Draw(cameraX, cameraY);
    }

    m_player.Draw(cameraX, cameraY);

    const std::vector<std::vector<std::string>>*texts = nullptr;

    switch (m_readType)
    {
    case ReadType::ShineObject:
        texts = &m_shineText;
        break;

    case ReadType::ShineObjectAfter:
        texts = &m_shineAfterText;
        break;

    default:
        break;
    }

    if (texts == nullptr)
        return;

    if (m_textPage < 0 || m_textPage >= static_cast<int>(texts->size()))
    {
        return;
    }

    Message::Draw((*texts)[m_textPage]);
}

void Game::DrawInteractionMessage()
{
    const char* message = nullptr;

    // ドア
    if (IsNearDoor())
    {
        message = "Spaceで入る";
    }
    else
    {
        // 調べられるオブジェクト
        for (const auto& event : m_events)
        {
            if (event.IsNear(m_player, 50.0))
            {
                message = "Spaceで調べる";
                break;
            }
        }
    }

    if (message == nullptr)
    {
        return;
    }

    /*(Constants::SCREEN_WIDTH / 2 - 150, Constants::SCREEN_HEIGHT / 1.3, Constants::SCREEN_WIDTH / 2 + 150, Constants::SCREEN_HEIGHT / 1.2, GetColor(0, 0, 0), TRUE);*/

    DrawString(Constants::SCREEN_WIDTH / 2 - 150, Constants::SCREEN_HEIGHT / 1.3, message, GetColor(255, 255, 255));
}

void Game::ChangeMap(const std::string& mapPath, const std::string& objectPath, const std::string& gimmickPath, double playerX, double playerY)
{
    if (!m_map.LoadCSV(mapPath))
    {
        return;
    }

    if (!m_map.LoadObjectCSV(objectPath))
    {
        return;
    }

    if (!m_map.LoadGimmickCSV(gimmickPath))
    {
        return;
    }

    // 新しい部屋のイベントを作り直す
    RebuildGimmickEvents();

    m_player.SetPosition(playerX, playerY);
}