#include "raggy.hpp"
#include "source.cpp"
#include "rects.cpp"
#include "dialogues.cpp"
#include "menus.cpp"

SDL_Surface *WindowSurface;
SDL_Window *Window;

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

    //load fonts------------------------
    printf("Loading fonts... ");

    TTF_Init();
    fonts F;

    SDL_Surface *TextSurface;
    printf("Successful\n");
    //----------------------------------
    int frameIndex = 0;

    //------KEYS-------------
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

    bool Playing = false;

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
        E_Key = false;
        F_Key = false;

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
        float totalRAM = (float)(totalPhysMem / 1073741824.0f);
        SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
        float currentRAM = (float)(physMemUsedByMe / 1073741824.0f * 1024.0f);

        float RAM1, RAM2;

        //--------------------------------------------------------------------------------------------------------------
        //FPS------------------------------------------------------
        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart = SDL_GetTicks();
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
        while (SDL_PollEvent(&Event)) //if pPollEvent returns 1 then we enter the while loop this
                                      //means thatif we have more than one event, it gathers them all before running
        {
            if (Event.type == SDL_QUIT)
            {
                GameIsRunning = false;
            }
            if (Event.key.keysym.sym == SDLK_q)
            {
                GameIsRunning = false;
            }
            //only pressable once key:
            if (Event.type == SDL_KEYDOWN)
            {
                if (Event.key.keysym.sym == SDLK_e && Event.key.repeat == false)
                {
                    E_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_f && Event.key.repeat == false)
                {
                    F_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_TAB && Event.key.repeat == false)
                {
                    Tab_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_SPACE && Event.key.repeat == false)
                {
                    Space_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_ESCAPE && Event.key.repeat == false)
                {
                    RightButton = false;
                    LeftButton = false;
                    UpButton = false;
                    DownButton = false;
                    Shift = false;
                    F_Key = false;
                    H_Key = false;
                    E_Key = false;
                    Tab_Key = false;
                    Space_Key = false;
                    Playing = false;
                    Mix_PauseMusic();
                }
            }

            //printf(E_Key ? "E = true\n" : "E = false\n");
            //cool shit:
            if (Event.type == SDL_KEYDOWN || Event.type == SDL_KEYUP)
            {
                bool KeyState = Event.type == SDL_KEYDOWN ? true : false;
                // upButton for example will have the value of keystate
                // which is true when button is down and when thats not the
                // case it can only be up vecause thats the only two options
                // given by the if statement
                switch (Event.key.keysym.sym)
                {
                case SDLK_UP:
                    UpButton = KeyState;
                    break;
                case SDLK_DOWN:
                    DownButton = KeyState;
                    break;
                case SDLK_LEFT:
                    LeftButton = KeyState;
                    break;
                case SDLK_RIGHT:
                    RightButton = KeyState;
                    break;
                case SDLK_LSHIFT:
                    Shift = KeyState;
                    break;
                case SDLK_RSHIFT:
                    Shift = KeyState;
                    break;
                case SDLK_h:
                    H_Key = KeyState;
                    break;
                default:
                    break;
                }
            }
        }

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
        PlayerUpdate(&Player, CamPosX, RightButton, LeftButton, UpButton,
                     DownButton, Shift, MapLimitL, MapLimitR, WalkSpeed, RunSpeed);
        PlayerSoundUpdate(Sound, F_Key, H_Key);
        MapUpdate(&CamPosX, &Player);

        FartUpdate(&Player, &PlayerFart, PlayerFartCloud, F_Key);

        DoorsUpdate(Door, &R, F.Regular, TextSurface, WindowSurface, WindowWidth, WindowHeight, E_Key);
        NPCUpdate(&Player, &R, F.Regular, TextSurface, WindowSurface, WindowWidth, WindowHeight, E_Key);

        if (Player.Chatting == true)
        {
            if (FadedIn == false && FadeIn == false)
            {
                FadeIn = true;
            }
            if (FadedIn == true && FadeIn == false)
            {
                //lets go of all already pressed buttons otherwise they dont reset
                RightButton = false;
                LeftButton = false;
                UpButton = false;
                DownButton = false;
                Shift = false;
                F_Key = false;
                H_Key = false;
                E_Key = false;
                Tab_Key = false;
                Space_Key = false;

                ResetFades();

                RAM1 = currentRAM; // this is here to calculate RAM change after leaving Dialogue Mode to detect leaks.

                DialogueMode(F,
                             &TextSurface, &WindowSurface, &Window, &WindowWidth, &WindowHeight, &Player, BackgroundMusic, &MusicBool, &SoundBool);
                ResetFades();
            }
        }
        //----------------------------LOAD RECTS HERE------------------------------------------
        //          IMPORTANT: make sure you update this function here and in rect.cpp
        //                     every time you add a new object! and define the rects
        //                     outside the loop.
        LoadRects(&WindowWidth, &WindowHeight, CamPosX, &F_Key, &R,
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
        //          IMPORTANT: make sure you render the character last and the map first.
        //                     NPCs go before the player typically.
        RenderAll(&R, F, &Player, &Map, MapTile, Door, NPC, PlayerFartCloud, &PlayerFart);

        /////////////////////////Printf section///////////////////////////////

        //printf("PX=%f, PRect= %i, DoorRect0= %i, CamPosX=%f, DTPosX=%f \n", Player.PosX, PlayerRect.x, DoorRect[Door[5].nextDoor].x, CamPosX, Door[MAPPPP].PosX);
        //printf(ToUpdateMapRects ? "TUM = true\n" : "TUM = false\n");

        //////////////////////////////////////////////////////////////////////

        //FPS and Resources------------------------------------------------------
        {
            char NowFPS[10]; 
            int frameEnd = SDL_GetTicks();
            int frameTime = frameEnd - frameStart;
            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }
            int actualFrameEnd = SDL_GetTicks();
            if (frameIndex++ % 10 == 0)
            {
                int CurrentFPS = (float)(1000.0f / (actualFrameEnd - frameStart));
                sprintf(NowFPS, "FPS: %i", CurrentFPS);
            }
            char NowRAM[50];
            sprintf(NowRAM, "RAM usage: %.2f MB/ %.1f GB", currentRAM, totalRAM);
            //---------------------leak detector-------------------------------
            if (frameIndex == 60)
            {
                RAM1 = currentRAM;
            }
            if (frameIndex % 60 == 0)
            {
                RAM2 = currentRAM;
            }
            float sensetivity = 0.5;
            if (frameIndex > 120 && (RAM2 - RAM1) > sensetivity)
            {
                RAMleak = true;
            }
            if (frameIndex > 120 && (RAM2 - RAM1) < sensetivity)
            {
                RAMleak = false;
            }
            if (RAMleak)
            {
                bool on;
                int count;
                if (frameIndex % 60 == 0)
                {
                    on = true;
                    count = 0;
                }
                if (count == 30)
                {
                    on = false;
                }
                char RAMdiff[50];
                float RAMdifference = RAM2 - RAM1;
                sprintf(RAMdiff, "leak: %.2fMB", RAMdifference);
                if (on)
                {
                    RenderText(F.Bold, "Memory Leak Detected!", 255, 255, 0, WindowWidth - 325, 25, TextSurface, WindowSurface, WindowWidth, WindowHeight);
                    RenderText(F.RegularS, RAMdiff, 255, 255, 255, WindowWidth - 100, 27, TextSurface, WindowSurface, WindowWidth, WindowHeight);
                }
                if (on == false)
                {
                    RenderText(F.Bold, "Memory Leak Detected!", 255, 0, 0, WindowWidth - 325, 25, TextSurface, WindowSurface, WindowWidth, WindowHeight);
                    RenderText(F.RegularS, RAMdiff, 255, 255, 255, WindowWidth - 100, 27, TextSurface, WindowSurface, WindowWidth, WindowHeight);
                }
                count++;
            }

            RenderText(F.RegularS, NowFPS, 170, 170, 255, WindowWidth - 60, 0, TextSurface, WindowSurface, WindowWidth, WindowHeight);
            RenderText(F.RegularS, NowRAM, 255, 255, 150, WindowWidth - 300, 0, TextSurface, WindowSurface, WindowWidth, WindowHeight);

            char DebugPlayerPosX[30];
            sprintf(DebugPlayerPosX, "Player Pos X: %i", (int16_t)Player.PosX);
            RenderText(F.RegularS, DebugPlayerPosX, 170, 170, 170, WindowWidth - 300, 50, TextSurface, WindowSurface, WindowWidth, WindowHeight);
            char DebugCamPosX[30];
            sprintf(DebugCamPosX, "Camera Position X: %i", (int16_t)CamPosX);
            RenderText(F.RegularS, DebugCamPosX, 170, 170, 170, WindowWidth - 300, 75, TextSurface, WindowSurface, WindowWidth, WindowHeight);
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
