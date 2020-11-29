

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

struct player
{
    sprite LeftAnimation;
    sprite RightAnimation;
    sprite Idle;
    float PosX;
    float PosY;
    sprite *ActiveTexture;
    int i;
    int T;
    int Speed;
};

player LoadPlayer()
{
    player Player = {};
    Player.Idle = LoadSprite("textures/player.png");
    Player.LeftAnimation = LoadSprite("textures/player_walk_left.png");
    Player.RightAnimation = LoadSprite("textures/player_walk_right.png");

    return Player;
};

void PlayerUpdate(player *Player, bool RightButton, bool LeftButton, bool UpButton, bool DownButton)
{
    Player->Speed = 1;
    float dx = 0;
    float dy = 0;

    Player->ActiveTexture = &Player->Idle;
    //TODO: fix the idle guy in Pyxel

    if (RightButton)
    {
        Player->ActiveTexture = &Player->RightAnimation;
        dx++;
    }
    if (LeftButton)
    {
        Player->ActiveTexture = &Player->LeftAnimation;
        dx--;
    }
    if (UpButton)
    {
        //TODO: Draw them
        dy--;
    }
    if (DownButton)
    {
        //TODO: Draw them, Asshole.
        dy++;
    }
    //sum mafs
    float dl = sqrtf(dx * dx + dy * dy);
    if (dl != 0)
    {
        dx = dx / dl;
        dy = dy / dl;
        printf("dx=%f, dy=%f dl=%f\n", dx, dy, dl);
        Player->PosX = Player->PosX + dx * Player->Speed;
        Player->PosY = Player->PosY + dy * Player->Speed;
    }
    if (!RightButton && !LeftButton && !UpButton && !DownButton)
    {
        Player->ActiveTexture = &Player->Idle;
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
