#include "DxLib.h"
#include "Game.h"
#include "Constants.h"
#include <algorithm>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //TUREでWindow、FALSEでフルスクリーン
    ChangeWindowMode(TRUE);

    SetGraphMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, 32);

    const int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
    const int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

    const double scaleX = static_cast<double>(monitorWidth - 40) / Constants::SCREEN_WIDTH;
    const double scaleY = static_cast<double>(monitorHeight - 100) / Constants::SCREEN_HEIGHT;

    const double scale = (std::min)({ 1.0, scaleX, scaleY });
    SetWindowSizeExtendRate(scale);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    Game game;

    if (!game.Initialize())
    {
        MessageBox(nullptr, TEXT("マップの読み込みに失敗しました"), TEXT("Error"), MB_OK);

        DxLib_End();

        return -1;
    }

    while (ProcessMessage() == 0)
    {
        game.Update();

        ClearDrawScreen();

        game.Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}