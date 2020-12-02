#pragma once
#include "raggy.hpp"

struct sprite
{
    int w, h, n;
    Uint8 *Pixels;
    SDL_Surface *Surface;
};

sprite LoadSprite(char *filename)
{
    sprite Result = {};
    Result.Pixels = stbi_load(filename, &Result.w, &Result.h, &Result.n, 0);
    Result.Surface = SDL_CreateRGBSurfaceWithFormatFrom(
        Result.Pixels, Result.w, Result.h, Result.n,
        Result.w * Result.n, SDL_PIXELFORMAT_RGBA32);
    return Result;
}

//--------SOUNDS-----------
struct sounds
{
    Mix_Chunk *Yo;
    Mix_Chunk *Fart;
};

sounds LoadSound()
{
    sounds Sound = {};
    Sound.Yo = Mix_LoadWAV("data/sounds/yo.wav");
    Sound.Fart = Mix_LoadWAV("data/sounds/fart.wav");

    return Sound;
}

//---------FONTS-------------
struct font
{
    int r;
    int g;
    int b;
    TTF_Font *Font;
    char *filename = "data/fonts/PTSans-Regular.ttf";

    SDL_Surface *TextSurface;
    SDL_Color TextColor;
};

void RenderTextCentered(TTF_Font *Font, char *text, Uint8 R, Uint8 G, Uint8 B,
                int PosXfromCenter, int PosYfromCenter, SDL_Surface *TextSurface, SDL_Surface *WindowSurface, 
                int WindowWidth, int WindowHight)
{

    SDL_Color TextColor = {R, G, B};
    TextSurface = TTF_RenderText_Solid(Font, text, TextColor);

    SDL_Rect TextRect1;
    TextRect1.h = TextSurface->h;
    TextRect1.w = TextSurface->w;
    TextRect1.x = ((WindowWidth - TextSurface->w) / 2) + PosXfromCenter;
    TextRect1.y = WindowHight / 2 + PosYfromCenter;
    SDL_BlitSurface(TextSurface, 0, WindowSurface, &TextRect1);

    SDL_FreeSurface(TextSurface);
}

void RenderText(TTF_Font *Font, char *text, Uint8 R, Uint8 G, Uint8 B,
                int PosX, int PosY, SDL_Surface *TextSurface, SDL_Surface *WindowSurface, 
                int WindowWidth, int WindowHight)
{

    SDL_Color TextColor = {R, G, B};
    TextSurface = TTF_RenderText_Solid(Font, text, TextColor);

    SDL_Rect TextRect1;
    TextRect1.h = TextSurface->h;
    TextRect1.w = TextSurface->w;
    TextRect1.x = PosX;
    TextRect1.y = PosY;
    SDL_BlitSurface(TextSurface, 0, WindowSurface, &TextRect1);

    SDL_FreeSurface(TextSurface);
}

//-----------------------------ACTOR STRUCTS---------------------------------------------
enum direction
{
    RightDirection,
    LeftDirection
};

struct player
{
    sprite LeftAnimation;
    sprite LeftRunAnimation;
    sprite RightAnimation;
    sprite RightRunAnimation;
    sprite IdleRight;
    sprite IdleLeft;
    float PosX;
    float PosY;
    sprite *ActiveTexture; //WHY is this a pointer??????????????
    int i;
    int T;
    int Speed;
    direction Direction;
};

struct map
{
    sprite ActiveMap;
    float PosX;
    float PosY;
    //int Speed; no longer necessary
};

enum doorStatus
{
    Closed,
    Open
};
struct door
{
    sprite Closed;
    sprite Open;
    float PosX;
    float PosY;
    sprite *ActiveTexture;
    doorStatus Status;
    int t;
};
//-----------------things----------------------
door LoadDoor()
{
    door Door = {};

    Door.Closed = LoadSprite("data/textures/door_closed.png");
    Door.Open = LoadSprite("data/textures/door_open.png");
    Door.Status = Closed;
    return Door;
}

void DoorUpdate(door *Door, SDL_Rect PlayerRect, SDL_Rect DoorRect, TTF_Font *Font, SDL_Surface *TextSurface,
                SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    if (Door->Status == Closed)
    {
        Door->ActiveTexture = &Door->Closed;
    }
    else if (Door->Status == Open)
    {
        Door->ActiveTexture = &Door->Open;
    }

    if (Door->Status == Closed)
    {
        if (PlayerRect.x < (DoorRect.x + 48) && PlayerRect.x > (DoorRect.x - 48))
        {
            RenderTextCentered(Font, "Door: press E to Open", 255, 255, 255, 0, 140, TextSurface, WindowSurface, WindowWidth, WindowHight);

            if (E_Key)
            {
                Door->ActiveTexture = &Door->Open;
                Door->Status = Open;
            }
        }
    }
    else if (Door->Status == Open)
    {
        if (PlayerRect.x < (DoorRect.x + 48) && PlayerRect.x > (DoorRect.x - 48))
        {
            RenderTextCentered(Font, "Door: press E to Close", 255, 255, 255, 0, 140, TextSurface, WindowSurface, WindowWidth, WindowHight);

            if (E_Key)
            {
                Door->ActiveTexture = &Door->Closed;
                Door->Status = Closed;
            }
        }
    }

}

