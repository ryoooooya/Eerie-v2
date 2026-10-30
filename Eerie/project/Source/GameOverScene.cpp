#include "GameOverScene.h"
#include "Fader.h"

GameOverScene::GameOverScene()
{
	Fader* fader = FindGameObject<Fader>();
	fader->FadeIn(3.0f);
	GameOverBg = LoadGraph("data/image/gameover.jpg");
	Blinkcount = 0;
}

GameOverScene::~GameOverScene()
{
	DeleteGraph(GameOverBg);
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_O)) {
		SceneManager::ChangeScene("TITLE");
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager::ChangeScene("TITLE");
	}

	Blinkcount++;
	if (Blinkcount > 90)
	{
		Blinkcount = 0;
	}
}

void GameOverScene::Draw()
{
	DrawGraph(0, 0, GameOverBg, TRUE);
	if (Blinkcount < 60)
	{
		SetFontSize(60);
		DrawString(600, 500, "Push [SPACE]Key To Title", GetColor(255, 255, 255));
	}
	SetFontSize(16);
}
