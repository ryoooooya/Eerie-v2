#include "TextBoxClear.h"
#include "ClearScene.h"

TextBoxClear::TextBoxClear()
{
	MainState = ClearText::STATE_END;
	TalkState = ClearText::STATE_END;
}

TextBoxClear::~TextBoxClear()
{
	DeleteGraph(BoxImage);
	DeleteSoundMem(NEXT_TEXT_SOUND);
}

void TextBoxClear::Update()
{
	Common* common = FindGameObject<Common>();
	auto scene = dynamic_cast<ClearScene*>(SceneManager::CurrentScene());
	switch (MainState)
	{
		case ClearText::STATE_AFTER_ESCAPE://脱出後
		switch (TalkState)
		{
		case ClearText::STATE_STRING_1:
			if (common->GetLagCheck())
			{
				if (CheckHitKey(KEY_INPUT_SPACE))
				{
					PlaySoundMem(NEXT_TEXT_SOUND, DX_PLAYTYPE_BACK);
					TalkState = ClearText::STATE_STRING_2;
					common->SetLagIn_T();
				}
			}
			break;
		case ClearText::STATE_STRING_2:
			if (common->GetLagCheck())
			{
				if (CheckHitKey(KEY_INPUT_SPACE))
				{
					PlaySoundMem(NEXT_TEXT_SOUND, DX_PLAYTYPE_BACK);
					if (scene)
					{
						scene->SetEndMono();
					}
					MainState = ClearText::STATE_END;
					TalkState = ClearText::STATE_END;
					common->SetLagIn_T();
				}
			}
			break;
		default:
			break;
		}
		break;
	case ClearText::STATE_END://STATE終了
		break;
	default:
		break;
	}

}

void TextBoxClear::Draw()
{
	switch (MainState)
	{
	case ClearText::STATE_AFTER_ESCAPE:
				DrawRectGraph(BoxPosX, BoxPosY, 0, 0, 599, 180, BoxImage, 1);
		switch (TalkState)
		{
		case ClearText::STATE_STRING_1:
			DrawString(490, HintY, "「テストの回答を盗み出すことができた！」", GetColor(255, 255, 255), 1);
			break;
		case ClearText::STATE_STRING_2:
			DrawString(490, HintY, "「学校から脱出した」", GetColor(255, 255, 255), 1);
			break;
		default:
			break;
		}
	    break;
	case ClearText::STATE_END://STATE終了
		break;
	}
}
