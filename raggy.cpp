#include "raggy.hpp"
#include "player.cpp"

int main(int argc, char **argv)
{

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *Window;

    Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);
    SDL_Surface *WindowSurface = SDL_GetWindowSurface(Window);

    int frameIndex = 0;

    bool RightButton = false;
    bool LeftButton = false;
    bool UpButton = false;
    bool DownButton = false;

    player Player = LoadPlayer();

    bool running = true;

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
                default:
                    break;
                }
            }
            
        }

        //GameUpdate------------------------------------------------------

        PlayerUpdate(&Player, RightButton, LeftButton, UpButton, DownButton);

        //----------------------------------------------------------------
        int R = 100;
        int G = 100;
        int B = 100;
        int A = 255;

        // WINDOWS USES BGRA (that does not stand for bulgaria)
        // Todo; map it so that it works on multiple systems

        SDL_FillRect(WindowSurface, 0, (A << 24) | (R << 16) | (G << 8) | (B));
        SDL_Rect Rect;
        Rect.x = Player.PosX;
        Rect.y = Player.PosY;
        Rect.w = 32 * 3;
        Rect.h = 32 * 3;

        SDL_Rect ActiveRectangle;
        //int p = (i-1) % 3;
        //int q = (i-1) / 3;

        int p;
        int q;

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

        ActiveRectangle.x = p * 32;
        ActiveRectangle.y = q * 32;
        ActiveRectangle.w = ActiveRectangle.h = 32;

        SDL_BlitScaled(Player.ActiveTexture->Surface, &ActiveRectangle, WindowSurface, &Rect);

        //SDL_BlitScaled(Player.ActiveTexture->Surface, 0, WindowSurface, &Rect);
        //SDL_BlitSurface(Player.Surface,  PlayerPos, WindowSurface,  PlayerPos);

        SDL_UpdateWindowSurface(Window);

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

    return 0;
}
