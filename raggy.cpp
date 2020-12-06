#include "raggy.hpp"
#include "source.cpp"
#include "rects.cpp"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *Window;

    int WindowWidth = 800;
    int WindowHight = 640;

    Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHight, 0);
    SDL_Surface *WindowSurface = SDL_GetWindowSurface(Window);

    int frameIndex = 0;

    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;
    bool Shift = false;
    bool F_Key = false;
    bool H_Key = false;
    bool E_Key = false;


    bool ChattingAhole = false;
    //-------------------Load Objects--------------------------

    player Player = LoadPlayer();
    map Map = LoadMap();
    door Door = LoadDoor();
    fart PlayerFart = LoadFart();
    fartCloud PlayerFartCloud[20];
    for (FCi = 0; FCi < 20; FCi++)
    {
        PlayerFartCloud[FCi] = LoadFartCloud();
    }
    door DT[10];
    for (DTi = 0; DTi < 10; DTi++)
    {
        DT[DTi] = LoadDoor();
    }
    npc Ahole = LoadAhole();
    //---------------------------------------------------------

    Player.PosX = 0;
    Player.PosY = 0;

    bool running = true;

    Map.ActiveMap.h = Map.ActiveMap.h * 3;
    Map.ActiveMap.w = Map.ActiveMap.w * 3;

    //putting the map in the center of the screen:
    Map.PosX = -(Map.ActiveMap.w - WindowWidth) / 2; // -1000 + 800 / 2 = -100
    Map.PosY = -(Map.ActiveMap.h - WindowHight) / 2;

    int InitialMapPosX = Map.PosX;
    int InitialPlayerPosX = Player.PosX;

    int WalkSpeed = 2;
    int RunSpeed = 4; //They're now adjustable, I still don't see why you need them to be as such,
                      // but here, 1,5 hours later, I present you ADJUSTABLE SPEEDS * Confetti *

    //------load sounds--------

    Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 4096);
    sounds Sound = LoadSound();

    printf("Press H to say hello\nPress F to pay respects\n");

    float CamPosX = 0;

    //load fonts---
    TTF_Init();
    TTF_Font *Regular = TTF_OpenFont("data/fonts/PTSans-Regular.ttf", 20);
    TTF_Font *Bold = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 20);
    TTF_Font *Bold2 = TTF_OpenFont("data/fonts/PTSans-Bold.ttf", 24);
    SDL_Surface *TextSurface;
    //---
    SDL_Rect PlayerRect;
    PlayerRect.x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16);
    PlayerRect.y = (WindowHight / 2) - 48;
    PlayerRect.w = 32 * 3; // TODO: Scaling is an inherent problem that needs fixing.
    PlayerRect.h = 32 * 3;

    SDL_Rect MapRect;
    MapRect.h = Map.ActiveMap.h;
    MapRect.w = Map.ActiveMap.w;
    MapRect.x = ((WindowWidth - Map.ActiveMap.w) / 2) - CamPosX;
    MapRect.y = Map.PosY;

    SDL_Rect DoorRect;
    DoorRect.h = Door.Closed.h * 3;
    DoorRect.w = Door.Closed.w * 3;
    DoorRect.x = (WindowWidth / 2) - CamPosX - 48 - 200;
    DoorRect.y = (WindowHight / 2) - 96;

    SDL_Rect PlayerActiveRectangle;
    SDL_Rect PlayerFartRectR;
    SDL_Rect PlayerFartRectL;
    SDL_Rect PlayerFartActiveRect;
    SDL_Rect PlayerFartCloudRect[20];
    SDL_Rect PlayerFartCloudActiveRect[20];
    SDL_Rect DTRect[10];
    SDL_Rect AholeRect;

    //Game Loop-----------------------------------------------------------------
    while (running)
    {
        //leave these here. they're necessary for the "pressable once" system.
        E_Key = false;
        F_Key = false;

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
                running = false;
            }
            if (Event.key.keysym.sym == SDLK_q)
            {
                running = false;
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
        int R = 50;
        int G = 50;
        int B = 50;
        int A = 255;

        // WINDOWS USES BGRA (that does not stand for bulgaria)
        // TODO: map it so that it works on multiple systems
        SDL_FillRect(WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));

        float MapLimitR = Map.ActiveMap.w * 0.5f - 3 * 16;
        float MapLimitL = -Map.ActiveMap.w * 0.5f + 3 * 16;

        //-------------------------Game Update----------------------------------------------------------

        PlayerUpdate(&Player, CamPosX, RightButton, LeftButton, UpButton,
                     DownButton, Shift, MapLimitL, MapLimitR, WalkSpeed, RunSpeed);
        PlayerSoundUpdate(Sound, F_Key, H_Key);
        MapUpdate(&CamPosX, &Player);
        DoorUpdate(&Door, PlayerRect, DoorRect, Regular, TextSurface,
                   WindowSurface, WindowWidth, WindowHight, E_Key);

        FartUpdate(&Player, &PlayerFart, PlayerFartCloud, F_Key);

        DTUpdate(DT, PlayerRect, DTRect, Regular, TextSurface, WindowSurface, WindowWidth, WindowHight, E_Key);
        AholeUpdate(Player, PlayerRect, &AholeRect, Regular, TextSurface, WindowSurface, WindowWidth, WindowHight, E_Key);
        if(Player.ChattingAhole == true)
        {

        }
        //----------------------------LOAD RECTS HERE------------------------------------------
        //          IMPORTANT: make sure you update this function here and in rect.cpp
        //                     every time you add a new object! and define the rects
        //                     outside the loop.

        /*if (F_Key)
        {

            if (FartCloudReadIndex != (MaxFartClouds))
            {
                PlayerFartCloud[FartCloudReadIndex]; //ReadIndex is declared outside and initialized as Zero
                FartCloudReadIndex++;
                FartCloudBufferLength++;
            }
            if (FartCloudReadIndex == (MaxFartClouds))
            {
                FartCloudReadIndex = 1;
                FartCloudBufferLength = 20;
            }
        }*/
        LoadRects(WindowWidth, WindowHight, CamPosX, &F_Key,
                  &PlayerRect, &Player,
                  &PlayerActiveRectangle,
                  &MapRect, Map,
                  &DoorRect, Door,
                  &PlayerFartRectR, PlayerFart,
                  &PlayerFartRectL,
                  &PlayerFartActiveRect,
                  PlayerFartCloudRect, PlayerFartCloud,
                  PlayerFartCloudActiveRect,
                  DTRect, DT,
                  &AholeRect, Ahole);
        //-----------------------------Rendering-------------------------------------------------
        //          IMPORTANT: make sure you render the character last and the map first.
        //                     NPCs go before the player typically.

        SDL_BlitScaled(Map.ActiveMap.Surface, 0, WindowSurface, &MapRect);

        RenderTextCentered(Bold2, "This is a Game", 255, 255, 255, 0, -170, TextSurface, WindowSurface, WindowWidth, WindowHight);
        RenderText(Regular, "Press H to say hello", 255, 255, 255, 0, 0, TextSurface, WindowSurface, WindowWidth, WindowHight);
        RenderText(Regular, "Press F to pay respects", 255, 255, 255, 0, 25, TextSurface, WindowSurface, WindowWidth, WindowHight);
        RenderText(Regular, "Press Q to quit the game", 255, 255, 255, 0, 50, TextSurface, WindowSurface, WindowWidth, WindowHight);

        SDL_BlitScaled(Door.ActiveTexture->Surface, 0, WindowSurface, &DoorRect);

        for (DTi = 0; DTi < 10; DTi++)
        {
            SDL_BlitScaled(DT[DTi].ActiveTexture->Surface, 0, WindowSurface, &DTRect[DTi]);
        }

        SDL_BlitScaled(Ahole.LeftLeaning.Surface, 0, WindowSurface, &AholeRect);

        SDL_BlitScaled(Player.ActiveTexture->Surface, &PlayerActiveRectangle, WindowSurface, &PlayerRect);

        if (PlayerFart.ToFart)
        {
            if (Player.Direction == RightDirection)
            {
                SDL_BlitScaled(PlayerFart.ActiveTexture->Surface,
                               &PlayerFartActiveRect, WindowSurface, &PlayerFartRectR);
            }
            if (Player.Direction == LeftDirection)
            {
                SDL_BlitScaled(PlayerFart.ActiveTexture->Surface,
                               &PlayerFartActiveRect, WindowSurface, &PlayerFartRectL);
            }
        }
        for (FCi = 0; FCi < FClength; FCi++)
        {
            if (PlayerFartCloud[FCi].HasFarted)
            {
                SDL_BlitScaled(PlayerFartCloud[FCi].FartCloud.Surface, &PlayerFartCloudActiveRect[FCi],
                               WindowSurface, &PlayerFartCloudRect[FCi]);
            }
        }
        char FCcount[50];
        sprintf(FCcount, "Fart Cloud count: %i", FClength);

        RenderText(Regular, FCcount, 255, 255, 255, WindowWidth - 180, 25, TextSurface, WindowSurface, WindowWidth, WindowHight);
        //printf("read index = %i, write index = %i\n", FartCloudReadIndex, FartCloudWriteIndex);

        //FPS------------------------------------------------------
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
                //   printf("\rFPS:%f", (float)(1000.0f / (actualFrameEnd - frameStart)));
            }

            RenderText(Regular, NowFPS, 255, 255, 255, WindowWidth - 70, 0, TextSurface, WindowSurface, WindowWidth, WindowHight);
        } //------------------------------------------------------
        SDL_UpdateWindowSurface(Window);
    };
    //end of game loop

    return 0;
}