//-----------------PLAYER------------------------------------------------------------------------

player LoadPlayer()
{
    player Player = {}; // it sets everything to zero, so Direction would be 0 which is RightDirection based on the enum
    //to be sure we could set it :
    Player.Direction = RightDirection;
    Player.IdleRight = LoadSprite("data/textures/player_right.png");
    Player.IdleLeft = LoadSprite("data/textures/player_left.png");
    Player.LeftAnimation = LoadSprite("data/textures/player_walk_left.png");
    Player.LeftRunAnimation = LoadSprite("data/textures/player_run_left.png");
    Player.RightAnimation = LoadSprite("data/textures/player_walk_right.png");
    Player.RightRunAnimation = LoadSprite("data/textures/player_run_right.png");

    return Player;
};

void PlayerUpdate(player *Player, float CamPosX,
                  bool RightButton, bool LeftButton, bool UpButton,
                  bool DownButton, bool Shift,
                  float MapLimitL, float MapLimitR,
                  int WalkSpeed, int RunSpeed)
{
    float dx = 0;
    float dy = 0;

    if (Player->Direction == RightDirection)
    {
        Player->ActiveTexture = &Player->IdleRight;
    }
    else if (Player->Direction == LeftDirection)
    {
        Player->ActiveTexture = &Player->IdleLeft;
    }

    //TODO: fix the idle guy in Pyxel

    Player->Speed = WalkSpeed;
    if (Shift)
    {
        Player->Speed = RunSpeed;
    }

    //RIGHT
    if (RightButton)
    {
        dx++;
    }

    //LEFT
    if (LeftButton)
    {
        dx--;
    }

    bool IsIdle = true;

    if (dx > 0)
    {
        IsIdle = false;

        Player->Direction = RightDirection;
        Player->ActiveTexture = &Player->RightAnimation;
        if (Shift)
        {
            Player->ActiveTexture = &Player->RightRunAnimation;
        }
    }
    else if (dx < 0)
    {
        IsIdle = false;

        Player->Direction = LeftDirection;
        Player->ActiveTexture = &Player->LeftAnimation;
        if (Shift)
        {
            Player->ActiveTexture = &Player->LeftRunAnimation;
        }
    }

    //sum mafs
    float dl = sqrtf(dx * dx + dy * dy);
    if (dl != 0)
    {
        dx = dx / dl;
        dy = dy / dl;
        Player->PosX = Player->PosX + dx * Player->Speed;
        Player->PosY = Player->PosY + dy * Player->Speed;
    }

    if (Player->PosX < MapLimitL)
    {
        Player->PosX = MapLimitL;
    }
    else if (Player->PosX > MapLimitR)
    {
        Player->PosX = MapLimitR;
    }

    if (IsIdle)
    {
        Player->i = 1;
    }
    else if (Player->T++ % 10 == 0)
    {
        if (Player->i >= 1 && Player->i < 6)
        {
            Player->i++;
        }
        else
        {
            Player->i = 1;
        }
    }
}

void PlayerSoundUpdate(sounds Sound, bool F_Key, bool H_Key)
{
    if (Mix_Playing(1) == 0)
    {
        if (F_Key)
        {
            Mix_PlayChannel(1, Sound.Fart, 0);
        }
        if (H_Key)
        {
            Mix_PlayChannel(1, Sound.Yo, 0);
        }
    }
}

//-------------------------MAPS-------------------------------------------------------

map LoadMap()
{
    map LoadedMap = {};
    LoadedMap.ActiveMap = LoadSprite("data/textures/map.png");

    return LoadedMap;
}

void MapUpdate(float *CamPosX, player *Player)
{
    float PlayerCamX = Player->PosX - *CamPosX;

    float HalfRange = 300.0f;

    if (PlayerCamX < -HalfRange)
    {
        *CamPosX = Player->PosX + HalfRange;
    }
    else if (PlayerCamX > HalfRange)
    {
        *CamPosX = Player->PosX - HalfRange;
    }
    //printf("Player->Posx = %.0f  ", Player->PosX);
    //printf("PlayerCamX = %.0f  ", PlayerCamX);
    //printf("HalfRange = %.0f  ", HalfRange);
}
