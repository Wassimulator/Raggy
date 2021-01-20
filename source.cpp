#pragma once
#include "raggy.hpp"
using namespace std;

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

struct rects
{
    SDL_Rect PlayerRect;
    SDL_Rect MapRect;
    SDL_Rect PlayerActiveRectangle;
    SDL_Rect PlayerFartRectR;
    SDL_Rect PlayerFartRectL;
    SDL_Rect PlayerFartActiveRect;
    SDL_Rect PlayerFartCloudRect[20];
    SDL_Rect PlayerFartCloudActiveRect[20];
    SDL_Rect DoorRect[MaxDoors];
    SDL_Rect MapTileRect[MaxTiles];
    SDL_Rect NPCRect;
};

struct keys
{
    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Shift = false;
    bool F_Key = false;
    bool H_Key = false;
    bool E_Key = false;
    bool Tab_Key = false;
    bool Space_Key = false;
    bool Return_Key = false;
    bool F1_Key = false;
};

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

struct NPCsounds
{
    Mix_Chunk *Node[100];
};

//---------FONTS-------------
struct fonts
{
    TTF_Font *Regular = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 20);
    TTF_Font *RegularS = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 17);
    TTF_Font *Bold = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 20);
    TTF_Font *Bold2 = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 24);
    TTF_Font *Title1B = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 60);
    TTF_Font *Title1 = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 60);
    TTF_Font *Title2B = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 40);
    TTF_Font *Title2 = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 40);
    TTF_Font *Title3B = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 30);
    TTF_Font *Title3 = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 30);
    SDL_Surface *TextSurface;
};
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

void RenderTextCenteredX(TTF_Font *Font, string textstring, Uint8 R, Uint8 G, Uint8 B,
                         int PosXfromCenter, int PosYfromTop, SDL_Surface *TextSurface, SDL_Surface *WindowSurface,
                         int WindowWidth, int WindowHight)
{

    SDL_Color TextColor = {R, G, B};

    const char *temp = textstring.c_str();
    char *text = const_cast<char *>(temp);

    TextSurface = TTF_RenderText_Solid(Font, text, TextColor);

    SDL_Rect TextRect1;
    TextRect1.h = TextSurface->h;
    TextRect1.w = TextSurface->w;
    TextRect1.x = ((WindowWidth - TextSurface->w) / 2) + PosXfromCenter;
    TextRect1.y = PosYfromTop;
    SDL_BlitSurface(TextSurface, 0, WindowSurface, &TextRect1);

    SDL_FreeSurface(TextSurface);
}

void RenderTextDialogue(TTF_Font *Font, string textstring, Uint8 R, Uint8 G, Uint8 B,
                        int PosX, int PosY, SDL_Surface *TextSurface, SDL_Surface *WindowSurface,
                        int WindowWidth, int WindowHight, Uint32 WrapLength)
{

    SDL_Color TextColor = {R, G, B};

    const char *temp = textstring.c_str();
    char *text = const_cast<char *>(temp);

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
    bool ChattingNPC = false;
};

struct npc
{
    sprite LeaningLeft;
};

struct map
{
    sprite ActiveMap;
    float PosX;
    float PosY;
    //int Speed; no longer necessary
};
enum maptileType
{
    Start,
    Middle,
    End
};
struct maptile
{
    sprite Start, Middle, End;
    sprite *ActiveTexture;
    maptileType Type;
    float PosX;
    float PosY;
    bool exists = true;
};

