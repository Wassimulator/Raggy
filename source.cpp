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
    float PosX = 400 - 48;
    float PosY = 320 - 48;
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

void PlayerUpdate(player *Player, bool RightButton, bool LeftButton, bool UpButton, bool DownButton, bool LeftShift)
{
    Player->Speed = 1;
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

    if (RightButton && Player->PosX < 600 && Player->PosX > 100)
    {
        Player->ActiveTexture = &Player->RightAnimation;
        dx++;
        Player->Direction = RightDirection;
    }
    if (RightButton && Player->PosX == 599)
    {
        Player->ActiveTexture = &Player->RightAnimation;
        Player->PosX = 598;
        Player->Direction = RightDirection;
    }
    if (RightButton && LeftShift && Player->PosX < 597 && Player->PosX > 103)
    {
        Player->ActiveTexture = &Player->RightRunAnimation;
        dx++;
        Player->Speed = 3.5;
        Player->Direction = RightDirection;
    }
    if (RightButton && LeftShift && Player->PosX <= 599 && Player->PosX >= 597)
    {
        Player->ActiveTexture = &Player->RightRunAnimation;
        Player->PosX = 598;
        Player->Direction = RightDirection;
    }
    if (LeftButton && Player->PosX < 600 && Player->PosX > 100)
    {
        Player->ActiveTexture = &Player->LeftAnimation;
        dx--;
        Player->Direction = LeftDirection;
    }
    if (LeftButton && Player->PosX == 101)
    {
        Player->ActiveTexture = &Player->LeftAnimation;
        Player->PosX = 102;
        Player->Direction = LeftDirection;
    }
    if (LeftButton && LeftShift && Player->PosX < 597 && Player->PosX > 103)
    {
        Player->ActiveTexture = &Player->LeftRunAnimation;
        dx--;
        Player->Speed = 3.5;
        Player->Direction = LeftDirection;
    }
    if (LeftButton && LeftShift && Player->PosX <= 103 && Player->PosX >= 101)
    {
        Player->ActiveTexture = &Player->LeftRunAnimation;
        Player->PosX = 102;
        Player->Direction = LeftDirection;
    }
    /*if (UpButton)
    {
        //TODO: Draw them
        dy--;
    }
    if (DownButton)
    {
        //TODO: Draw them, Asshole.
        dy++;
    }*/

    //sum mafs
    float dl = sqrtf(dx * dx + dy * dy);
    if (dl != 0)
    {
        dx = dx / dl;
        dy = dy / dl;
        //printf("dx=%f, dy=%f dl=%f\n", dx, dy, dl);
        //printf("PosX=%f, PosY=%f\n", Player->PosX, Player->PosY);
        Player->PosX = Player->PosX + dx * Player->Speed;
        Player->PosY = Player->PosY + dy * Player->Speed;
    }
    if (!RightButton && !LeftButton /*&& !UpButton && !DownButton*/)
    {
        //Player->ActiveTexture = &Player->Idle
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

//-------------------------MAPS-------------------------------------------------------


map LoadMap()
{
    map LoadedMap = {};
    LoadedMap.ActiveMap = LoadSprite("textures/map.png");

    return LoadedMap;
}

void MapUpdate(map *Map, player *Player, bool RightButton, bool LeftButton, bool UpButton, bool DownButton, bool LeftShift)
{
    Map->Speed = 1;
    float dx = 0;
    float dy = 0;

    if (RightButton && Player->PosX == 599)
    {
        dx--;
    }
    if (RightButton && LeftShift && Player->PosX <= 599 && Player->PosX >= 597 && Map->PosX != -2299 && Map->PosX != -2298 && Map->PosX != -2297)
    {
        dx--;
        Map->Speed = 3.5;
    }
    if (LeftButton && Player->PosX == 101)
    {
        dx++;
    }
    if (LeftButton && LeftShift && Player->PosX <= 103 && Player->PosX >= 101 && Map->PosX != 99 && Map->PosX != 98 && Map->PosX != 97)
    {
        dx++;
        Map->Speed = 3.5;
    }

    if (LeftButton && Map->PosX == 99)
    {
        Map->PosX = 98;
    }
    if (LeftButton && LeftShift && Map->PosX <= 99 && Map->PosX >= 96)
    {
        Map->PosX = 97;
    }

    if (RightButton && Map->PosX == 2299)
    {
        Map->PosX = -2297;
    }
    if (RightButton && LeftShift && Map->PosX <= -2296 && Map->PosX >= -2300)
    {

        Map->PosX = -2297;
    }

    float dl = sqrtf(dx * dx + dy * dy);
    if (dl != 0)
    {
        dx = dx / dl;
        dy = dy / dl;

        Map->PosX = Map->PosX + dx * Map->Speed;
        Map->PosY = Map->PosY + dy * Map->Speed;
    }
    //printf("PosX Map = %f\n", Map->PosX);
}
