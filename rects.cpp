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
               SDL_Rect *PlayerFartCloudActiveRect,
               SDL_Rect *DTRect, door *DT,
               SDL_Rect *AholeRect, npc Ahole) // Do I need a pointer here for the array since the array is a pointer?
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
    DoorRect->x = (WindowWidth / 2) - CamPosX - 48;
    DoorRect->y = (WindowHight / 2) - 96;

    for (DTi = 0; DTi < 10; DTi++)
    {
        DTRect[DTi].h = DT[DTi].Closed.h * 3;
        DTRect[DTi].w = DT[DTi].Closed.w * 3;
        DTRect[DTi].x = (WindowWidth / 2) - CamPosX - 48 - 1300 + 300 * (DTi);
        DTRect[DTi].y = (WindowHight / 2) - 96;
    }

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

    for (FCi = 0; FCi < 20; FCi++) //update all the cloud's positions as they all need to be rising with time
    {
        if (PlayerFartCloud[FCi].T % 20 == 0)
        {
            if (PlayerFartCloud[FCi].iY >= 0 && PlayerFartCloud[FCi].iY < 64)
            {
                PlayerFartCloud[FCi].iY++;
            }
            if (PlayerFartCloud[FCi].iY == 64)
            {
                PlayerFartCloud[FCi].iY = 65;
            }
        }
    }

    int PlayerPosition = PlayerRect->x + CamPosX; // This the player's actual position on the map

    const int MaxFartClouds = 20;
    {
        if (*F_Key == true)
        {
            if (Player->Direction == RightDirection)
            {
                PlayerFartCloud[FCiw].FartCloudInitX = PlayerPosition - 50;
            }
            else if (Player->Direction == LeftDirection)
            {
                PlayerFartCloud[FCiw].FartCloudInitX = PlayerPosition + 80;
            }
            PlayerFartCloud[FCiw].iY = 0;
            if (FCiw == 20)
            {
                FCiw = 0;
            }
            if (FCiw != 20)
            {
                FCiw++; // increment the write index
            }
        }

        for (FCi = 0; FCi < 20; FCi++)
        {
            PlayerFartCloudRect[FCi].x = PlayerFartCloud[FCi].FartCloudInitX - CamPosX;
            PlayerFartCloudRect[FCi].y = (WindowHight / 2) - PlayerFartCloud[FCi].iY - 24;
            PlayerFartCloudRect[FCi].w = PlayerFartCloudRect[FCi].h = 32 * 2;

            PlayerFartCloudActiveRect[FCi].x = (PlayerFartCloud[FCi].i - 1) * 32;
            PlayerFartCloudActiveRect[FCi].y = 0;
            PlayerFartCloudActiveRect[FCi].h = PlayerFartCloudActiveRect[FCi].w = 32;
        }
    }

    AholeRect->h = Ahole.LeftLeaning.h * 3;
    AholeRect->w = Ahole.LeftLeaning.w * 3;
    AholeRect->x = (WindowWidth / 2) - CamPosX - 48 + 650;
    AholeRect->y = (WindowHight / 2) - 52;
};
