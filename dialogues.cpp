#pragma once
#include "raggy.hpp"
#include "source.cpp"
#include "Ahole.cpp"
#include "menus.cpp"


void PollEventsDialogue(SDL_Event *Event, keys *K, bool *DialogueRunning, player* Player)
{
    while (SDL_PollEvent(Event))
    {
        if (Event->type == SDL_QUIT)
        {
            GameIsRunning = false;
            *DialogueRunning = false;
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
                Player->Chatting = false;
            }
            if (Event->key.keysym.sym == SDLK_RETURN && Event->key.repeat == false)
            {
                K->Return_Key = true;
            }
            if (Event->key.keysym.sym == SDLK_DOWN && Event->key.repeat == false)
            {
                K->DownButton = true;
            }
            if (Event->key.keysym.sym == SDLK_UP && Event->key.repeat == false)
            {
                K->UpButton = true;
            }
            if (Event->key.keysym.sym == SDLK_ESCAPE && Event->key.repeat == false)
            {
                Playing = false;
                Mix_PauseMusic();
                Mix_Pause(2);
            }
        }

        //cool shit:
        if (Event->type == SDL_KEYDOWN || Event->type == SDL_KEYUP)
        {
            bool KeyState = Event->type == SDL_KEYDOWN ? true : false;
            switch (Event->key.keysym.sym)
            {
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
void DialogueMode(fonts F,
                  SDL_Surface **TextSurface,
                  SDL_Surface **WindowSurface,
                  SDL_Window **Window,
                  int *WindowWidth, int *WindowHeight,
                  player *Player, Mix_Music *BackgroundMusic, bool *MusicBool, bool *SoundBool)
{
    dialogues Dialogue;

    int frameIndex = 0;
    bool DialogueRunning = true;

    SDL_Rect HeaderRect;
    SDL_Rect ViewRect;
    SDL_Rect NPCtextRect;
    SDL_Rect PlayerTextRect;
    SDL_Rect Options;

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
    bool AholeSLoaded = false;

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

    bool Playing = true;

    ResetFades();

    string FileString;
    debug D;
    keys K;

    while (DialogueRunning)
    {
        if (GameIsRunning == false)
        {
            break;
        }
        K.E_Key = false;
        K.F_Key = false;
        K.Return_Key = false;
        K.DownButton = false;
        K.UpButton = false;
        K.F1_Key = false;
        //-------------------Getting Resource usage---------------------------------------------------------------------
        DebugPre(&D);
        //---------------------------------------------------------

        if (Playing == false)
        {
            MainMenu(F, TextSurface, WindowSurface, Window, WindowWidth, WindowHeight, &Playing,
                     MusicBool, SoundBool);
        }

        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(BackgroundMusic, 30);
        }
        if (*MusicBool == true)
        {
            Mix_VolumeMusic(128);
        }
        if (*MusicBool == false)
        {
            Mix_VolumeMusic(0);
        }
        if (*SoundBool == true)
        {
            Mix_Volume(-1, 128);
        }
        if (*SoundBool == false)
        {
            Mix_Volume(-1, 0);
        }

        PollEventsDialogue(&Event, &K, &DialogueRunning, Player);

        //--------------------------------------------------------------
        for (int i = 0; i < Dialogue.MaxOptions; i++)
        {
            if (Dialogue.HighlightedOption[i] == true && K.Return_Key == true)
            {
                Dialogue.SelectedOption[i] = true;
                refresh = true;
            }
        }
        //-----------------------Update---------------------------------
        if (Player->ChattingNPC == true)
        {
            AholeMain(&AholeSLoaded, &AholeS, &Dialogue, Player, &Ahole,
                      &firstrun, &refresh, &isTalking, &FileString);
        }
        if (refresh)
        {
            for (int i = 0; i < Dialogue.MaxOptions; i++)
            {
                const char *temp = Dialogue.Option[i].Text.c_str();
                OptionText[i] = const_cast<char *>(temp);
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

            OptionSurface[i] = TTF_RenderText_Blended_Wrapped(F.Regular, OptionText[i], DialogueColor, (*WindowWidth - 60));
            OptionNumSurface[i] = TTF_RenderText_Blended_Wrapped(F.Regular, o[i], DialogueColor, 30);
            SelectedOptionSurface[i] = TTF_RenderText_Blended_Wrapped(F.Bold, OptionText[i], DialogueColor, (*WindowWidth - 60));
            SelectedOptionNumSurface[i] = TTF_RenderText_Blended_Wrapped(F.Bold, o[i], DialogueColor, 30);

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
        if (K.DownButton)
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
        if (K.UpButton)
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
        HeaderRect.w = *WindowWidth;
        HeaderRect.h = 60;
        HeaderRect.x = 0;
        HeaderRect.y = 0;

        ViewRect.w = *WindowWidth;
        ViewRect.h = (*WindowHeight / 3);
        ViewRect.x = 0;
        ViewRect.y = 60;

        NPCtextRect.w = *WindowWidth;
        NPCtextRect.h = 75;
        NPCtextRect.x = 0;
        NPCtextRect.y = 60 + ViewRect.h;

        PlayerTextRect.w = *WindowWidth;
        PlayerTextRect.h = 75;
        PlayerTextRect.x = 0;
        PlayerTextRect.y = 60 + ViewRect.h + NPCtextRect.h;

        Options.w = *WindowWidth;
        Options.h = *WindowHeight - (60 + ViewRect.h + NPCtextRect.h + PlayerTextRect.h);
        Options.x = 0;
        Options.y = 60 + ViewRect.h + NPCtextRect.h + PlayerTextRect.h;

        int R = 50;
        int G = 50;
        int B = 50;
        int A = 255;

        SDL_FillRect(*WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));

        SDL_FillRect(*WindowSurface, &Options, (255 << 24) | (75 << 16) | (100 << 8) | (75));

        for (int i = 0; i < Dialogue.MaxOptions; i++)
        {
            if (Dialogue.HighlightedOption[i] == false)
            {
                SDL_BlitSurface(OptionSurface[i], 0, *WindowSurface, &OptionRect[i]);
                SDL_BlitSurface(OptionNumSurface[i], 0, *WindowSurface, &OptionNumRect[i]);
            }
            if (Dialogue.HighlightedOption[i] == true)
            {
                if ((OptionRect[i].y + OptionSurface[i]->h) > *WindowHeight)
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
                SDL_BlitSurface(SelectedOptionSurface[i], 0, *WindowSurface, &OptionRect[i]);
                SDL_BlitSurface(SelectedOptionNumSurface[i], 0, *WindowSurface, &OptionNumRect[i]);
            }
        }
        SDL_FillRect(*WindowSurface, &HeaderRect, (200 << 24) | (50 << 16) | (50 << 8) | (50));
        SDL_FillRect(*WindowSurface, &ViewRect, (255 << 24) | (200 << 16) | (200 << 8) | (200));
        SDL_FillRect(*WindowSurface, &NPCtextRect, (255 << 24) | (255 << 16) | (150 << 8) | (150));
        SDL_FillRect(*WindowSurface, &PlayerTextRect, (255 << 24) | (150 << 16) | (150 << 8) | (255));

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

        SDL_BlitScaled(Dialogue.View.Surface, &ViewActiveRect, *WindowSurface, &ViewRect);

        RenderTextCenteredX(F.Bold2, Dialogue.DialogueTitle, 255, 255, 255, 0, 10, *TextSurface, *WindowSurface, *WindowWidth, *WindowHeight);

        RenderTextDialogue(F.Regular, Dialogue.NPCtext, 255, 255, 255, 20, (NPCtextRect.y + 10), *TextSurface, *WindowSurface, *WindowWidth, *WindowHeight, (*WindowWidth - 40));
        RenderTextDialogue(F.Regular, Dialogue.PlayerText, 255, 255, 255, 20, (PlayerTextRect.y + 10), *TextSurface, *WindowSurface, *WindowWidth, *WindowHeight, (*WindowWidth - 40));

        RenderText(F.Regular, "Press Tab to exit Dialogue mode", 255, 255, 255, 0, 0, *TextSurface, *WindowSurface, *WindowWidth, *WindowHeight);

        if (FadeOut == true)
        {
            SDL_Surface *FadeSurface = SDL_CreateRGBSurface(0, *WindowWidth, *WindowHeight, 32, 0, 0, 0, 255);
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
            SDL_BlitSurface(FadeSurface, 0, *WindowSurface, 0);
            SDL_FreeSurface(FadeSurface);
        }
        if (FadeIn == true)
        {
            SDL_Surface *FadeSurface = SDL_CreateRGBSurface(0, *WindowWidth, *WindowHeight, 32, 0, 0, 0, 255);
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
            SDL_BlitSurface(FadeSurface, 0, *WindowSurface, 0);
            SDL_FreeSurface(FadeSurface);
        }
        if (Player->Chatting == false)
        {
            if (FadedIn == false && FadeIn == false)
            {
                FadeIn = true;
            }
            if (FadedIn == true && FadeIn == false)
            {
                DialogueRunning = false;
                Mix_HaltChannel(2);
                FadedIn = false;
            }
        }

        //FPS and Resources------------------------------------------------------
        FPSdelay(&D);
        DebugPost(&D, &F, &K); //------------------------------------------------------

        SDL_UpdateWindowSurface(*Window);

        for (int i = 0; i < Dialogue.MaxOptions; i++) //plugging dem memory leaks
        {
            //free(OptionText[i]);
            SDL_FreeSurface(OptionSurface[i]);
            SDL_FreeSurface(OptionNumSurface[i]);
            SDL_FreeSurface(SelectedOptionSurface[i]);
            SDL_FreeSurface(SelectedOptionNumSurface[i]);
        }
    };
    //------------Memory clean up-------------------------------
    for (int i = 0; i < 100; i++) //TODO: set max voice node chunk number
    {
        Mix_FreeChunk(AholeS.Node[i]);
    }
    SDL_FreeSurface(Ahole.IdleView.Surface);
    SDL_FreeSurface(Ahole.TalkView.Surface);
}
