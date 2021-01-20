#include "raggy.hpp"
#include "source.cpp"
#include "rects.cpp"
#include "dialogues.cpp"
#include "menus.cpp"



int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    bool SoundBool;
    bool MusicBool;

    {
        fstream InFile("data/settings.rgg");
        string buffer;
        getline(InFile, buffer, ':');
        getline(InFile, buffer, '\n');
        WindowWidth = stoi(buffer);
        getline(InFile, buffer, ':');
        getline(InFile, buffer, '\n');
        WindowHeight = stoi(buffer);
        getline(InFile, buffer, ':');
        getline(InFile, buffer, '\n');
        if (buffer == "ON")
        {
            MusicBool = true;
        }
        if (buffer == "OFF")
        {
            MusicBool = false;
        }
        getline(InFile, buffer, ':');
        getline(InFile, buffer, '\n');
        if (buffer == "ON")
        {
            SoundBool = true;
        }
        if (buffer == "OFF")
        {
            SoundBool = false;
        }
    }
    Window = SDL_CreateWindow("Raggy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, 0);
    WindowSurface = SDL_GetWindowSurface(Window);
    SDL_FillRect(WindowSurface, 0, (255 << 24) | (50 << 16) | (50 << 8) | (50));
    SDL_UpdateWindowSurface(Window);

    //------load sounds--------
    printf("Loading sounds... ");

    Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 4096);
    sounds Sound = LoadSound();

    Mix_Music *BackgroundMusic = Mix_LoadMUS("data/sounds/music/background.WAV");

    printf("Successful\n");

    float CamPosX = 0;

    //load fonts---------------
    printf("Loading fonts... ");

    TTF_Init();
    fonts F;

    SDL_Surface *TextSurface;
    printf("Successful\n");

    //------KEYS-------------
    keys K;
    //-----------------------

    //-------------------Load Objects--------------------------
    printf("Loading objects... ");

    player Player = LoadPlayer();
    map Map = LoadMap(CurrentMap);
    fart PlayerFart = LoadFart();
    fartCloud PlayerFartCloud[20];
    for (FCi = 0; FCi < 20; FCi++)
    {
        PlayerFartCloud[FCi] = LoadFartCloud();
    }

    door Door[MaxDoors];
    for (Di = 0; Di < MaxDoors; Di++)
    {
        Door[Di] = LoadDoor();
    }

    maptile MapTile[MaxTiles];

    for (int i = 0; i < MaxTiles; i++)
    {
        MapTile[i] = LoadTile(0);
    }
    MapTile[0].ActiveTexture = &MapTile[0].Start;
    MapTile[MaxTiles - 1].ActiveTexture = &MapTile[MaxTiles - 1].End;
    for (int i = 1; i < MaxTiles - 1; i++)
    {
        MapTile[i].ActiveTexture = &MapTile[i].Middle;
    }

    npc NPC[MaxNPCs];
    for (int i = 0; i < MaxNPCs; i++)
    {
        NPC[i] = LoadNPC(i);
    }

    printf("Successful\n");
    //---------------------------------------------------------

    Player.PosX = 0;
    Player.PosY = 0;

    Map.ActiveMap.h = Map.ActiveMap.h * 3;
    Map.ActiveMap.w = Map.ActiveMap.w * 3;

    int InitialMapPosX = Map.PosX;
    int InitialPlayerPosX = Player.PosX;

    int WalkSpeed = 2;
    int RunSpeed = 4; //They're now adjustable, I still don't see why you need them to be as such,
                      // but here, 1,5 hours later, I present you ADJUSTABLE SPEEDS * Confetti *

    rects R;

    R.PlayerRect.x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16);
    R.PlayerRect.y = (WindowHeight / 2) - 48;
    R.PlayerRect.w = 32 * 3; // TODO: Scaling is an inherent problem that needs fixing.
    R.PlayerRect.h = 32 * 3;

    R.MapRect.h = Map.ActiveMap.h;
    R.MapRect.w = Map.ActiveMap.w;
    R.MapRect.x = ((WindowWidth - Map.ActiveMap.w) / 2) - CamPosX;
    R.MapRect.y = Map.PosY;

    ResetFades();

    levelInfo LevelInfo;
    string LevelFileString = Lexer_FileToString("data/levels.rmp");
    stringstream LevelInput;
    token LevelToken;
    LevelInput.str(LevelFileString);
    RMP_ParseLevel(&LevelInput, &LevelInfo);
    RMP_ReloadStream(&LevelInput, LevelFileString);

    //---------------intro Logo----------------------
    {

        bool IntroRunning = true;
        SDL_Event Event;
        SDL_Rect LogoRect;
        printf("Loading logo... ");
        sprite Logo = LoadSprite("data/textures/ragsterb_logo.png");
        printf("Successful\n");
        Mix_Chunk *IntroMusic;
        printf("Loading intro music... ");
        IntroMusic = Mix_LoadWAV("data/sounds/music/intro.wav");
        printf("Successful\n");
        Mix_PlayChannel(3, IntroMusic, 0);

        while (IntroRunning)
        {
            //FPS------------------------------------------------------
            const int FPS = 60;
            const int frameDelay = 1000 / FPS;
            Uint32 frameStart = SDL_GetTicks();
            //------------------------------------------------------
            while (SDL_PollEvent(&Event)) //if pPollEvent returns 1 then we enter the while loop this
                                          //means thatif we have more than one event, it gathers them all before running
            {
                if (Event.type == SDL_QUIT)
                {
                    IntroRunning = false;
                    GameIsRunning = false;
                }
                if (Event.type == SDL_KEYDOWN)
                {
                    if (Event.key.keysym.sym == SDLK_RETURN && Event.key.repeat == false)
                    {
                        IntroRunning = false;
                        Mix_HaltChannel(3);
                    }
                }
            }
            LogoRect.h = Logo.h / 6;
            LogoRect.w = Logo.w / 6;
            LogoRect.x = (WindowWidth - Logo.w / 6) / 2;
            LogoRect.y = (WindowHeight - Logo.h / 6) / 2;

            SDL_FillRect(WindowSurface, 0, (255 << 24) | (50 << 16) | (50 << 8) | (50));
            SDL_SetSurfaceBlendMode(Logo.Surface, SDL_BLENDMODE_BLEND);
            SDL_SetSurfaceAlphaMod(Logo.Surface, FadeIni);
            if (FadedIn == false)
            {
                if (FadeIni < 255 && (FadeIni + 1) < 255)
                {
                    FadeIni = FadeIni + 1;
                }
                if (FadeIni == 255 || (FadeIni + 1) > 255)
                {
                    FadeIni = 255;
                    SDL_SetSurfaceAlphaMod(Logo.Surface, FadeIni);
                    FadeIn = false;
                    FadedIn = true;
                }
            }

            SDL_BlitScaled(Logo.Surface, 0, WindowSurface, &LogoRect);
            char NowFPS[10];
            int frameEnd = SDL_GetTicks();
            int frameTime = frameEnd - frameStart;
            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }

            SDL_UpdateWindowSurface(Window);
            if (Mix_Playing(3) == 0)
            {
                IntroRunning = false;
            }
        }
        SDL_FreeSurface(Logo.Surface);
    }
    ResetFades();

    for (int i = 0; i < MaxDoors; i++)
    {
        Door[i].exists = false;
    }
    Door[5].exists = true;
    Door[5].next = 1;
    Door[5].nextDoor = 0;
    Door[0].next = 0;
    Door[0].nextDoor = 5;

    {
        fstream InFile("data/levels.txt");
        string buffer;
        getline(InFile, buffer, '\n');
        getline(InFile, buffer, ','); //skip level number...
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
            if (buffer.empty() == false)
            {
                Door[i].nextDoor = stoi(buffer);
            }
        }
        getline(InFile, buffer, '\n');
        InFile.close();
    }

    debug D;

    bool UpdatedMap = false;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////    GAME LOOP    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (GameIsRunning)
    {
        if (Playing == false)
        {
            Mix_PauseMusic();
            MainMenu(F, &TextSurface, &WindowSurface, &Window, &WindowWidth, &WindowHeight, &Playing,
                     &MusicBool, &SoundBool);
        }
        if (GameIsRunning == false)
        {
            break;
        }
        if (ToUpdateMap)
        {
            UpdateMap(&R, &Map, Door, &CamPosX, &WindowWidth,
                      &LevelInfo, &LevelInput, &LevelToken, LevelFileString);
            UpdateTiles(MapTile, LevelInfo);
            ToUpdateMap = false;
            UpdatedMap = true;
        }

        //leave these here. they're necessary for the "pressable once" system.
        K.E_Key = false;
        K.F_Key = false;
        K.F1_Key = false;

        DebugPre(&D);

        //---------------------------------------------------------
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(BackgroundMusic, 50);
        }
        if (MusicBool == true)
        {
            Mix_VolumeMusic(128);
        }
        if (MusicBool == false)
        {
            Mix_VolumeMusic(0);
        }
        if (SoundBool == true)
        {
            Mix_Volume(-1, 128);
        }
        if (SoundBool == false)
        {
            Mix_Volume(-1, 0);
        }

        SDL_Event Event;
        PollEvents(&Event, &K);

        //----------------------------Fill Screen---------------------------------------------------
        int Red = 50;
        int Green = 50;
        int Blue = 50;
        int Alpha = 255;

        // WINDOWS USES BGRA (that does not stand for bulgaria)
        // TODO: map it so that it works on multiple systems
        SDL_FillRect(WindowSurface, 0, (Alpha << 24) | (Red << 16) | (Green << 8) | (Blue));

        float MapLimitR = Map.ActiveMap.w * 0.5f - 3 * 16;
        float MapLimitL = -Map.ActiveMap.w * 0.5f + 3 * 16;

        int limitcounter = 0;
        for (int i = 0; i < MaxTiles; i++)
        {
            if (MapTile[i].exists)
            {
                ++limitcounter;
            }
        }
        MapLimitR = limitcounter * 32 * 3 * 0.5f - 3 * 16;
        MapLimitL = -limitcounter * 32 * 3 * 0.5f + 3 * 16;

        //-------------------------Game Update----------------------------------------------------------
        PlayerUpdate(&Player, CamPosX, &K, MapLimitL, MapLimitR, WalkSpeed, RunSpeed);
        PlayerSoundUpdate(Sound, &K);
        MapUpdate(&CamPosX, &Player);

        FartUpdate(&Player, &PlayerFart, PlayerFartCloud, K.F_Key);

        DoorsUpdate(Door, &R, F.Regular, TextSurface, WindowSurface, WindowWidth, WindowHeight, K.E_Key);
        NPCUpdate(&Player, &R, F.Regular, TextSurface, WindowSurface, WindowWidth, WindowHeight, K.E_Key);

        if (Player.Chatting == true)
        {
            if (FadedIn == false && FadeIn == false)
            {
                FadeIn = true;
            }
            if (FadedIn == true && FadeIn == false)
            {
                //lets go of all already pressed buttons otherwise they dont reset
                K.RightButton = false;
                K.LeftButton = false;
                K.UpButton = false;
                K.DownButton = false;
                K.Shift = false;
                K.F_Key = false;
                K.H_Key = false;
                K.E_Key = false;
                K.Tab_Key = false;
                K.Space_Key = false;

                ResetFades();

                D.RAM1 = D.currentRAM; // this is here to calculate RAM change after leaving Dialogue Mode to detect leaks.

                DialogueMode(F,
                             &TextSurface, &WindowSurface, &Window, &WindowWidth, &WindowHeight, &Player, BackgroundMusic, &MusicBool, &SoundBool);
                ResetFades();
            }
        }
        //----------------------------LOAD RECTS HERE------------------------------------------
        //          IMPORTANT: make sure you update this function here and in rect.cpp
        //                     every time you add a new object! and define the rects
        //                     outside the loop.
        LoadRects(&WindowWidth, &WindowHeight, CamPosX, &K.F_Key, &R,
                  &Player,
                  Map,
                  PlayerFart,
                  PlayerFartCloud,
                  Door,
                  NPC,
                  MapTile);
        if (ToUpdateMapRects && UpdatedMap)
        {
            UpdateMapRects(&R, &Player, &Map, Door, &CamPosX, &WindowWidth);
            //MapUpdate(&CamPosX, &Player);
            UpdatedMap = false;
        }

        //-----------------------------Rendering-------------------------------------------------
        RenderAll(&R, F, &Player, &Map, MapTile, Door, NPC, PlayerFartCloud, &PlayerFart);

        /////////////////////////Printf section///////////////////////////////

        //printf("PX=%f, PRect= %i, DoorRect0= %i, CamPosX=%f, DTPosX=%f \n", Player.PosX, PlayerRect.x, DoorRect[Door[5].nextDoor].x, CamPosX, Door[MAPPPP].PosX);
        //printf(ToUpdateMapRects ? "TUM = true\n" : "TUM = false\n");

        //////////////////////////////////////////////////////////////////////

        //FPS and Resources------------------------------------------------------
        FPSdelay(&D);
        DebugPost(&D, &F, &K);

        if (D.ShowDebug){
            char DebugPlayerPosX[30];
            sprintf(DebugPlayerPosX, "Player Pos X: %i", (int16_t)Player.PosX);
            RenderText(F.RegularS, DebugPlayerPosX, 170, 170, 170, WindowWidth - 300, 50, F.TextSurface, WindowSurface, WindowWidth, WindowHeight);
            char DebugCamPosX[30];
            sprintf(DebugCamPosX, "Camera Position X: %i", (int16_t)CamPosX);
            RenderText(F.RegularS, DebugCamPosX, 170, 170, 170, WindowWidth - 300, 75, F.TextSurface, WindowSurface, WindowWidth, WindowHeight);
        }
        //------------------------------------------------------
        SDL_UpdateWindowSurface(Window);
    };
    //end of game loop

    //save settings:
    ofstream OutFile("data/settings.rgg");
    OutFile << "WindowWidth:" << WindowWidth << endl;
    OutFile << "WindowHeight:" << WindowHeight << endl;
    OutFile << "Music:";
    if (MusicBool == true)
    {
        OutFile << "ON" << endl;
    }
    if (MusicBool == false)
    {
        OutFile << "OFF" << endl;
    }
    OutFile << "Sound:";
    if (SoundBool == true)
    {
        OutFile << "ON" << endl;
    }
    if (SoundBool == false)
    {
        OutFile << "OFF" << endl;
    }
    printf("\nSettings saved\nExit successful\n");

    return 0;
}
