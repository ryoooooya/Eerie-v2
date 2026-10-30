#include "Message.h"
#include "DxLib.h"
#include "Constants.h"

void Message::Draw(
    const std::vector<std::string>& lines)
{
    constexpr int margin = 40;
    constexpr int boxHeight = 200;

    const int left = margin;
    const int top = Constants::SCREEN_HEIGHT - boxHeight - margin;
    const int right = Constants::SCREEN_WIDTH - margin;
    const int bottom = Constants::SCREEN_HEIGHT - margin;

    // 背景
    DrawBox(left, top, right, bottom, GetColor(0, 0, 0), TRUE);

    // 白い枠線
    DrawBox(left,top,right,bottom,GetColor(230, 230, 230),FALSE);

    int y = 0;

    for (const auto& line : lines)
    {
        DrawString(left + 30, top + 30 + y, line.c_str(), GetColor(255, 255, 255));

        y += 32;
    }

    DrawString(left + 1640, top + 140, "Space", GetColor(150, 150, 150));
}

void Message::DrawColored(const std::vector<std::vector<ColoredText>>& lines)
{
    constexpr int margin = 40;
    constexpr int boxHeight = 200;

    const int left = margin;
    const int top = Constants::SCREEN_HEIGHT - boxHeight - margin;

    const int right = Constants::SCREEN_WIDTH - margin;

    const int bottom = Constants::SCREEN_HEIGHT - margin;

    // 背景
    DrawBox(left, top, right, bottom, GetColor(5, 5, 10), TRUE);

    // 枠
    DrawBox(left, top, right, bottom, GetColor(230, 230, 230), FALSE);

    int y = 0;

    for (const auto& line : lines)
    {
        int x = left + 30;

        for (const auto& part : line)
        {
            DrawString(x, top + 30 + y, part.text.c_str(), part.color);

            // 描画した文字列の横幅ぶん次へ
            x += GetDrawStringWidth(part.text.c_str(), static_cast<int>(part.text.size()));
        }

        y += 32;
    }

    DrawString(left + 1640, top + 140, "Space", GetColor(150, 150, 150));
}

void Message::DrawColoredIntro(const std::vector<std::vector<ColoredText>>& lines)
{
    constexpr int lineHeight = 40;

    int totalHeight = static_cast<int>(lines.size()) * lineHeight;

    int y = Constants::SCREEN_HEIGHT / 2 - totalHeight / 2;

    for (const auto& line : lines)
    {
        int lineWidth = 0;

        for (const auto& part : line)
        {
            lineWidth += GetDrawStringWidth(part.text.c_str(), static_cast<int>(part.text.size()));
        }

        int x = Constants::SCREEN_WIDTH / 2 - lineWidth / 2;

        for (const auto& part : line)
        {
            DrawString(x, y, part.text.c_str(), part.color);

            x += GetDrawStringWidth(part.text.c_str(), static_cast<int>(part.text.size()));
        }

        y += lineHeight;
    }

    DrawString(Constants::SCREEN_WIDTH - 250, Constants::SCREEN_HEIGHT - 180, "Space", GetColor(120, 120, 120));
}