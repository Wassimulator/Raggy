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

void RenderTextCenteredX(TTF_Font *Font, char *text, Uint8 R, Uint8 G, Uint8 B,
                         int PosXfromCenter, int PosYfromTop, SDL_Surface *TextSurface, SDL_Surface *WindowSurface,
                         int WindowWidth, int WindowHight)
{

    SDL_Color TextColor = {R, G, B};
    TextSurface = TTF_RenderText_Solid(Font, text, TextColor);

    SDL_Rect TextRect1;
    TextRect1.h = TextSurface->h;
    TextRect1.w = TextSurface->w;
    TextRect1.x = ((WindowWidth - TextSurface->w) / 2) + PosXfromCenter;
    TextRect1.y = PosYfromTop;
    SDL_BlitSurface(TextSurface, 0, WindowSurface, &TextRect1);

    SDL_FreeSurface(TextSurface);
}

void RenderTextDialogue(TTF_Font *Font, char *text, Uint8 R, Uint8 G, Uint8 B,
                        int PosX, int PosY, SDL_Surface *TextSurface, SDL_Surface *WindowSurface,
                        int WindowWidth, int WindowHight, Uint32 WrapLength)
{

    SDL_Color TextColor = {R, G, B};
    TextSurface = TTF_RenderText_Blended_Wrapped(Font, text, TextColor, WrapLength);

    SDL_Rect TextRect1;
    TextRect1.h = TextSurface->h;
    TextRect1.w = TextSurface->w;
    TextRect1.x = PosX;
    TextRect1.y = PosY;
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
    sprite *ActiveTexture;
    int i;
    int T;
    int Speed;
    direction Direction;
    bool Chatting = false;
    bool ChattingAhole = false;

    int AholeLevel = 0; // TODO: There should be a better way for tracking dialogue progress
};

struct npc
{
    sprite LeftLeaning;
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
};
struct fart
{
    sprite FartLeft;
    sprite FartRight;
    sprite Empty;
    sprite *ActiveTexture; //WHY is this a pointer??????????????
    int i;
    int T;
    bool ToFart;
    direction Direction;
};
struct fartCloud
{
    sprite FartCloud;
    bool HasFarted;
    int FartCloudInitX;
    int ReadIndex;
    int i;
    int iY;
    int aY;
    int T;
};
//-----------------things----------------------
fartCloud LoadFartCloud()
{
    fartCloud PlayerFartCloud = {};
    PlayerFartCloud.FartCloud = LoadSprite("data/textures/fart_cloud.png");
    PlayerFartCloud.FartCloudInitX = 0;
    PlayerFartCloud.ReadIndex = 0;
    return PlayerFartCloud;
}
fart LoadFart()
{
    fart PlayerFart = {};

    PlayerFart.FartLeft = LoadSprite("data/textures/fart_left.png");
    PlayerFart.FartRight = LoadSprite("data/textures/fart_right.png");
    PlayerFart.Empty = LoadSprite("data/textures/empty.png");
    PlayerFart.ToFart = false;

    return PlayerFart;
}
void FartUpdate(player *Player, fart *PlayerFart, fartCloud *PlayerFartCloud, bool F_Key)
{
    if (F_Key)
    {
        if (Player->Direction == RightDirection)
        {
            PlayerFart->ActiveTexture = &PlayerFart->FartRight;
        }
        if (Player->Direction == LeftDirection)
        {
            PlayerFart->ActiveTexture = &PlayerFart->FartLeft;
        }
        PlayerFart->ToFart = true;
        for (FCi = 0; FCi < 20; FCi++)
        {
            PlayerFartCloud[FCi].i = 2; //i is used for the animation sequence
        }
        if (FClength != 20)
        {
            FClength++; // increment the lenghth
        }
    }

    if (PlayerFart->T++ % 5 == 0 && PlayerFart->ToFart == true)
    {
        if (PlayerFart->i >= 0 && PlayerFart->i <= 3)
        {
            PlayerFart->i++;
        }
        if (PlayerFart->i > 3)
        {
            PlayerFart->ToFart = false;
            PlayerFart->i = 1;
            for (FCi = 0; FCi < 20; FCi++)
            {
                PlayerFartCloud[FCi].HasFarted = true;
            }
        }
    }
    for (FCi = 0; FCi < 20; FCi++) //updating all the i's simultaneously to make all clouds move in unison
    {
        if (PlayerFartCloud[FCi].T++ % 20 == 0)
        {
            if (PlayerFartCloud[FCi].i == 1)
            {
                PlayerFartCloud[FCi].i = 2;
            }
            else if (PlayerFartCloud[FCi].i == 2)
            {
                PlayerFartCloud[FCi].i = 1;
            }
        }
    }
}

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
            RenderText(Font, "Door: press E to Open", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);

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
            RenderText(Font, "Door: press E to Close", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);

            if (E_Key)
            {
                Door->ActiveTexture = &Door->Closed;
                Door->Status = Closed;
            }
        }
    }
}

