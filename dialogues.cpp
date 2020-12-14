#pragma once
#include "raggy.hpp"
#include "source.cpp"

void Dialogue(TTF_Font *Regular, TTF_Font *Bold, TTF_Font *Bold2,
              SDL_Surface *TextSurface,
              SDL_Surface *WindowSurface,
              SDL_Window *Window,
              int *WindowWidth, int *WindowHight)
{
    bool DialogueRunning = true;

    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Shift = false;
    bool F_Key = false;
    bool H_Key = false;
    bool E_Key = false;

    SDL_Rect View;
    View.w = *WindowWidth;
    View.h = (*WindowHight / 3);
    View.x = 0;
    View.y = 60;

    SDL_Rect NPCtext;
    NPCtext.w = *WindowWidth;
    NPCtext.h = 75;
    NPCtext.x = 0;
    NPCtext.y = 60 + View.h;

    SDL_Rect PlayerText;
    PlayerText.w = *WindowWidth;
    PlayerText.h = 75;
    PlayerText.x = 0;
    PlayerText.y = 60 + View.h + NPCtext.h;

    SDL_Rect Options;
    Options.w = *WindowWidth;
    Options.h = *WindowHight - (60 + View.h + NPCtext.h + PlayerText.h);
    Options.x = 0;
    Options.y = 60 + View.h + NPCtext.h + PlayerText.h;

    //---------Options------------

    bool SelectedOption[12];
    for (int i = 0; i < 12; i++)
    {
        SelectedOption[i] = false;
    }

    SelectedOption[0] = true;

    SDL_Color DialogueColor = {255, 255, 255};

    char *text[12];
    char *o[12];
    char *OptionText[12];

    text[0] = "Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you ";
    text[1] = "NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA NA BATMAAAAAN";
    text[2] = "Bzzzzt Static........ bzzzzzt Dynamic.......";
    text[3] = "I'm in love with the shape of you, and that shape is a four dimentional polychoron in a non eucledian plane. sexy.";
    text[4] = "THIS IS NOT SPARTAAAAA and I cannot stress this enough. I am very stressed. what am I doing with my life. is this sparta? morelike, WHAT is sparta? how and when can I know? but wait a minute, if I can't see sparta... do I exist?";
    text[5] = "Place your ad here";
    text[6] = "Place your ad above. how many adspaces do you want asshole?";
    text[7] = "Cyberpunk sucks major ass, my disappointment in that game is immense and my day was ruined";
    text[8] = "God how many of these do you expectg me to write?";
    text[9] = "Site is under construction. Error 404, 501, bad gateway, connection timeout. just fuck off";
    text[10] = "I have information that could lead to hillary's arrest.";
    text[11] = "DO YOU KNOW DE WAE? who does... who does... what is life worth anymore? nothing. goodbye cruel world..... no I'm just going to sleep. tell me what yhou fink!";

    o[0] = "1) ";
    o[1] = "2) ";
    o[2] = "3) ";
    o[3] = "4) ";
    o[4] = "5) ";
    o[5] = "6) ";
    o[6] = "7) ";
    o[7] = "8) ";
    o[8] = "9) ";
    o[9] = "10) ";
    o[10] = "11) ";
    o[11] = "12) ";

    SDL_Surface *OptionSurface[12];
    SDL_Surface *SelectedOptionSurface[12];
    SDL_Rect Option[12];
    for (int i = 0; i < 12; i++)
    {
        OptionText[i] = (char *)malloc(1 + strlen(o[i]) + strlen(text[i]));
        strcpy(OptionText[i], o[i]);
        strcat(OptionText[i], text[i]);

        OptionSurface[i] = TTF_RenderText_Blended_Wrapped(Regular, OptionText[i], DialogueColor, (*WindowWidth - 40));
        SelectedOptionSurface[i] = TTF_RenderText_Blended_Wrapped(Bold, OptionText[i], DialogueColor, (*WindowWidth - 40));
        Option[i].h = OptionSurface[i]->h;
        Option[i].w = OptionSurface[i]->w;
        Option[i].x = 20;
    }

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

    SDL_Event Event;

    while (DialogueRunning)
    {

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
                }
                if (Event.key.keysym.sym == SDLK_DOWN && Event.key.repeat == false)
                {
                    for (int i = 0; i < 12; i++)
                    {
                        if (SelectedOption[i] == true && i < 11 && i >= 0)
                        {
                            SelectedOption[i + 1] = true;
                            SelectedOption[i] = false;
                            break;
                        }
                    }
                }
                if (Event.key.keysym.sym == SDLK_UP && Event.key.repeat == false)
                {
                    for (int i = 0; i < 12; i++)
                    {
                        if (SelectedOption[i] == true && i <= 11 && i > 0)
                        {
                            SelectedOption[i - 1] = true;
                            SelectedOption[i] = false;
                            break;
                        }
                    }
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
        int R = 50;
        int G = 50;
        int B = 50;
        int A = 255;

        SDL_FillRect(WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));

        SDL_FillRect(WindowSurface, &Options, (255 << 24) | (75 << 16) | (100 << 8) | (75));

        for (int i = 0; i < 12; i++)
        {

            if (SelectedOption[i] == false)
            {
                SDL_BlitSurface(OptionSurface[i], 0, WindowSurface, &Option[i]);
            }
            if (SelectedOption[i] == true)
            {
                if ((Option[i].y + OptionSurface[i]->h) > *WindowHight)
                {
                    for (int t = 0; t < 12; t++)
                    {
                        Option[t].y = Option[t].y - OptionSurface[i]->h;
                    }
                }
                if ((Option[i].y) < (PlayerText.y + PlayerText.h))
                {
                    for (int t = 0; t < 12; t++)
                    {
                        Option[t].y = Option[t].y + OptionSurface[i]->h;
                    }
                }
                SDL_BlitSurface(SelectedOptionSurface[i], 0, WindowSurface, &Option[i]);
            }
        }
        SDL_FillRect(WindowSurface, &View, (255 << 24) | (200 << 16) | (200 << 8) | (200));
        SDL_FillRect(WindowSurface, &NPCtext, (255 << 24) | (255 << 16) | (150 << 8) | (150));
        SDL_FillRect(WindowSurface, &PlayerText, (255 << 24) | (150 << 16) | (150 << 8) | (255));

        RenderTextCenteredX(Bold2, "dialogue title", 255, 255, 255, 0, 10, TextSurface, WindowSurface, *WindowWidth, *WindowHight);

        RenderTextDialogue(Regular,
                           "This is stuff that the NPC says anfd it is limited in this case to around 250 characters."
                           "This is stuff that the NPC says and it is limited in this case to around 250 characters.",
                           255, 255, 255, 20, (NPCtext.y + 10), TextSurface, WindowSurface, *WindowWidth, *WindowHight, (*WindowWidth - 40));
        RenderTextDialogue(Regular,
                           "This is stuff that the Player says anfd it is limited in this case to around 250 characters."
                           "This is stuff that the Player says and it is limited in this case to around 250 characters.",
                           255, 255, 255, 20, (PlayerText.y + 10), TextSurface, WindowSurface, *WindowWidth, *WindowHight, (*WindowWidth - 40));

        SDL_UpdateWindowSurface(Window);
    };
}