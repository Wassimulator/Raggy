#pragma once
#include "raggy.hpp"
#include "source.cpp"
#include "Ahole.cpp"

void DialogueMode(TTF_Font *Regular, TTF_Font *RegularS, TTF_Font *Bold, TTF_Font *Bold2,
                  SDL_Surface *TextSurface,
                  SDL_Surface *WindowSurface,
                  SDL_Window *Window,
                  int *WindowWidth, int *WindowHight,
                  player *Player, Mix_Music *BackgroundMusic)
{
    dialogues Dialogue;

    int frameIndex = 0;
    bool DialogueRunning = true;

    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Shift = false;
    bool F_Key = false;
    bool H_Key = false;
    bool E_Key = false;
    bool Return_Key = false;

    SDL_Rect HeaderRect;
    HeaderRect.w = *WindowWidth;
    HeaderRect.h = 60;
    HeaderRect.x = 0;
    HeaderRect.y = 0;

    SDL_Rect ViewRect;
    ViewRect.w = *WindowWidth;
    ViewRect.h = (*WindowHight / 3);
    ViewRect.x = 0;
    ViewRect.y = 60;

    SDL_Rect NPCtextRect;
    NPCtextRect.w = *WindowWidth;
    NPCtextRect.h = 75;
    NPCtextRect.x = 0;
    NPCtextRect.y = 60 + ViewRect.h;

    SDL_Rect PlayerTextRect;
    PlayerTextRect.w = *WindowWidth;
    PlayerTextRect.h = 75;
    PlayerTextRect.x = 0;
    PlayerTextRect.y = 60 + ViewRect.h + NPCtextRect.h;

    SDL_Rect Options;
    Options.w = *WindowWidth;
    Options.h = *WindowHight - (60 + ViewRect.h + NPCtextRect.h + PlayerTextRect.h);
    Options.x = 0;
    Options.y = 60 + ViewRect.h + NPCtextRect.h + PlayerTextRect.h;

    SDL_Rect ViewActiveRect;

    //---------Options------------

    for (int i = 0; i < 12; i++)
    {
        Dialogue.HighlightedOption[i] = false;
    }

    Dialogue.HighlightedOption[0] = true;

    SDL_Color DialogueColor = {255, 255, 255};

    char *text[12];
    char *o[12];
    char *OptionText[12];

    OptionText[0] = "Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you ";
    OptionText[1] = "NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA BATMAAAAAN";
    OptionText[2] = "Bzzzzt Static........ bzzzzzt Dynamic.......";
    OptionText[3] = "I'm in love with the shape of you, and that shape is a four dimentional polychoron in a non eucledian plane. sexy.";
    OptionText[4] = "THIS IS NOT SPARTAAAAA and I cannot stress this enough. I am very stressed. what am I doing with my life. is this sparta? morelike, WHAT is sparta? how and when can I know? but wait a minute, if I can't see sparta... do I exist?";
    OptionText[5] = "Place your ad here";
    OptionText[6] = "Place your ad above. how many adspaces do you want asshole?";
    OptionText[7] = "Cyberpunk sucks major ass, my disappointment in that game is immense and my day was ruined";
    OptionText[8] = "God how many of these do you expectg me to write?";
    OptionText[9] = "Site is under construction. Error 404, 501, bad gateway, connection timeout. just fuck off";
    OptionText[10] = "I have information that could lead to hillary's arrest.";
    OptionText[11] = "DO YOU KNOW DE WAE? who does... who does... what is life worth anymore? nothing. goodbye cruel world..... no I'm just going to sleep. tell me what yhou fink!";

    o[0] = "  1) ";
    o[1] = "  2) ";
    o[2] = "  3) ";
    o[3] = "  4) ";
    o[4] = "  5) ";
    o[5] = "  6) ";
    o[6] = "  7) ";
    o[7] = "  8) ";
    o[8] = "  9) ";
    o[9] = "10) ";
    o[10] = "11) ";
    o[11] = "12) ";

    SDL_Surface *OptionSurface[12];
    SDL_Surface *OptionNumSurface[12];
    SDL_Surface *SelectedOptionSurface[12];
    SDL_Surface *SelectedOptionNumSurface[12];
    SDL_Rect OptionRect[12];
    SDL_Rect OptionNumRect[12];

    SDL_Event Event;

    int PQT = 1;
    int PQTt = 0;

    //--------------------------define NPCs here----------------------
    dialogueNPC Ahole;
    NPCsounds AholeS;
    AholeS.Node[2] = Mix_LoadWAV("data/sounds/ahole/2.WAV");
    AholeS.Node[3] = Mix_LoadWAV("data/sounds/ahole/3.WAV");
    AholeS.Node[4] = Mix_LoadWAV("data/sounds/ahole/4.WAV");
    AholeS.Node[5] = Mix_LoadWAV("data/sounds/ahole/5.WAV");
    AholeS.Node[6] = Mix_LoadWAV("data/sounds/ahole/6.WAV");
    AholeS.Node[7] = Mix_LoadWAV("data/sounds/ahole/7.WAV");

    Ahole.IdleView = LoadSprite("data/textures/ahole_dialogue_idle_anim.png");
    Ahole.TalkView = LoadSprite("data/textures/ahole_dialogue_talk_anim.png");
    //----------------------------------------------------------------

    bool firstrun = true;
    bool refresh = true;
    bool isTalking = false;

    for (int i = 0; i < 12; i++)
    {
        Dialogue.SelectedOption[i] = false;
    }

    while (DialogueRunning)
    {
        
        E_Key = false;
        F_Key = false;
        Return_Key = false;
        DownButton = false;
        UpButton = false;
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
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(BackgroundMusic, 3);
        }

        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_QUIT)
            {
                GameIsRunning = false;
                DialogueRunning = false;
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
                    DialogueRunning = false;
                    Player->Chatting = false;
                    Player->AholeLevel = 0;
                    Mix_HaltChannel(2); //stop the NPC talking
                }
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
            }

            //cool shit:
            if (Event.type == SDL_KEYDOWN || Event.type == SDL_KEYUP)
            {
                bool KeyState = Event.type == SDL_KEYDOWN ? true : false;
                switch (Event.key.keysym.sym)
                {
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
        //--------------------------------------------------------------
        for (int i = 0; i < Dialogue.MaxOptions; i++)
        {
            if (Dialogue.HighlightedOption[i] == true && Return_Key == true)
            {
                Dialogue.SelectedOption[i] = true;
                refresh = true;
            }
        }
        //-----------------------Update---------------------------------
        if (Player->ChattingAhole == true)
        {
            AholeDialogue(&Dialogue, Player, &Ahole, &AholeS, &firstrun, &refresh, &isTalking);
        }
        if (refresh)
        {
            for (int i = 0; i < Dialogue.MaxOptions; i++)
            {
                OptionText[i] = Dialogue.Option[i].Text;
            }
            for (int i = 0; i < 12; i++)
            {
                Dialogue.HighlightedOption[i] = false;
            }
            Dialogue.HighlightedOption[0] = true;
        }
        for (int i = 0; i < Dialogue.MaxOptions; i++)
        {
            /*OptionText[i] = (char *)malloc(1 + strlen(o[i]) + strlen(Dialogue.Option[i].Text));
            strcpy(OptionText[i], o[i]);
            strcat(OptionText[i], Dialogue.Option[i].Text);*/
            //TODO: leave this to show to Said, this is the reason I had memory corruption.

            OptionSurface[i] = TTF_RenderText_Blended_Wrapped(Regular, OptionText[i], DialogueColor, (*WindowWidth - 60));
            OptionNumSurface[i] = TTF_RenderText_Blended_Wrapped(Regular, o[i], DialogueColor, 30);
            SelectedOptionSurface[i] = TTF_RenderText_Blended_Wrapped(Bold, OptionText[i], DialogueColor, (*WindowWidth - 60));
            SelectedOptionNumSurface[i] = TTF_RenderText_Blended_Wrapped(Bold, o[i], DialogueColor, 30);

            OptionRect[i].h = OptionSurface[i]->h;
            OptionRect[i].w = OptionSurface[i]->w;
            OptionNumRect[i].h = OptionNumSurface[i]->h;
            OptionNumRect[i].w = OptionNumSurface[i]->w;
        }
        if (refresh)
        {
            UpdateOptionRects(OptionRect, Options, OptionNumRect);
        }
        refresh = false;
        for (int i = 0; i < 12; i++)
        {
            OptionRect[i].x = 40;
            OptionNumRect[i].y = OptionRect[i].y;
            OptionNumRect[i].x = 0;
        }
        //--------------------------------------------------------------
        for (int i = 0; i < 12; i++)
        {
            Dialogue.SelectedOption[i] = false;
        }
        if (DownButton)
        {
            for (int i = 0; i < Dialogue.MaxOptions; i++)
            {
                if (Dialogue.HighlightedOption[i] == true && i < (Dialogue.MaxOptions - 1) && i >= 0)
                {
                    Dialogue.HighlightedOption[i + 1] = true;
                    Dialogue.HighlightedOption[i] = false;
                    break;
                }
            }
        }
        if (UpButton)
        {
            for (int i = 0; i < Dialogue.MaxOptions; i++)
            {
                if (Dialogue.HighlightedOption[i] == true && i <= (Dialogue.MaxOptions - 1) && i > 0)
                {
                    Dialogue.HighlightedOption[i - 1] = true;
                    Dialogue.HighlightedOption[i] = false;
                    break;
                }
            }
        }
        //--------------------------------------------------------------
        int R = 50;
        int G = 50;
        int B = 50;
        int A = 255;

        SDL_FillRect(WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));

        SDL_FillRect(WindowSurface, &Options, (255 << 24) | (75 << 16) | (100 << 8) | (75));

        for (int i = 0; i < Dialogue.MaxOptions; i++)
        {
            if (Dialogue.HighlightedOption[i] == false)
            {
                SDL_BlitSurface(OptionSurface[i], 0, WindowSurface, &OptionRect[i]);
                SDL_BlitSurface(OptionNumSurface[i], 0, WindowSurface, &OptionNumRect[i]);
            }
            if (Dialogue.HighlightedOption[i] == true)
            {
                if ((OptionRect[i].y + OptionSurface[i]->h) > *WindowHight)
                {
                    for (int t = 0; t < Dialogue.MaxOptions; t++)
                    {
                        OptionRect[t].y = OptionRect[t].y - OptionSurface[i]->h;
                        //OptionNumRect[t].y = OptionRect[t].y - OptionSurface[i]->h;
                    }
                }
                if ((OptionRect[i].y) < (PlayerTextRect.y + PlayerTextRect.h))
                {
                    for (int t = 0; t < Dialogue.MaxOptions; t++)
                    {
                        OptionRect[t].y = OptionRect[t].y + OptionSurface[i]->h;
                        //OptionNumRect[t].y = OptionRect[t].y + OptionSurface[i]->h;
                    }
                }
                SDL_BlitSurface(SelectedOptionSurface[i], 0, WindowSurface, &OptionRect[i]);
                SDL_BlitSurface(SelectedOptionNumSurface[i], 0, WindowSurface, &OptionNumRect[i]);
            }
        }
        SDL_FillRect(WindowSurface, &HeaderRect, (200 << 24) | (50 << 16) | (50 << 8) | (50));
        SDL_FillRect(WindowSurface, &ViewRect, (255 << 24) | (200 << 16) | (200 << 8) | (200));
        SDL_FillRect(WindowSurface, &NPCtextRect, (255 << 24) | (255 << 16) | (150 << 8) | (150));
        SDL_FillRect(WindowSurface, &PlayerTextRect, (255 << 24) | (150 << 16) | (150 << 8) | (255));

        if (isTalking == false)
        {

            if (PQTt++ % 30 == 0) //idle movement, yeah it's slow.
            {
                if (PQT >= 1 && PQT < 6)
                {
                    PQT++;
                }
                else
                {
                    PQT = 1;
                }
            }
        }
        if (isTalking == true)
        {

            if (PQTt++ % 5 == 0) //talk movement, it be fast.
            {
                if (PQT >= 1 && PQT < 6)
                {
                    PQT++;
                }
                else
                {
                    PQT = 1;
                }
            }
        }

        int SixCounterP = (PQT - 1) % 3;
        int SixCounterQ = (PQT - 1) / 3;
        ViewActiveRect.x = SixCounterP * 256;
        ViewActiveRect.y = SixCounterQ * 64;
        ViewActiveRect.w = 256;
        ViewActiveRect.h = 64;

        SDL_BlitScaled(Dialogue.View.Surface, &ViewActiveRect, WindowSurface, &ViewRect);

        RenderTextCenteredX(Bold2, Dialogue.DialogueTitle, 255, 255, 255, 0, 10, TextSurface, WindowSurface, *WindowWidth, *WindowHight);

        RenderTextDialogue(Regular, Dialogue.NPCtext, 255, 255, 255, 20, (NPCtextRect.y + 10), TextSurface, WindowSurface, *WindowWidth, *WindowHight, (*WindowWidth - 40));
        RenderTextDialogue(Regular, Dialogue.PlayerText, 255, 255, 255, 20, (PlayerTextRect.y + 10), TextSurface, WindowSurface, *WindowWidth, *WindowHight, (*WindowWidth - 40));

        RenderText(Regular, "Press Tab to exit Dialogue mode", 255, 255, 255, 0, 0, TextSurface, WindowSurface, *WindowWidth, *WindowHight);

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
                    RenderText(Bold, "Memory Leak Detected!", 255, 255, 0, *WindowWidth - 300, 25, TextSurface, WindowSurface, *WindowWidth, *WindowHight);
                }
                if (on == false)
                {
                    RenderText(Bold, "Memory Leak Detected!", 255, 0, 0, *WindowWidth - 300, 25, TextSurface, WindowSurface, *WindowWidth, *WindowHight);
                }
                count++;
            }

            RenderText(RegularS, NowFPS, 170, 170, 255, *WindowWidth - 60, 0, TextSurface, WindowSurface, *WindowWidth, *WindowHight);
            RenderText(RegularS, NowRAM, 255, 255, 150, *WindowWidth - 300, 0, TextSurface, WindowSurface, *WindowWidth, *WindowHight);
        } //------------------------------------------------------

        SDL_UpdateWindowSurface(Window);

        for (int i = 0; i < Dialogue.MaxOptions; i++) //plugging dem memory leaks
        {
            //free(OptionText[i]);
            SDL_FreeSurface(OptionSurface[i]);
            SDL_FreeSurface(OptionNumSurface[i]);
            SDL_FreeSurface(SelectedOptionSurface[i]);
            SDL_FreeSurface(SelectedOptionNumSurface[i]);
        }
    };
}