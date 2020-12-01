#include "raggy.hpp"
#include "source.cpp"

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

    player Player = LoadPlayer();

    Player.PosX = 0;
    Player.PosY = 0;

    bool running = true;

    map Map = LoadMap();
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

    //Game Loop-----------------------------------------------------------------
    while (running)
    {

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
                case SDLK_f:
                    F_Key = KeyState;
                    break;
                case SDLK_h:
                    H_Key = KeyState;
                    break;
                default:
                    break;
                }
            }
        }

        //GameUpdate----------------------------------------------------------------------------------

        float MapLimitR = Map.ActiveMap.w * 0.5f - 3 * 16;
        float MapLimitL = -Map.ActiveMap.w * 0.5f + 3 * 16;

        PlayerUpdate(&Player, CamPosX, RightButton, LeftButton, UpButton, DownButton, Shift, MapLimitL, MapLimitR, WalkSpeed, RunSpeed);
        PlayerSoundUpdate(Sound, F_Key, H_Key);
        MapUpdate(&CamPosX, &Player);

        //printf("CamPosX = %.0f  ", CamPosX);
        //printf("P-PosX = %.0f  ", Player.PosX);
        //printf("M-PosX = %.0f  \n", Map.PosX);
        //printf("MapLimitL = %.0f  ", MapLimitL);
        //printf("MapLimitR = %0.f\n", MapLimitR);

        //----------------------------Pre-Rendering---------------------------------------------------
        int R = 50;
        int G = 50;
        int B = 50;
        int A = 255;

        // WINDOWS USES BGRA (that does not stand for bulgaria)
        // TODO: map it so that it works on multiple systems

        // TODO: Move the SDL_Rect clutter into somehting organized
        SDL_FillRect(WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));
        SDL_Rect Rect;

        //putting the player in the center of the screen later :
        Rect.x = (WindowWidth / 2) + (Player.PosX - CamPosX) - (3 * 16);
        Rect.y = (WindowHight / 2) - 48;

        // TODO: Scaling is an inherent problem that needs fixing.
        Rect.w = 32 * 3;
        Rect.h = 32 * 3;

        SDL_Rect ActiveRectangle;
        //int p = (i-1) % 3;
        //int q = (i-1) / 3;

        int p;
        int q;

        //animation sequence----------
        switch (Player.i)
        {
        case 1:
            p = 0;
            q = 0;
            break;
        case 2:
            p = 1;
            q = 0;
            break;
        case 3:
            p = 2;
            q = 0;
            break;
        case 4:
            p = 0;
            q = 1;
            break;
        case 5:
            p = 1;
            q = 1;
            break;
        case 6:
            p = 2;
            q = 1;
        default:
            break;
        };

        //-----------------------------Rendering-------------------------------------------------

        ActiveRectangle.x = p * 32;
        ActiveRectangle.y = q * 32;
        ActiveRectangle.w = ActiveRectangle.h = 32;

        //TODO: move the SDL_Rect clutter in a struct or something.

        SDL_Rect MapRect;
        MapRect.h = Map.ActiveMap.h;
        MapRect.w = Map.ActiveMap.w;
        MapRect.x = ((WindowWidth - Map.ActiveMap.w) / 2) - CamPosX;
        MapRect.y = Map.PosY;

        SDL_BlitScaled(Map.ActiveMap.Surface, 0, WindowSurface, &MapRect);

        SDL_BlitScaled(Player.ActiveTexture->Surface, &ActiveRectangle, WindowSurface, &Rect);

        RenderText(Regular, "Test, test, testicles.", 255, 255, 255, WindowSurface, WindowWidth, WindowHight);

        /*font Message1 = LoadFont("this is a fixed Text in white", 20, 255, 255, 255);
        SDL_Rect TextRect1;
        TextRect1.h = Message1.TextSurface->h;
        TextRect1.w = Message1.TextSurface->w;
        TextRect1.x = ((WindowWidth - Message1.TextSurface->w) / 2);
        TextRect1.y = WindowHight/2 +150;
        SDL_BlitSurface(Message1.TextSurface, 0, WindowSurface, &TextRect1);

        font Message2 = LoadFont("this is a moving Text in blue", 20, 100, 100, 255);
        SDL_Rect TextRect2;
        TextRect2.h = Message2.TextSurface->h;
        TextRect2.w = Message2.TextSurface->w;
        TextRect2.x = ((WindowWidth - Message2.TextSurface->w) / 2) - CamPosX;
        TextRect2.y = WindowHight/2 -160;
        SDL_BlitSurface(Message2.TextSurface, 0, WindowSurface, &TextRect2);*/

        SDL_UpdateWindowSurface(Window);

        printf("CamPos = %.0f\n", CamPosX);

        //FPS------------------------------------------------------
        {
            int frameEnd = SDL_GetTicks();
            int frameTime = frameEnd - frameStart;
            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
            }
            int actualFrameEnd = SDL_GetTicks();
            if (frameIndex++ % 10 == 0)
            {
                //   printf("\rFPS:%f", (float)(1000.0f / (actualFrameEnd - frameStart)));
            }
        } //------------------------------------------------------
    };
    //end of game loop

    return 0;
}
