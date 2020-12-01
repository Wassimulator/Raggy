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
enum direction
{
    RightDirection,
    LeftDirection
};

//-----------------------------STRUCTS---------------------------------------------

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
    sprite *ActiveTexture;
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
    int Speed;
};

struct sound
{
    char *file;
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
};
//-----------------PLAYER------------------------------------------------------------------------

player LoadPlayer()
{
    player Player = {}; // it sets everything to zero, so Direction would be 0 which is RightDirection based on the enum
    //to be sure we could set it :
    Player.Direction = RightDirection;
    Player.IdleRight = LoadSprite("textures/player_right.png");
    Player.IdleLeft = LoadSprite("textures/player_left.png");
    Player.LeftAnimation = LoadSprite("textures/player_walk_left.png");
    Player.LeftRunAnimation = LoadSprite("textures/player_run_left.png");
    Player.RightAnimation = LoadSprite("textures/player_walk_right.png");
    Player.RightRunAnimation = LoadSprite("textures/player_run_right.png");

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

    //Player->ActiveTexture = &Player->Idle;
    //TODO: fix the idle guy in Pyxel

    Player->Speed = WalkSpeed;
    if (Shift) {
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

    if (dx > 0) {
        IsIdle = false;

        Player->Direction = RightDirection;
        Player->ActiveTexture = &Player->RightAnimation;
        if (Shift) {
            Player->ActiveTexture = &Player->RightRunAnimation;
        }
    } else if (dx < 0) {
        IsIdle = false;

        Player->Direction = LeftDirection;
        Player->ActiveTexture = &Player->LeftAnimation;
        if (Shift) {
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

    if (Player->PosX < MapLimitL) {
        Player->PosX = MapLimitL;
    } else if (Player->PosX > MapLimitR) {
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

void PlayerSoundUpdate(Mix_Chunk *Sound, bool F_Key)
{
    if(Mix_Playing(1) == 0)
    {
        if (F_Key)
        {
            Mix_PlayChannel(1, Sound, 0);
        }
    }
}

//-------------------------MAPS-------------------------------------------------------

map LoadMap()
{
    map LoadedMap = {};
    LoadedMap.ActiveMap = LoadSprite("textures/map.png");

    return LoadedMap;
}

void MapUpdate(float *CamPosX, player *Player)
{
    float PlayerCamX = Player->PosX - *CamPosX;

    float HalfRange = 300.0f;

    if (PlayerCamX < -HalfRange) {
        *CamPosX = Player->PosX + HalfRange;
    } else if (PlayerCamX > HalfRange) {
        *CamPosX = Player->PosX - HalfRange;
    }
}
