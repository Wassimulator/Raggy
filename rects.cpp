#pragma once
#include "raggy.hpp"
#include "source.cpp"

void LoadRects(int WindowWidth, int WindowHight, int CamPosX,
               SDL_Rect *PlayerRect, player Player,
               SDL_Rect *PlayerActiveRectangle,
               SDL_Rect *MapRect, map Map,
               SDL_Rect *DoorRect, door Door,
               SDL_Rect *PlayerFartRectR, fart PlayerFart,
               SDL_Rect *PlayerFartRectL,
               SDL_Rect *PlayerFartActiveRect)
{
    //animation sequence
    int SixCounterP = (Player.i - 1) % 3;
    int SixCOunterQ = (Player.i - 1) / 3;

    PlayerRect->x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16);
    PlayerRect->y = (WindowHight / 2) - 48;
    PlayerRect->w = 32 * 3; // TODO: Scaling is an inherent problem that needs fixing.
    PlayerRect->h = 32 * 3;

    MapRect->h = Map.ActiveMap.h;
    MapRect->w = Map.ActiveMap.w;
    MapRect->x = ((WindowWidth - Map.ActiveMap.w) / 2) - CamPosX;
    MapRect->y = Map.PosY;

    DoorRect->h = Door.Closed.h * 3;
    DoorRect->w = Door.Closed.w * 3;
    DoorRect->x = (WindowWidth / 2) - CamPosX - 48 - 200;
    DoorRect->y = (WindowHight / 2) - 96;

    PlayerActiveRectangle->x = SixCounterP * 32;
    PlayerActiveRectangle->y = SixCOunterQ * 32;
    PlayerActiveRectangle->w = PlayerActiveRectangle->h = 32;

    PlayerFartRectR->x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16) - 38;
    PlayerFartRectR->y = (WindowHight / 2) - 48 + 25;
    PlayerFartRectR->w = 32 * 2;
    PlayerFartRectR->h = 32 * 2;

    PlayerFartRectL->x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16) + 70;
    PlayerFartRectL->y = (WindowHight / 2) - 48 + 25;
    PlayerFartRectL->w = 32 * 2;
    PlayerFartRectL->h = 32 * 2;

    PlayerFartActiveRect->x = (PlayerFart.i - 1) * 32;
    PlayerFartActiveRect->y = 0;
    PlayerFartActiveRect->w = PlayerFartActiveRect->h = 32;
};