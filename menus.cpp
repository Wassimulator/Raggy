#pragma once
#include "raggy.hpp"
#include "source.cpp"

void OptionsMenu(TTF_Font *Regular, TTF_Font *RegularS, TTF_Font *Bold, TTF_Font *Bold2,
                 TTF_Font *Title1, TTF_Font *Title2, TTF_Font *Title1B, TTF_Font *Title2B,
                 TTF_Font *Title3, TTF_Font *Title3B,
                 SDL_Surface *TextSurface,
                 SDL_Surface **WindowSurface,
                 SDL_Window **Window,
                 int *WindowWidth, int *WindowHight,
                 bool *Playing)
{
    bool OptionsRunning = true;
    int frameIndex = 0;
    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Return_Key = false;
    const int MaxOptions = 4;

    SDL_Rect OptionRect[MaxOptions];
    SDL_Surface *OptionSurface[MaxOptions];
    SDL_Surface *HighlightedOptionSurface[MaxOptions];
    SDL_Color MenuOptionsColor = {200, 200, 200};
    SDL_Color ResolutionColor = {50, 200, 50};
    char *OptionText[MaxOptions];
    bool SelectedOption[MaxOptions];
    bool HighlightedOption[MaxOptions];

    SDL_Rect ResolutionRect[5];
    SDL_Surface *ResolutionSurface[5];
    SDL_Surface *HighlightedResolutionSurface[5];
    char *ResolutionText[5];
    bool SelectedResolution[5];
    bool HighlightedResolution[5];

    const int MaxOnOff = 4;
    SDL_Rect OnOffRect[MaxOnOff];
    SDL_Surface *OnOffSurface[MaxOnOff];
    SDL_Surface *OnOffSelectedSurface[MaxOnOff];
    char *OnOffText[MaxOnOff];
    bool HighlightedOnOff[MaxOnOff];
    OnOffText[0] = "ON";
    OnOffText[1] = "OFF";
    OnOffText[2] = "ON";
    OnOffText[3] = "OFF";

    ResolutionText[0] = "800x600";
    ResolutionText[1] = "960x720";
    ResolutionText[2] = "1024x768";
    ResolutionText[3] = "1280x960";
    ResolutionText[4] = "1280x720";

    OptionText[0] = "Resolution:";
    OptionText[1] = "Music:";
    OptionText[2] = "Sounds:";
    OptionText[3] = "Back";
    for (int i = 0; i < MaxOptions; i++)
    {
        HighlightedOption[i] = false;
    }
    HighlightedOption[0] = true;
    for (int i = 0; i < 5; i++)
    {
        HighlightedResolution[i] = false;
    }
    switch (*WindowWidth)
    {
    case 800:
        HighlightedResolution[0] = true;
        break;
    case 960:
        HighlightedResolution[1] = true;
        break;
    case 1024:
        HighlightedResolution[2] = true;
        break;
    case 1280:
        if (*WindowHight == 960)
        {
            HighlightedResolution[3] = true;
        }
        if (*WindowHight == 720)
        {
            HighlightedResolution[4] = true;
        }

        break;
    }

    if (Mix_VolumeMusic(-1) == 128)
    {
        HighlightedOnOff[0] = true;
        HighlightedOnOff[1] = false;
    }
    if (Mix_VolumeMusic(-1) == 0)
    {
        HighlightedOnOff[1] = true;
        HighlightedOnOff[0] = false;
    }

    if (Mix_Volume(-1, -1) == 128)
    {
        HighlightedOnOff[2] = true;
        HighlightedOnOff[3] = false;
    }
    if (Mix_Volume(-1, -1) == 0)
    {
        HighlightedOnOff[3] = true;
        HighlightedOnOff[2] = false;
    }

    while (OptionsRunning)
    {
        RightButton = false;
        LeftButton = false;
        UpButton = false;
        DownButton = false;
        Return_Key = false;
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
        //--------------------------------------------------------------------------------------------------------------
        //FPS------------------------------------------------------
        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart = SDL_GetTicks();
        //---------------------------------------------------------
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) //if pPollEvent returns 1 then we enter the while loop this
                                      //means thatif we have more than one event, it gathers them all before running
        {
            if (Event.type == SDL_QUIT)
            {
                GameIsRunning = false;
                MainMenuRunning = false;
                OptionsRunning = false;
                break;
            }
            if (Event.type == SDL_KEYDOWN)
            {
                if (Event.key.keysym.sym == SDLK_RETURN && Event.key.repeat == false)
                {
                    Return_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_DOWN && Event.key.repeat == false)
                {
                    DownButton = true;
                }
                if (Event.key.keysym.sym == SDLK_UP && Event.key.repeat == false)
                {
                    UpButton = true;
                }
                if (Event.key.keysym.sym == SDLK_LEFT && Event.key.repeat == false)
                {
                    LeftButton = true;
                }
                if (Event.key.keysym.sym == SDLK_RIGHT && Event.key.repeat == false)
                {
                    RightButton = true;
                }
            }
        }

        for (int i = 0; i < MaxOptions; i++)
        {
            OptionSurface[i] = TTF_RenderText_Blended(Title2, OptionText[i], MenuOptionsColor);
            HighlightedOptionSurface[i] = TTF_RenderText_Blended(Title2B, OptionText[i], MenuOptionsColor);
            OptionRect[i].h = OptionSurface[i]->h;
            OptionRect[i].w = OptionSurface[i]->w;
            OptionRect[i].x = (*WindowWidth - OptionSurface[i]->w) / 2;
        }

        OptionRect[0].y = *WindowHight / 2 - 100;
        for (int i = 1; i < MaxOptions; i++)
        {
            OptionRect[i].y = OptionRect[i - 1].y + OptionRect[i - 1].h + 40;
        }

        for (int i = 0; i < 5; i++)
        {
            ResolutionSurface[i] = TTF_RenderText_Blended(Title3, ResolutionText[i], ResolutionColor);
            HighlightedResolutionSurface[i] = TTF_RenderText_Blended(Title3B, ResolutionText[i], ResolutionColor);
            ResolutionRect[i].h = ResolutionSurface[i]->h;
            ResolutionRect[i].w = ResolutionSurface[i]->w;
            ResolutionRect[i].y = OptionRect[0].y + OptionSurface[0]->h;
        }
        int ResolutionLength = ResolutionSurface[0]->w +
                               ResolutionSurface[1]->w +
                               ResolutionSurface[2]->w +
                               ResolutionSurface[3]->w +
                               ResolutionSurface[4]->w;

        ResolutionRect[0].x = ((*WindowWidth - ResolutionLength - 50) / 2);
        for (int i = 1; i < 5; i++)
        {
            ResolutionRect[i].x = ResolutionRect[i - 1].x + ResolutionSurface[i - 1]->w + 20;
        }

        for (int i = 0; i < MaxOnOff; i = i + 2)
        {
            OnOffSurface[i] = TTF_RenderText_Blended(Title3, OnOffText[i], ResolutionColor);
            OnOffSelectedSurface[i] = TTF_RenderText_Blended(Title3B, OnOffText[i], ResolutionColor);
            OnOffRect[i].h = OnOffSurface[i]->h;
            OnOffRect[i].w = OnOffSurface[i]->w;
            OnOffRect[i].x = (*WindowWidth / 2) - 20 - OnOffSurface[i]->w;
        }
        OnOffRect[0].y = OnOffRect[1].y = OptionRect[1].y + OptionSurface[1]->h;
        OnOffRect[2].y = OnOffRect[3].y = OptionRect[2].y + OptionSurface[2]->h;
        for (int i = 1; i < MaxOnOff; i = i + 2)
        {
            OnOffSurface[i] = TTF_RenderText_Blended(Title3, OnOffText[i], ResolutionColor);
            OnOffSelectedSurface[i] = TTF_RenderText_Blended(Title3B, OnOffText[i], ResolutionColor);
            OnOffRect[i].h = OnOffSurface[i]->h;
            OnOffRect[i].w = OnOffSurface[i]->w;
            OnOffRect[i].x = (*WindowWidth / 2) + 10;
        }

        for (int i = 0; i < MaxOptions; i++)
        {
            if (HighlightedOption[i] == true && Return_Key == true)
            {
                SelectedOption[i] = true;
            }
        }
        if (DownButton)
        {
            for (int i = 0; i < MaxOptions; i++)
            {
                if (HighlightedOption[i] == true && i < (MaxOptions - 1) && i >= 0)
                {
                    HighlightedOption[i + 1] = true;
                    HighlightedOption[i] = false;
                    break;
                }
            }
        }
        if (UpButton)
        {
            for (int i = 0; i < MaxOptions; i++)
            {
                if (HighlightedOption[i] == true && i <= (MaxOptions - 1) && i > 0)
                {
                    HighlightedOption[i - 1] = true;
                    HighlightedOption[i] = false;
                    break;
                }
            }
        }
        /////////////////////////////////////////
        if (HighlightedOption[0] == true)
        {
            if (RightButton)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (HighlightedResolution[i] == true && i < (5 - 1) && i >= 0)
                    {
                        HighlightedResolution[i + 1] = true;
                        HighlightedResolution[i] = false;
                        break;
                    }
                }
            }
            if (LeftButton)
            {
                for (int i = 0; i < 5; i++)
                {
                    if (HighlightedResolution[i] == true && i <= (5 - 1) && i > 0)
                    {
                        HighlightedResolution[i - 1] = true;
                        HighlightedResolution[i] = false;
                        break;
                    }
                }
            }
        }

        if (HighlightedResolution[0] == true && Return_Key)
        {
            *WindowWidth = 800;
            *WindowHight = 600;
            SDL_DestroyWindow(*Window);
            *Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            *WindowSurface = SDL_GetWindowSurface(*Window);
        }
        if (HighlightedResolution[1] == true && Return_Key)
        {
            *WindowWidth = 960;
            *WindowHight = 720;
            SDL_DestroyWindow(*Window);
            *Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            *WindowSurface = SDL_GetWindowSurface(*Window);
        }
        if (HighlightedResolution[2] == true && Return_Key)
        {
            *WindowWidth = 1024;
            *WindowHight = 768;
            SDL_DestroyWindow(*Window);
            *Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            *WindowSurface = SDL_GetWindowSurface(*Window);
        }
        if (HighlightedResolution[3] == true && Return_Key)
        {
            *WindowWidth = 1280;
            *WindowHight = 960;
            SDL_DestroyWindow(*Window);
            *Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            *WindowSurface = SDL_GetWindowSurface(*Window);
        }
        if (HighlightedResolution[4] == true && Return_Key)
        {
            *WindowWidth = 1280;
            *WindowHight = 720;
            SDL_DestroyWindow(*Window);
            *Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            *WindowSurface = SDL_GetWindowSurface(*Window);
        }
        /*if (HighlightedResolution[1] == true)
        {
            *WindowWidth = 800;
            *WindowHight = 600;
            SDL_DestroyWindow(Window);
            Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, *WindowWidth, *WindowHight, 0);
            SDL_GL_MakeCurrent(Window, )

            glViewport()
            
            SDL_UpdateWindowSurface(Window);
        }*/

        if (HighlightedOption[1] == true)
        {
            if (RightButton)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (HighlightedOnOff[i] == true && i < (2 - 1) && i >= 0)
                    {
                        HighlightedOnOff[i + 1] = true;
                        HighlightedOnOff[i] = false;
                        break;
                    }
                }
            }
            if (LeftButton)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (HighlightedOnOff[i] == true && i <= (2 - 1) && i > 0)
                    {
                        HighlightedOnOff[i - 1] = true;
                        HighlightedOnOff[i] = false;
                        break;
                    }
                }
            }
        }
        if (HighlightedOption[2] == true)
        {
            if (RightButton)
            {
                for (int i = 2; i < 4; i++)
                {
                    if (HighlightedOnOff[i] == true && i < (4 - 1) && i >= 2)
                    {
                        HighlightedOnOff[i + 1] = true;
                        HighlightedOnOff[i] = false;
                        break;
                    }
                }
            }
            if (LeftButton)
            {
                for (int i = 2; i < 4; i++)
                {
                    if (HighlightedOnOff[i] == true && i <= (4 - 1) && i > 2)
                    {
                        HighlightedOnOff[i - 1] = true;
                        HighlightedOnOff[i] = false;
                        break;
                    }
                }
            }
        }
        if (HighlightedOnOff[2])
        {
            Mix_Volume(-1, 128);
        }
        if (HighlightedOnOff[3])
        {
            Mix_Volume(-1, 0);
        }
        if (HighlightedOnOff[0])
        {
            Mix_VolumeMusic(128);
        }
        if (HighlightedOnOff[1])
        {
            Mix_VolumeMusic(0);
        }

        if (HighlightedOption[MaxOptions - 1] == true && Return_Key)
        {
            break;
        }

        SDL_FillRect(*WindowSurface, 0, (255 << 24) | (50 << 16) | (50 << 8) | (50));

        for (int i = 0; i < MaxOptions; i++)
        {
            if (HighlightedOption[i] == false)
            {
                SDL_BlitSurface(OptionSurface[i], 0, *WindowSurface, &OptionRect[i]);
            }
            if (HighlightedOption[i] == true)
            {
                SDL_BlitSurface(HighlightedOptionSurface[i], 0, *WindowSurface, &OptionRect[i]);
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if (HighlightedResolution[i] == false)
            {
                SDL_BlitSurface(ResolutionSurface[i], 0, *WindowSurface, &ResolutionRect[i]);
            }
            if (HighlightedResolution[i] == true)
            {
                SDL_BlitSurface(HighlightedResolutionSurface[i], 0, *WindowSurface, &ResolutionRect[i]);
            }
        }
        for (int i = 0; i < MaxOnOff; i++)
        {
            if (HighlightedOnOff[i] == false)
            {
                SDL_BlitSurface(OnOffSurface[i], 0, *WindowSurface, &OnOffRect[i]);
            }
            if (HighlightedOnOff[i] == true)
            {
                SDL_BlitSurface(OnOffSelectedSurface[i], 0, *WindowSurface, &OnOffRect[i]);
            }
        }

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
            sprintf(NowRAM, "RAM usage: %.3f MB/ %.1f GB", currentRAM, totalRAM);
            //---------------------leak detector-------------------------------
            float RAM1, RAM2;
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
                if (on)
                {
                    RenderText(Bold, "Memory Leak Detected!", 255, 255, 0, *WindowWidth - 300, 25, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
                }
                if (on == false)
                {
                    RenderText(Bold, "Memory Leak Detected!", 255, 0, 0, *WindowWidth - 300, 25, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
                }
                count++;
            }

            RenderText(RegularS, NowFPS, 170, 170, 255, *WindowWidth - 60, 0, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
            RenderText(RegularS, NowRAM, 255, 255, 150, *WindowWidth - 300, 0, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
        } //------------------------------------------------------
        SDL_UpdateWindowSurface(*Window);
        for (int i = 0; i < MaxOptions; i++)
        {
            SDL_FreeSurface(OptionSurface[i]);
            SDL_FreeSurface(HighlightedOptionSurface[i]);
        }
        for (int i = 0; i < 5; i++)
        {
            SDL_FreeSurface(ResolutionSurface[i]);
            SDL_FreeSurface(HighlightedResolutionSurface[i]);
        }
        for (int i = 0; i < MaxOnOff; i++)
        {
            SDL_FreeSurface(OnOffSurface[i]);
            SDL_FreeSurface(OnOffSelectedSurface[i]);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////  MAIN MENU  ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainMenu(TTF_Font *Regular, TTF_Font *RegularS, TTF_Font *Bold, TTF_Font *Bold2, TTF_Font *Title1, TTF_Font *Title2, TTF_Font *Title1B, TTF_Font *Title2B,
              TTF_Font *Title3, TTF_Font *Title3B,
              SDL_Surface *TextSurface,
              SDL_Surface **WindowSurface,
              SDL_Window **Window,
              int *WindowWidth, int *WindowHight,
              bool *Playing)
{

    int frameIndex = 0;
    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Return_Key = false;

    SDL_Rect orPLAY;
    SDL_Color PlayColor = {255, 100, 100};
    SDL_Surface *osPLAY = TTF_RenderText_Blended(Title1, "Play", PlayColor);
    orPLAY.w = osPLAY->w;
    orPLAY.h = osPLAY->h;
    orPLAY.x = (*WindowWidth - osPLAY->w) / 2;
    orPLAY.y = *WindowHight / 2 - 100;
    bool PLAYselected = true;

    SDL_Rect orOPTIONS;
    SDL_Color MenuOptionsColor = {200, 200, 200};
    SDL_Surface *osOPTIONS = TTF_RenderText_Blended(Title2, "Options", MenuOptionsColor);
    orOPTIONS.w = osOPTIONS->w;
    orOPTIONS.h = osOPTIONS->h;
    orOPTIONS.x = (*WindowWidth - osOPTIONS->w) / 2;
    orOPTIONS.y = orPLAY.y + orPLAY.h;
    bool OPTIONSselected = false;

    SDL_Rect orEXIT;
    SDL_Surface *osEXIT = TTF_RenderText_Blended(Title2, "Exit", MenuOptionsColor);
    orEXIT.w = osEXIT->w;
    orEXIT.h = osEXIT->h;
    orEXIT.x = (*WindowWidth - osEXIT->w) / 2;
    orEXIT.y = orOPTIONS.y + orOPTIONS.h;
    bool EXITselected = false;

    while (MainMenuRunning)
    {
        RightButton = false;
        LeftButton = false;
        UpButton = false;
        DownButton = false;
        Return_Key = false;
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
        //--------------------------------------------------------------------------------------------------------------
        //FPS------------------------------------------------------
        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart = SDL_GetTicks();
        //---------------------------------------------------------
        SDL_Event Event;
        while (SDL_PollEvent(&Event)) //if pPollEvent returns 1 then we enter the while loop this
                                      //means thatif we have more than one event, it gathers them all before running
        {
            if (Event.type == SDL_QUIT)
            {
                GameIsRunning = false;
                MainMenuRunning = false;
            }
            if (Event.type == SDL_KEYDOWN)
            {
                if (Event.key.keysym.sym == SDLK_RETURN && Event.key.repeat == false)
                {
                    Return_Key = true;
                }
                if (Event.key.keysym.sym == SDLK_DOWN && Event.key.repeat == false)
                {
                    DownButton = true;
                }
                if (Event.key.keysym.sym == SDLK_UP && Event.key.repeat == false)
                {
                    UpButton = true;
                }
                if (Event.key.keysym.sym == SDLK_LEFT && Event.key.repeat == false)
                {
                    LeftButton = true;
                }
                if (Event.key.keysym.sym == SDLK_RIGHT && Event.key.repeat == false)
                {
                    RightButton = true;
                }
            }
        }

        SDL_FillRect(*WindowSurface, 0, (255 << 24) | (50 << 16) | (50 << 8) | (50));

        if (UpButton)
        {
            if (OPTIONSselected)
            {
                OPTIONSselected = false;
                PLAYselected = true;
            }
            if (EXITselected)
            {
                EXITselected = false;
                OPTIONSselected = true;
            }
        }

        if (DownButton)
        {
            if (OPTIONSselected)
            {
                OPTIONSselected = false;
                EXITselected = true;
            }
            if (PLAYselected)
            {
                PLAYselected = false;
                OPTIONSselected = true;
            }
        }

        if (PLAYselected && Return_Key)
        {
            *Playing = true;
            break;
        }
        if (EXITselected && Return_Key)
        {
            GameIsRunning = false;
            MainMenuRunning = false;
            break;
        }
        if (OPTIONSselected && Return_Key)
        {
            OptionsMenu(Regular, RegularS, Bold, Bold2, Title1, Title2, Title1B, Title2B, Title3, Title3B,
                        TextSurface, WindowSurface, Window, WindowWidth, WindowHight, Playing);
        }
        if (GameIsRunning == false)
        {
            break;
        }

        if (PLAYselected == false)
        {
            SDL_Surface *osPLAY = TTF_RenderText_Blended(Title1, "Play", PlayColor);
        }
        if (PLAYselected == true)
        {
            SDL_Surface *osPLAY = TTF_RenderText_Blended(Title1B, "Play", PlayColor);
        }
        orPLAY.w = osPLAY->w;
        orPLAY.h = osPLAY->h;
        orPLAY.x = (*WindowWidth - osPLAY->w) / 2;
        orPLAY.y = *WindowHight / 2 - 100;

        if (OPTIONSselected == false)
        {
            osOPTIONS = TTF_RenderText_Blended(Title2, "Options", MenuOptionsColor);
        }
        if (OPTIONSselected == true)
        {
            osOPTIONS = TTF_RenderText_Blended(Title2B, "Options", MenuOptionsColor);
        }
        orOPTIONS.w = osOPTIONS->w;
        orOPTIONS.h = osOPTIONS->h;
        orOPTIONS.x = (*WindowWidth - osOPTIONS->w) / 2;
        orOPTIONS.y = orPLAY.y + orPLAY.h;

        if (EXITselected == false)
        {
            osEXIT = TTF_RenderText_Blended(Title2, "Exit", MenuOptionsColor);
        }
        if (EXITselected == true)
        {
            osEXIT = TTF_RenderText_Blended(Title2B, "Exit", MenuOptionsColor);
        }
        orEXIT.w = osEXIT->w;
        orEXIT.h = osEXIT->h;
        orEXIT.x = (*WindowWidth - osEXIT->w) / 2;
        orEXIT.y = orOPTIONS.y + orOPTIONS.h;

        SDL_BlitSurface(osPLAY, 0, *WindowSurface, &orPLAY);
        SDL_BlitSurface(osOPTIONS, 0, *WindowSurface, &orOPTIONS);
        SDL_BlitSurface(osEXIT, 0, *WindowSurface, &orEXIT);

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
            sprintf(NowRAM, "RAM usage: %.3f MB/ %.1f GB", currentRAM, totalRAM);
            //---------------------leak detector-------------------------------
            float RAM1, RAM2;
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
                if (on)
                {
                    RenderText(Bold, "Memory Leak Detected!", 255, 255, 0, *WindowWidth - 300, 25, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
                }
                if (on == false)
                {
                    RenderText(Bold, "Memory Leak Detected!", 255, 0, 0, *WindowWidth - 300, 25, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
                }
                count++;
            }

            RenderText(RegularS, NowFPS, 170, 170, 255, *WindowWidth - 60, 0, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
            RenderText(RegularS, NowRAM, 255, 255, 150, *WindowWidth - 300, 0, TextSurface, *WindowSurface, *WindowWidth, *WindowHight);
        } //------------------------------------------------------
        SDL_UpdateWindowSurface(*Window);

        SDL_FreeSurface(osEXIT);
        SDL_FreeSurface(osOPTIONS);
        SDL_FreeSurface(osPLAY);
    }
    Mix_ResumeMusic();
    Mix_Resume(2);
}