void DTUpdate(door *DT, SDL_Rect PlayerRect, SDL_Rect *DTRect, TTF_Font *Font, SDL_Surface *TextSurface,
              SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    for (DTi = 0; DTi < 10; DTi++)
    {
        if (DT[DTi].Status == Closed)
        {
            DT[DTi].ActiveTexture = &DT[DTi].Closed;
        }
        else if (DT[DTi].Status == Open)
        {
            DT[DTi].ActiveTexture = &DT[DTi].Open;
        }

        if (DT[DTi].Status == Closed)
        {
            if (PlayerRect.x < (DTRect[DTi].x + 48) && PlayerRect.x > (DTRect[DTi].x - 48))
            {
                RenderText(Font, "Door: press E to Open", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);

                if (E_Key)
                {
                    DT[DTi].ActiveTexture = &DT[DTi].Open;
                    DT[DTi].Status = Open;
                }
            }
        }
        else if (DT[DTi].Status == Open)
        {
            if (PlayerRect.x < (DTRect[DTi].x + 48) && PlayerRect.x > (DTRect[DTi].x - 48))
            {
                RenderText(Font, "Door: press E to Close", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);

                if (E_Key)
                {
                    DT[DTi].ActiveTexture = &DT[DTi].Closed;
                    DT[DTi].Status = Closed;
                }
            }
        }
    }
}
//-----------------NPC---------------------------------------------------------------------------

void DialogueAhole(TTF_Font *Font, SDL_Surface *TextSurface,
                   SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    RenderTextCentered(Font, "Hey asshole", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);
}
npc LoadAhole()
{
    npc Ahole = {};
    Ahole.LeftLeaning = LoadSprite("data/textures/a_hole_leaning_left.png");
    return Ahole;
};

void AholeUpdate(player *Player, SDL_Rect PlayerRect, SDL_Rect *AholeRect, TTF_Font *Font, SDL_Surface *TextSurface,
                 SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    if (PlayerRect.x > (AholeRect->x - 60) && PlayerRect.x < (AholeRect->x + 10))
    {
        RenderText(Font, "Pricksoin Ahole: press E to Speak", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);
        if (E_Key)
        {
            Player->Chatting = true;
            Player->ChattingAhole = true;
            //RenderTextCentered(Font, "Hey asshole", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);
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

//---------------------Dialogues--------------------------
struct dialogueNPC
{
    sprite IdleView;
};

struct dialogues
{
    bool Ahole = false;
    char *OptionText[12];
    int MaxOptions = 12;
    char *DialogueTitle = "dialogue title";
    char *NPCtext = "placeholder for NPC text (what they say)";
    char *PlayerText = "placeholder for Player text (what he says)";
    bool HighlightedOption[12];
    bool SelectedOption[12];
    int ID;

    struct options
    {
        char *Text;
        dialogues *NextNode;
        int NextNodeID;
    };

    options Option[12];
    sprite View;
};

void UpdateOptionRects(SDL_Rect Option[12], SDL_Rect Options)
{
    Option[0].y = Options.y + 10;
    Option[1].y = Options.y + 10 + Option[0].h;
    Option[2].y = Options.y + 10 + Option[0].h + Option[1].h;
    Option[3].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h;
    Option[4].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h;
    Option[5].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h;
    Option[6].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h;
    Option[7].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h + Option[6].h;
    Option[8].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h + Option[6].h + Option[7].h;
    Option[9].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h + Option[6].h + Option[7].h + Option[8].h;
    Option[10].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h + Option[6].h + Option[7].h + Option[8].h + Option[9].h;
    Option[11].y = Options.y + 10 + Option[0].h + Option[1].h + Option[2].h + Option[3].h + Option[4].h + Option[5].h + Option[6].h + Option[7].h + Option[8].h + Option[9].h + Option[10].h;
}
//--------------------------------------------------------