#include "ClearScene.h"
#include "Fader.h"
#include "Common.h"
#include "TextBoxClear.h"

ClearScene::ClearScene()
{
	Common* common = FindGameObject<Common>();
	common->StopHellMusic();
	Fader* fader = FindGameObject<Fader>();
	fader->FadeIn(2.0f);
	mainState = STATE::INIT;
	subState = STATE::INIT;
	new TextBoxClear();
	lagTime = 0.0f;
	Blinkcount = 0;
}

ClearScene::~ClearScene()
{
	//âÊëúÅEâπê∫ëfçﬁÇÃdelete
	DeleteGraph(ClearBg);
	DeleteGraph(Night);
	DeleteGraph(Cong);
}

void ClearScene::Update()
{
	Fader* fader = FindGameObject<Fader>();
	switch (mainState)
	{
	case STATE::INIT:
		if (ClearScene::InIt() == STATE::END)
		{
			mainState = STATE::MONO;
			subState = STATE::CONTINUE;
		}
		break;
	case STATE::MONO:
		if (ClearScene::Mono() == STATE::END)
		{
			mainState = STATE::LAG;
			subState = STATE::INIT;
		}
		break;
	case STATE::LAG:
		if (ClearScene::Lag() == STATE::END)
		{
			mainState = STATE::CLEAR;
		}
		break;
	case STATE::CLEAR:
		if (CheckHitKey(KEY_INPUT_O)) {
			SceneManager::ChangeScene("TITLE");
		}
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			SceneManager::ChangeScene("TITLE");
		}
		break;

	}

	Blinkcount++;
	if (Blinkcount > 90)
	{
		Blinkcount = 0;
	}
}

void ClearScene::Draw()
{
	switch (mainState)
	{
	case STATE::INIT:
		break;
	case STATE::MONO:
		DrawExtendGraph(0, 0, 1280, 720, Night, TRUE);
		DrawGraph(0, 0, Cong, TRUE);
		//DrawRectGraph(100, 100, 0, 0, 360, 360, Star_1, 0);

		break;
	case STATE::CLEAR:
		DrawGraph(0, 0, ClearBg, TRUE);
		if (Blinkcount < 60)
		{
			SetFontSize(60);
			DrawString(600, 500, "Push [SPACE]Key To Title", GetColor(255, 255, 255));
			SetFontSize(16);
		}
		break;
	}
}

STATE ClearScene::InIt()
{
	/*TextBox* textBox = FindGameObject<TextBox>();
	textBox->SetAfterEscape();*/
	TextBoxClear* textBox = FindGameObject<TextBoxClear>();
	textBox->SetAfterEscape();
	Common* common = FindGameObject<Common>();
	common->PlayClearMusic();
	return STATE::END;
}

STATE ClearScene::Mono()
{
	if (subState == STATE::END)
	{
		Fader* fader = FindGameObject<Fader>();
		fader->FadeOut(1.0f);
		return STATE::END;
	}
	return STATE::CONTINUE;
}

STATE ClearScene::Lag()
{
	lagTime += Time::DeltaTime();
	if (lagTime >= 1.0f)
	{
		Fader* fader = FindGameObject<Fader>();
		fader->FadeIn(1.0f);
		lagTime = 0.0f;
		return STATE::END;
	}
	return STATE::CONTINUE;
}

