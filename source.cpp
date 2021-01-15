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

void DoorsUpdate(door *DT, SDL_Rect PlayerRect, SDL_Rect *DTRect, TTF_Font *Font, SDL_Surface *TextSurface,
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
                if (PlayerRect.x < (DTRect[Di].x + 48) && PlayerRect.x > (DTRect[Di].x - 48))
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

void NPCUpdate(player *Player, SDL_Rect PlayerRect, SDL_Rect *NPCRect, TTF_Font *Font, SDL_Surface *TextSurface,
               SDL_Surface *WindowSurface, int WindowWidth, int WindowHight, bool E_Key)
{
    if (PlayerRect.x > (NPCRect->x - 60) && PlayerRect.x < (NPCRect->x + 10))
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

void UpdateMap(SDL_Rect *PlayerRect, map *Map, door *Door, SDL_Rect *DTRect, float *CamPosX, int *WindowWidth,
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

void UpdateMapRects(player *Player, SDL_Rect *PlayerRect, map *Map, door *Door, SDL_Rect *DTRect, float *CamPosX, int *WindowWidth)
{
    for (int i = 0; i < MaxDoors; i++)
    {
        if (Door[i].Status == Open)
        {
            if (Player->PosX == DTRect[Door[i].nextDoor].x)
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
}