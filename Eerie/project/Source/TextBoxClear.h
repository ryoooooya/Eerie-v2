#pragma once
#include "../Library/GameObject.h"
#include "Common.h"

enum class ClearText
{
	//STATE終了
	STATE_END = 0,

	//プロローグ
	STATE_PRO = 1,
	//脱出後
	STATE_AFTER_ESCAPE,

	//同じ内容の文章で複数文章がある場合使用する
	STATE_STRING_1,
	STATE_STRING_2,
	STATE_STRING_3,
	STATE_STRING_4,
};

class TextBoxClear:public GameObject
{
public:
	TextBoxClear();
	~TextBoxClear();
	void Update() override;
	void Draw() override;

public:
void SetAfterEscape()
{
	MainState = ClearText::STATE_AFTER_ESCAPE;
	TalkState = ClearText::STATE_STRING_1;
}

private:
	ClearText MainState;
	ClearText TalkState;

private:
	const int BoxImage = LoadGraph("data/image/TextBox.png");
	const int NEXT_TEXT_SOUND = LoadSoundMem("data/sound/Nexttext.mp3");

private:
	const int BoxPosX = 350;
	const int BoxPosY = 500;
	const int HintY = 580;//textのY座標
};
