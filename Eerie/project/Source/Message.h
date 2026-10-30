#pragma once

#include <string>
#include <vector>

struct ColoredText
{
    std::string text;
    unsigned int color;
};

class Message
{
public:
    // 通常会話：下のボックス
    static void Draw(const std::vector<std::string>& lines);

    // 通常会話：色付き＋下のボックス
    static void DrawColored(const std::vector<std::vector<ColoredText>>& lines);

    // イントロ：色付き＋画面中央＋ボックスなし
    static void DrawColoredIntro(const std::vector<std::vector<ColoredText>>& lines);
};