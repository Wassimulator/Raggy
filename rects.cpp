#pragma once
#include "raggy.hpp"
#include "source.cpp"

void LoadRects(int *WindowWidth, int *WindowHeight, int CamPosX, bool *F_Key, rects *R,
               player *Player,
               map Map,
               fart PlayerFart,
               fartCloud *PlayerFartCloud,
               door *DT,
               npc NPC[MaxNPCs],
               maptile *MapTile) // Do I need a pointer here for the array since the array is a pointer?
{
    //animation sequence
    int SixCounterP = (Player->i - 1) % 3;
    int SixCOunterQ = (Player->i - 1) / 3;

    R->PlayerRect.x = (*WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16);
    R->PlayerRect.y = (*WindowHeight / 2) - 48;
    R->PlayerRect.w = 32 * 3; // TODO: Scaling is an inherent problem that needs fixing.
    R->PlayerRect.h = 32 * 3;

    //putting the map in the center of the screen:
    Map.PosX = -(Map.ActiveMap.w - *WindowWidth) / 2;
    Map.PosY = -(Map.ActiveMap.h - *WindowHeight) / 2;
    R->MapRect.h = Map.ActiveMap.h;
    R->MapRect.w = Map.ActiveMap.w;
    R->MapRect.x = ((*WindowWidth - Map.ActiveMap.w) / 2) - CamPosX;
    R->MapRect.y = Map.PosY;

    for (Di = 0; Di < MaxDoors; Di++)
    {
        if (DT[Di].exists)
        {
            R->DoorRect[Di].h = DT[Di].Closed.h * 3;
            R->DoorRect[Di].w = DT[Di].Closed.w * 3;
            R->DoorRect[Di].x = (*WindowWidth / 2) - CamPosX - 48 - (Map.ActiveMap.w / 2) + 100 + 300 * (Di);
            R->DoorRect[Di].y = (*WindowHeight / 2) - 96;
        }
    }

    int limitcounter = 0;
    for (int i = 0; i < MaxTiles; i++)
    {
        if (MapTile[i].exists)
        {
            ++limitcounter;
        }
    }
    int MapSizeFromTiles = limitcounter * 32 * 3;
    for (int i = 0; i < MaxTiles; i++)
    {
        if (MapTile[i].exists)
        {
            R->MapTileRect[i].h = MapTile[i].ActiveTexture->h * 3;
            R->MapTileRect[i].w = MapTile[i].ActiveTexture->w * 3;
            R->MapTileRect[i].x = (*WindowWidth / 2) - CamPosX - (MapSizeFromTiles / 2) + 32 * 3 * (i);
            R->MapTileRect[i].y = (*WindowHeight / 2) - (MapTile[i].ActiveTexture->h / 2 * 3);
        }
    }

    R->PlayerActiveRectangle.x = SixCounterP * 32;
    R->PlayerActiveRectangle.y = SixCOunterQ * 32;
    R->PlayerActiveRectangle.w = R->PlayerActiveRectangle.h = 32;

    R->PlayerFartRectR.x = (*WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16) - 38;
    R->PlayerFartRectR.y = (*WindowHeight / 2) - 48 + 25;
    R->PlayerFartRectR.w = 32 * 2;
    R->PlayerFartRectR.h = 32 * 2;

    R->PlayerFartRectL.x = (*WindowWidth / 2) + (Player->PosX - CamPosX) - (3 * 16) + 70;
    R->PlayerFartRectL.y = (*WindowHeight / 2) - 48 + 25;
    R->PlayerFartRectL.w = 32 * 2;
    R->PlayerFartRectL.h = 32 * 2;

    R->PlayerFartActiveRect.x = (PlayerFart.i - 1) * 32;
    R->PlayerFartActiveRect.y = 0;
    R->PlayerFartActiveRect.w = R->PlayerFartActiveRect.h = 32;

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

    int PlayerPosition = R->PlayerRect.x + CamPosX; // This the player's actual position on the map

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
            R->PlayerFartCloudRect[FCi].x = PlayerFartCloud[FCi].FartCloudInitX - CamPosX;
            R->PlayerFartCloudRect[FCi].y = (*WindowHeight / 2) - PlayerFartCloud[FCi].iY - 24;
            R->PlayerFartCloudRect[FCi].w = R->PlayerFartCloudRect[FCi].h = 32 * 2;

            R->PlayerFartCloudActiveRect[FCi].x = (PlayerFartCloud[FCi].i - 1) * 32;
            R->PlayerFartCloudActiveRect[FCi].y = 0;
            R->PlayerFartCloudActiveRect[FCi].h = R->PlayerFartCloudActiveRect[FCi].w = 32;
        }
    }

    R->NPCRect.h = NPC[0].LeaningLeft.h * 3;
    R->NPCRect.w = NPC[0].LeaningLeft.w * 3;
    R->NPCRect.x = (*WindowWidth / 2) - CamPosX - 48 + 650;
    R->NPCRect.y = (*WindowHeight / 2) - 52;
};