struct levelInfo
{
    int Length;
    int Texture_Type = 0;
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
    bool exists = true;
    int next;
    int nextDoor;
    int previous;
    int previousDoor;
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
maptile LoadTile(int levelNumber)
{
    maptile result = {};
    char Start[40];
    char Middle[40];
    char End[40];
    sprintf(Start, "data/textures/levels/%i_start.png", levelNumber);
    sprintf(Middle, "data/textures/levels/%i_middle.png", levelNumber);
    sprintf(End, "data/textures/levels/%i_end.png", levelNumber);

    result.Start = LoadSprite(Start);
    result.Middle = LoadSprite(Middle);
    result.End = LoadSprite(End);

    return result;
}

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

void DoorsUpdate(door *DT, rects *R, TTF_Font *Font, SDL_Surface *TextSurface,
                 SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    for (Di = 0; Di < MaxDoors; Di++)
    {
        if (DT[Di].exists)
        {
            if (DT[Di].Status == Closed)
            {
                DT[Di].ActiveTexture = &DT[Di].Closed;
            }
            else if (DT[Di].Status == Open)
            {
                DT[Di].ActiveTexture = &DT[Di].Open;
            }

            if (DT[Di].Status == Closed)
            {
                if (R->PlayerRect.x < (R->DoorRect[Di].x + 48) && R->PlayerRect.x > (R->DoorRect[Di].x - 48))
                {
                    RenderText(Font, "Door: press E to Open", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);

                    if (E_Key)
                    {
                        DT[Di].ActiveTexture = &DT[Di].Open;
                        DT[Di].Status = Open;
                        CurrentMap = DT[Di].next;
                        ToUpdateMap = true;
                        ToUpdateMapRects = true;
                    }
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
npc LoadNPC(int NPCnumber)
{
    npc NPC = {};
    char LeaningLeft[50];
    sprintf(LeaningLeft, "data/textures/npc/npc%i_leaning_left.png", NPCnumber);
    NPC.LeaningLeft = LoadSprite(LeaningLeft);
    return NPC;
};

void NPCUpdate(player *Player, rects *R, TTF_Font *Font, SDL_Surface *TextSurface,
               SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    if (R->PlayerRect.x > (R->NPCRect.x - 60) && R->PlayerRect.x < (R->NPCRect.x + 10))
    {
        RenderText(Font, "Pricksoin Ahole: press E to Speak", 255, 255, 255, 0, 150, TextSurface, WindowSurface, WindowWidth, WindowHight);
        if (E_Key)
        {
            Player->Chatting = true;
            Player->ChattingNPC = true;
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
                  keys *K,
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
    if (K->Shift)
    {
        Player->Speed = RunSpeed;
    }

    //RIGHT
    if (K->RightButton)
    {
        dx++;
    }

    //LEFT
    if (K->LeftButton)
    {
        dx--;
    }

    bool IsIdle = true;

    if (dx > 0)
    {
        IsIdle = false;

        Player->Direction = RightDirection;
        Player->ActiveTexture = &Player->RightAnimation;
        if (K->Shift)
        {
            Player->ActiveTexture = &Player->RightRunAnimation;
        }
    }
    else if (dx < 0)
    {
        IsIdle = false;

        Player->Direction = LeftDirection;
        Player->ActiveTexture = &Player->LeftAnimation;
        if (K->Shift)
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

void PlayerSoundUpdate(sounds Sound, keys *K)
{
    if (Mix_Playing(1) == 0)
    {
        if (K->F_Key)
        {
            Mix_PlayChannel(1, Sound.Fart, 0);
        }
        if (K->H_Key)
        {
            Mix_PlayChannel(1, Sound.Yo, 0);
        }
    }
}

//-------------------------MAPS-------------------------------------------------------

map LoadMap(int LevelNumber)
{
    map LoadedMap = {};
    char MapFile[50];
    sprintf(MapFile, "data/textures/levels/level%i.png", LevelNumber);
    LoadedMap.ActiveMap = LoadSprite(MapFile);

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
}

/*
fckn why
1280,112  380
1024,240  252
960,272   220  
800,352   40  
520
260

960, 288
800, 368
*/

//---------------------Dialogues--------------------------
struct dialogueNPC
{
    sprite IdleView;
    sprite TalkView;
};

struct dialogues
{
    bool Ahole = false;
    char *OptionText[12];
    int MaxOptions = 12;
    string DialogueTitle = "dialogue title";
    string NPCtext = "placeholder for NPC text (what they say)";
    string PlayerText = "placeholder for Player text (what he says)";
    bool HighlightedOption[12];
    bool SelectedOption[12];
    int ID;

    struct options
    {
        string Text;
        dialogues *NextNode;
        int NextNodeID;
    };

    options Option[12];
    sprite View;
};

void UpdateOptionRects(SDL_Rect OptionRect[12], SDL_Rect Options, SDL_Rect OptionNumRect[12])
{
    OptionRect[0].y = Options.y + 10;
    OptionRect[1].y = Options.y + 10 + OptionRect[0].h;
    OptionRect[2].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h;
    OptionRect[3].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h;
    OptionRect[4].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h;
    OptionRect[5].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h;
    OptionRect[6].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h;
    OptionRect[7].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h + OptionRect[6].h;
    OptionRect[8].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h + OptionRect[6].h + OptionRect[7].h;
    OptionRect[9].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h + OptionRect[6].h + OptionRect[7].h + OptionRect[8].h;
    OptionRect[10].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h + OptionRect[6].h + OptionRect[7].h + OptionRect[8].h + OptionRect[9].h;
    OptionRect[11].y = Options.y + 10 + OptionRect[0].h + OptionRect[1].h + OptionRect[2].h + OptionRect[3].h + OptionRect[4].h + OptionRect[5].h + OptionRect[6].h + OptionRect[7].h + OptionRect[8].h + OptionRect[9].h + OptionRect[10].h;
}
//--------------------------------------------------------

//-----------------------------------------------Parser-----------------------------------
void RXT_ParseNode(stringstream *Input, dialogues *D)
{
    token T;
    T = Lexer_GetToken(Input);
    if (T.Type == OpenBrace)
    {
        int i;
        while (true)
        {
            T = Lexer_GetToken(Input);
            if (T.Type == Identifier)
            {
                if (T.Text == "NPCprompt")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Text)
                        {
                            D->NPCtext = T.Text;
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
                if (T.Text == "Title")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Text)
                        {
                            D->DialogueTitle = T.Text;
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
                if (T.Text == "Option")
                {
                    bool WithinOption = true;
                    while (WithinOption)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == OpenSquareBrace)
                        {
                            T = Lexer_GetToken(Input);
                            if (T.Type == Number)
                            {
                                i = stoi(T.Text);
                            }
                        }
                        if (T.Type == Text)
                        {
                            D->Option[i].Text = T.Text;
                        }
                        if (T.Type == SmallerThan)
                        {
                            while (true)
                            {
                                /////////////////////////copy this and paste afterwards for more modifiers
                                T = Lexer_GetToken(Input);
                                if (T.Type == Identifier)
                                {
                                    if (T.Text == "nextNode")
                                    {
                                        while (true)
                                        {
                                            T = Lexer_GetToken(Input);
                                            if (T.Type == Number)
                                            {
                                                D->Option[i].NextNodeID = stoi(T.Text);
                                            }
                                            if (T.Type == SemiColon)
                                            {
                                                break;
                                            }
                                        }
                                    }
                                    /////////////////////////////
                                }
                                if (T.Type == LargerThan)
                                {
                                    WithinOption = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if (T.Type == ClosedBrace)
            {
                D->MaxOptions = i + 1;
                break;
            }
        }
    }
}

void RXT_ReloadStream(stringstream *Input, string FileString)
{
    stringstream tempstream;
    Input->swap(tempstream);
    Input->str(FileString);
}

int RXT_ParseNextNode(int choice, dialogues *D, stringstream *Input, token *T, string FileString)
{
    string NextNode = to_string(D->Option[choice].NextNodeID);

    int NodeFound = false;
    RXT_ReloadStream(Input, FileString);
    while (true)
    {
        *T = Lexer_GetToken(Input);
        if (T->Type == Identifier)
        {
            if (T->Text == "Node")
            {
                *T = Lexer_GetToken(Input);
                if (T->Type == OpenSquareBrace)
                {
                    *T = Lexer_GetToken(Input);
                    if (T->Type == Number && T->Text == NextNode)
                    {
                        *T = Lexer_GetToken(Input);
                        RXT_ParseNode(Input, D);
                        NodeFound = true;
                        break;
                    }
                }
            }
            if (T->Text == "EOF")
            {
                break;
            }
        }
    }
    if (NodeFound)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void RXT_FindAndParseHomeNode(token *T, stringstream *Input, dialogues *Dialogue, string FileString)
{
    *T = Lexer_GetToken(Input);
    if (T->Type == Identifier)
    {
        if (T->Text == "Homenode")
        {
            RXT_ParseNode(Input, Dialogue);
            RXT_ReloadStream(Input, FileString);
        }
    }
}

//---------RMP-----------
void RMP_ParseLevel(stringstream *Input, levelInfo *LevelInfo)
{
    token T;
    T = Lexer_GetToken(Input);
    if (T.Type == OpenBrace)
    {
        int i;
        while (true)
        {
            T = Lexer_GetToken(Input);
            if (T.Type == Identifier)
            {
                if (T.Text == "Length")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Number)
                        {
                            LevelInfo->Length = stoi(T.Text);
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
                if (T.Text == "TextureType")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Number)
                        {
                            LevelInfo->Texture_Type = stoi(T.Text);
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
            }
            if (T.Type == ClosedBrace)
            {
                break;
            }
        }
    }
}

void RMP_ReloadStream(stringstream *Input, string FileString)
{
    stringstream tempstream;
    Input->swap(tempstream);
    Input->str(FileString);
}

int RMP_ParseNextLevel(levelInfo *LevelInfo, stringstream *Input, token *T, string FileString)
{
    string NextLevel = to_string(CurrentMap);

    bool LevelFound = false;
    RMP_ReloadStream(Input, FileString);
    while (true)
    {
        *T = Lexer_GetToken(Input);
        if (T->Type == Identifier)
        {
            if (T->Text == "Level")
            {
                *T = Lexer_GetToken(Input);
                if (T->Type == OpenSquareBrace)
                {
                    *T = Lexer_GetToken(Input);
                    if (T->Type == Number && T->Text == NextLevel)
                    {
                        *T = Lexer_GetToken(Input);
                        RMP_ParseLevel(Input, LevelInfo);
                        LevelFound = true;
                        break;
                    }
                }
            }
            if (T->Text == "EOF")
            {
                break;
            }
        }
    }
    if (LevelFound)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------

void UpdateMap(rects *R, map *Map, door *Door, float *CamPosX, int *WindowWidth,
               levelInfo *LevelInfo, stringstream *LevelInput, token *LevelToken, string LevelFileString)
{

    RMP_ParseNextLevel(LevelInfo, LevelInput, LevelToken, LevelFileString);

    *Map = LoadMap(CurrentMap);
    Map->ActiveMap.h = Map->ActiveMap.h * 3;
    Map->ActiveMap.w = Map->ActiveMap.w * 3;

    int TargetLevel;
    for (int i = 0; i < MaxDoors; i++)
    {
        if (Door[i].Status == Open)
        {
            Door[Door[i].nextDoor].exists = true;
            TargetLevel = Door[i].next;
        }
    }

    int count = 0;
    fstream InFile("data/levels.txt");
    string buffer;
    getline(InFile, buffer, '\n');
    while (1)
    {
        getline(InFile, buffer, ',');
        if (stoi(buffer) == TargetLevel)
        {
            for (int i = 0; i < 10; i++)
            {
                getline(InFile, buffer, ',');
                if (buffer.empty() == false)
                {
                    Door[i].exists = true;
                }
                if (buffer.empty() == true)
                {
                    Door[i].exists = false;
                }
                getline(InFile, buffer, ',');
                if (buffer.empty() == false)
                {
                    Door[i].next = stoi(buffer);
                }
                getline(InFile, buffer, ',');
                if (buffer.empty() == false && buffer != "\n")
                {
                    Door[i].nextDoor = stoi(buffer);
                }
            }
            getline(InFile, buffer, '\n');
            break;
            count = 0;
        }
        else
        {
            getline(InFile, buffer, '\n');
        }
    }
}

void UpdateMapRects(rects *R, player *Player, map *Map, door *Door, float *CamPosX, int *WindowWidth)
{
    for (int i = 0; i < MaxDoors; i++)
    {
        if (Door[i].Status == Open)
        {
            if (Player->PosX == R->DoorRect[Door[i].nextDoor].x)
            {
                ToUpdateMapRects = false;
            }
            int y;

            Player->PosX = -(Map->ActiveMap.w / 2) + 100 + 300 * (Door[i].nextDoor);
            //Player->PosX = DTRect[Door[i].nextDoor].x;

            *CamPosX = Player->PosX;
        }
    }

    for (Di = 0; Di < MaxDoors; Di++)
    {
        Door[Di].Status = Closed;
    }

    ToUpdateMapRects = false;
}

void UpdateTiles(maptile *MapTile, levelInfo LevelInfo)
{
    printf("Loading map... ");

    for (int i = 0; i < MaxTiles; i++)
    {
        MapTile[i] = LoadTile(LevelInfo.Texture_Type);
    }
    for (int i = 0; i < (LevelInfo.Length + 1); i++)
    {
        MapTile[i].exists = true;
    }
    for (int i = LevelInfo.Length + 1; i < MaxTiles; i++)
    {
        MapTile[i].exists = false;
    }

    MapTile[0].ActiveTexture = &MapTile[0].Start;
    MapTile[LevelInfo.Length].ActiveTexture = &MapTile[LevelInfo.Length].End;
    for (int i = 1; i < LevelInfo.Length; i++)
    {
        MapTile[i].ActiveTexture = &MapTile[i].Middle;
    }
    printf("Successful\n");
}

void RenderAll(rects *R, fonts Fonts, player *Player, map *Map, maptile *MapTile, door *Door, npc *NPC, fartCloud *PlayerFartCloud, fart *PlayerFart)
{
    SDL_BlitScaled(Map->ActiveMap.Surface, 0, WindowSurface, &R->MapRect);

    for (int i = 0; i < MaxTiles; i++)
    {
        if (MapTile[i].exists)
        {
            SDL_BlitScaled(MapTile[i].ActiveTexture->Surface, 0, WindowSurface, &R->MapTileRect[i]);
        }
    }

    RenderTextCentered(Fonts.Bold2, "This is a Game", 255, 255, 255, 0, -170, Fonts.TextSurface, WindowSurface, WindowWidth, WindowHeight);
    RenderText(Fonts.Regular, "Press H to say hello", 255, 255, 255, 0, 0, Fonts.TextSurface, WindowSurface, WindowWidth, WindowHeight);
    RenderText(Fonts.Regular, "Press F to pay respects", 255, 255, 255, 0, 25, Fonts.TextSurface, WindowSurface, WindowWidth, WindowHeight);
    RenderText(Fonts.Regular, "Press Q to quit the game", 255, 255, 255, 0, 50, Fonts.TextSurface, WindowSurface, WindowWidth, WindowHeight);
    RenderText(Fonts.Regular, "Press Esc to pause the game", 255, 255, 255, 0, 75, Fonts.TextSurface, WindowSurface, WindowWidth, WindowHeight);

    for (Di = 0; Di < MaxDoors; Di++)
    {
        if (Door[Di].exists)
        {
            SDL_BlitScaled(Door[Di].ActiveTexture->Surface, 0, WindowSurface, &R->DoorRect[Di]);
        }
    }

    SDL_BlitScaled(NPC[0].LeaningLeft.Surface, 0, WindowSurface, &R->NPCRect); //TODO: update to fit NPC!

    SDL_BlitScaled(Player->ActiveTexture->Surface, &R->PlayerActiveRectangle, WindowSurface, &R->PlayerRect);

    if (PlayerFart->ToFart)
    {
        if (Player->Direction == RightDirection)
        {
            SDL_BlitScaled(PlayerFart->ActiveTexture->Surface,
                           &R->PlayerFartActiveRect, WindowSurface, &R->PlayerFartRectR);
        }
        if (Player->Direction == LeftDirection)
        {
            SDL_BlitScaled(PlayerFart->ActiveTexture->Surface,
                           &R->PlayerFartActiveRect, WindowSurface, &R->PlayerFartRectL);
        }
    }
    for (FCi = 0; FCi < FClength; FCi++)
    {
        if (PlayerFartCloud[FCi].HasFarted)
        {
            SDL_BlitScaled(PlayerFartCloud[FCi].FartCloud.Surface, &R->PlayerFartCloudActiveRect[FCi],
                           WindowSurface, &R->PlayerFartCloudRect[FCi]);
        }
    }
    char FCcount[50];
    sprintf(FCcount, "Fart Cloud count: %i", FClength);

    if (FadeOut == true)
    {
        SDL_Surface *FadeSurface = SDL_CreateRGBSurface(0, WindowWidth, WindowHeight, 32, 0, 0, 0, 255);
        SDL_SetSurfaceBlendMode(FadeSurface, SDL_BLENDMODE_BLEND);
        SDL_SetSurfaceAlphaMod(FadeSurface, FadeOuti);
        if (FadeOuti > 0 && (FadeOuti - 20) > 0)
        {
            FadeOuti = FadeOuti - 20;
        }
        if (FadeOuti == 0 || (FadeOuti - 20) < 0)
        {
            SDL_SetSurfaceAlphaMod(FadeSurface, FadeIni);
            FadeOuti = 0;
            FadeOut = false;
        }
        SDL_BlitSurface(FadeSurface, 0, WindowSurface, 0);
        SDL_FreeSurface(FadeSurface);
    }
    if (FadeIn == true)
    {

        SDL_Surface *FadeSurface = SDL_CreateRGBSurface(0, WindowWidth, WindowHeight, 32, 0, 0, 0, 255);
        SDL_SetSurfaceBlendMode(FadeSurface, SDL_BLENDMODE_BLEND);
        SDL_SetSurfaceAlphaMod(FadeSurface, FadeIni);
        if (FadeIni < 255 && (FadeIni + 20) < 255)
        {
            FadeIni = FadeIni + 20;
        }
        if (FadeIni == 255 || (FadeIni + 20) > 255)
        {
            FadeIni = 255;
            SDL_SetSurfaceAlphaMod(FadeSurface, FadeIni);
            FadeIn = false;
            FadedIn = true;
        }
        SDL_BlitSurface(FadeSurface, 0, WindowSurface, 0);
        SDL_FreeSurface(FadeSurface);
    }
}

struct debug
{
    Uint32 frameStart;
    float RAM1, RAM2;
    int frameIndex = 0;
    float totalRAM, currentRAM;
    int CurrentFPS;
    int count;
    bool on;
    bool ShowDebug = false;
};

void DebugPre(debug *D)
{
    //-------------------Getting Resource usage---------------------------------------------------------------------
    //
    //                  ------- RAM -------
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    D->totalRAM = (float)(totalPhysMem / 1073741824.0f);
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    D->currentRAM = (float)(physMemUsedByMe / 1073741824.0f * 1024.0f);

    //--------------------------------------------------------------------------------------------------------------
    //FPS------------------------------------------------------
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    D->frameStart = SDL_GetTicks();
}

void FPSdelay(debug *D)
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    int frameEnd = SDL_GetTicks();
    int frameTime = frameEnd - D->frameStart;
    if (frameTime < frameDelay)
    {
        SDL_Delay(frameDelay - frameTime);
    }
}

void DebugPost(debug *D, fonts *F, keys *K)
{
    if (K->F1_Key)
    {
        for (;;)
        {
            if (D->ShowDebug)
            {
                D->ShowDebug = false;
                break;
            }
            if (!D->ShowDebug)
            {
                D->ShowDebug = true;
                break;
            }
        }
    }

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    char NowFPS[10];
    int frameEnd = SDL_GetTicks();
    int frameTime = frameEnd - D->frameStart;
    int actualFrameEnd = SDL_GetTicks();
    if (D->frameIndex++ % 10 == 0)
    {
        D->CurrentFPS = (float)(1000.0f / (actualFrameEnd - D->frameStart));
    }
    sprintf(NowFPS, "FPS: %i", D->CurrentFPS);
    char NowRAM[50];
    sprintf(NowRAM, "RAM usage: %.2f MB/ %.1f GB", D->currentRAM, D->totalRAM);
    //---------------------leak detector-------------------------------
    if (D->frameIndex == 60)
    {
        D->RAM1 = D->currentRAM;
    }
    if (D->frameIndex % 60 == 0)
    {
        D->RAM2 = D->currentRAM;
    }
    float sensetivity = 0.5;
    if (D->frameIndex > 120 && (D->RAM2 - D->RAM1) > sensetivity)
    {
        RAMleak = true;
    }
    if (D->frameIndex > 120 && (D->RAM2 - D->RAM1) < sensetivity)
    {
        RAMleak = false;
    }
    if (RAMleak)
    {
        if (D->frameIndex % 60 == 0)
        {
            D->on = true;
            D->count = 0;
        }
        if (D->count == 30)
        {
            D->on = false;
        }
        char RAMdiff[50];
        float RAMdifference = D->RAM2 - D->RAM1;
        sprintf(RAMdiff, "leak: %.2fMB", RAMdifference);
        if (D->on)
        {
            if (D->ShowDebug)
            {
                RenderText(F->Bold, "Memory Leak Detected!", 255, 255, 0, WindowWidth - 325, 25, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
                RenderText(F->RegularS, RAMdiff, 255, 255, 255, WindowWidth - 100, 27, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
            }
        }
        if (D->on == false)
        {
            if (D->ShowDebug)
            {
                RenderText(F->Bold, "Memory Leak Detected!", 255, 0, 0, WindowWidth - 325, 25, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
                RenderText(F->RegularS, RAMdiff, 255, 255, 255, WindowWidth - 100, 27, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
            }
        }
        D->count++;
    }
    if (D->ShowDebug)
    {
        RenderText(F->RegularS, NowFPS, 170, 170, 255, WindowWidth - 60, 0, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
        RenderText(F->RegularS, NowRAM, 255, 255, 150, WindowWidth - 300, 0, F->TextSurface, WindowSurface, WindowWidth, WindowHeight);
    }
}

void PollEvents(SDL_Event *Event, keys *K)
{
    while (SDL_PollEvent(Event)) //if pPollEvent returns 1 then we enter the while loop this
                                 //means thatif we have more than one event, it gathers them all before running
    {
        if (Event->type == SDL_QUIT)
        {
            GameIsRunning = false;
        }
        if (Event->key.keysym.sym == SDLK_q)
        {
            GameIsRunning = false;
        }
        //only pressable once key:
        if (Event->type == SDL_KEYDOWN)
        {
            if (Event->key.keysym.sym == SDLK_e && Event->key.repeat == false)
            {
                K->E_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_f && Event->key.repeat == false)
            {
                K->F_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_TAB && Event->key.repeat == false)
            {
                K->Tab_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_SPACE && Event->key.repeat == false)
            {
                K->Space_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_F1 && Event->key.repeat == false)
            {
                K->F1_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_ESCAPE && Event->key.repeat == false)
            {
                K->RightButton = false;
                K->LeftButton = false;
                K->UpButton = false;
                K->DownButton = false;
                K->Shift = false;
                K->F_Key = false;
                K->H_Key = false;
                K->E_Key = false;
                K->Tab_Key = false;
                K->Space_Key = false;
                Playing = false;
                Mix_PauseMusic();
            }
        }

        //printf(E_Key ? "E = true\n" : "E = false\n");
        //cool shit:
        if (Event->type == SDL_KEYDOWN || Event->type == SDL_KEYUP)
        {
            bool KeyState = Event->type == SDL_KEYDOWN ? true : false;
            // upButton for example will have the value of keystate
            // which is true when button is down and when thats not the
            // case it can only be up vecause thats the only two options
            // given by the if statement
            switch (Event->key.keysym.sym)
            {
            case SDLK_UP:
                K->UpButton = KeyState;
                break;
            case SDLK_DOWN:
                K->DownButton = KeyState;
                break;
            case SDLK_LEFT:
                K->LeftButton = KeyState;
                break;
            case SDLK_RIGHT:
                K->RightButton = KeyState;
                break;
            case SDLK_LSHIFT:
                K->Shift = KeyState;
                break;
            case SDLK_RSHIFT:
                K->Shift = KeyState;
                break;
            case SDLK_h:
                K->H_Key = KeyState;
                break;
            default:
                break;
            }
        }
    }
}