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

//these are for resource monitoring on Windows:-------------------------------------------------
#include "windows.h"
#include "psapi.h"
//--------------------

//-----------------array indeces---------------------------

int DTi = 0;
int FCi = 0;
int FCiw = 0;
int FClength = 0;

bool GameIsRunning = true;
bool RAMleak = false;
bool MainMenuRunning = true;

