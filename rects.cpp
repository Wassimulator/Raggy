#pragma once
#include "raggy.hpp"
#include "source.cpp"

void LoadRects(int WindowWidth, int WindowHight, int CamPosX, bool *F_Key,
               SDL_Rect *PlayerRect, player *Player,
               SDL_Rect *PlayerActiveRectangle,
               SDL_Rect *MapRect, map Map,
               SDL_Rect *DoorRect, door Door,
               SDL_Rect *PlayerFartRectR, fart PlayerFart,
               SDL_Rect *PlayerFartRectL,
               SDL_Rect *PlayerFartActiveRect,
               SDL_Rect *PlayerFartCloudRect, fartCloud *PlayerFartCloud,
               SDL_Rect *PlayerFartCloudActiveRect)
{
    //animation sequence
    int SixCounterP = (Player->i - 1) % 3;
    int SixCOunterQ = (Player->i - 1) / 3;

    PlayerRect->x = (WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16);
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

    PlayerFartRectR->x = (WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16) - 38;
    PlayerFartRectR->y = (WindowHight / 2) - 48 + 25;
    PlayerFartRectR->w = 32 * 2;
    PlayerFartRectR->h = 32 * 2;

    PlayerFartRectL->x = (WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16) + 70;
    PlayerFartRectL->y = (WindowHight / 2) - 48 + 25;
    PlayerFartRectL->w = 32 * 2;
    PlayerFartRectL->h = 32 * 2;

    PlayerFartActiveRect->x = (PlayerFart.i - 1) * 32;
    PlayerFartActiveRect->y = 0;
    PlayerFartActiveRect->w = PlayerFartActiveRect->h = 32;

    if (PlayerFartCloud[FartCloudReadIndex].T % 20 == 0)
    {
        if (PlayerFartCloud[FartCloudReadIndex].iY >= 0 && PlayerFartCloud[FartCloudReadIndex].iY < 64)
        {
            PlayerFartCloud[FartCloudReadIndex].iY++;
        }
        if (PlayerFartCloud[FartCloudReadIndex].iY == 64)
        {
            PlayerFartCloud[FartCloudReadIndex].iY = 65;
        }
    }

    int PlayerPosition = PlayerRect->x + CamPosX; // This the player's actual position on the map

    const int MaxFartClouds = 20;

    if (*F_Key == true)
    {
        if (Player->Direction == RightDirection)
        {
            PlayerFartCloud[FartCloudReadIndex].FartCloudInitX = PlayerPosition - 70;
        }
        else if (Player->Direction == LeftDirection)
        {
            PlayerFartCloud[FartCloudReadIndex].FartCloudInitX = PlayerPosition + 70;
        }
        PlayerFartCloud[FartCloudReadIndex].iY = 0;
    }
    for (FartCloudReadIndex = 1;
         FartCloudReadIndex < MaxFartClouds;
         FartCloudReadIndex++)
    {
        PlayerFartCloudRect[FartCloudReadIndex].x = PlayerFartCloud[FartCloudReadIndex].FartCloudInitX - CamPosX;
        PlayerFartCloudRect[FartCloudReadIndex].y = (WindowHight / 2) - PlayerFartCloud[FartCloudReadIndex].iY - 24;
        PlayerFartCloudRect[FartCloudReadIndex].w = PlayerFartCloudRect->h = 32 * 2;

        PlayerFartCloudActiveRect[FartCloudReadIndex].x = (PlayerFartCloud[FartCloudReadIndex].i - 1) * 32;
        PlayerFartCloudActiveRect[FartCloudReadIndex].y = 0;
        PlayerFartCloudActiveRect[FartCloudReadIndex].h = PlayerFartCloudActiveRect[FartCloudReadIndex].w = 32;
    }
    //  printf("initX = %i, CloudRectPosX = %i, PlayerRect->x = %i, CamPosX = %i\n",
    //         PlayerFartCloud->FartCloudInitX, PlayerFartCloudRect->x, PlayerRect->x, CamPosX);
};