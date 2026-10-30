#pragma once
#include "../Library/SceneBase.h"

enum class STATE
{
	INIT = 0,  //初期化
	MONO,      //夜の背景
	CLEAR,     //クリアー
	CONTINUE,  //ステート継続
	END,       //ステート終了
	LAG        //モノローグとクリアー間のラグ
};

class ClearScene : public SceneBase
{
public:
	ClearScene();
	~ClearScene();
	void Update() override;
	void Draw() override;
	STATE InIt();
	STATE Mono();
	STATE Lag();

public:
	void SetEndMono() { subState = STATE::END; }

private:
	STATE mainState;
	STATE subState;
	float lagTime;
	int Blinkcount;

private:
	int ClearBg = LoadGraph("data/image/gameclear.jpg");;
	int Night = LoadGraph("data/image/viewStar.jpg");
	int Cong = LoadGraph("data/image/CongText.png");
};

