#pragma once
#include <Siv3D.hpp>
#include "Game.hpp"

void Main()
{
    Game game;
    bool success = game.Initialize();
    if (success)
    {
        game.RunLoop();
    }
    game.Shutdown();
}
