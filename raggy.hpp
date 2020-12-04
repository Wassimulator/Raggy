#pragma once   
#include <stdio.h>
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "SDL_audio.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int FartCloudWriteIndex = 1;
int FartCloudReadIndex = 1;
int const MaxFartClouds = 20;
int FartCloudBufferLength;