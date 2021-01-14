#pragma once
#include <stdio.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
#include <iostream>

#include <lexer.h>

//these are for resource monitoring on Windows and should be commented out elsewherer:
#include "windows.h"
#include "psapi.h"
//--------------------


//-----------------array indeces---------------------------

int Di = 0;
int FCi = 0;
int FCiw = 0;
int FClength = 0;

//-----------------array max values------------------------

const int MaxNPCs = 1;
const int MaxMaps = 1;
const int MaxDoors = 10;

//---------------------------------------------------------

bool GameIsRunning = true;
bool RAMleak = false;
bool MainMenuRunning = true;

bool FadeOut = true;
int FadeOuti;
bool FadeIn = false;
int FadeIni;
bool FadedIn = false;

int CurrentMap = 0; // TODO: deal with this.
bool ToUpdateMap = false;
bool ToUpdateMapRects = false;


void ResetFades()
{
    FadeOut = true;
    FadeOuti = 255;
    FadeIn = false;
    FadeIni = 0;
    FadedIn = false;
}
