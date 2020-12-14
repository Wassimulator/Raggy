#pragma once
#include "raggy.hpp"
#include "source.cpp"
#include "dialogues.cpp"

void AholeDialogue(dialogues *Dialogue, player *Player)
{
    Dialogue->DialogueTitle = "Pricksoin Ahole";
    Dialogue->View = LoadSprite("data/textures/ahole_dialogue_idle_anim.png");

    if (Player->AholeLevel == 0)
    {
        Dialogue->NPCtext = "Hey there champ! Amazing day isn't it? and what a good "
                            "day it is to be me!, how're you doing? I'm sure fine. Why wouldn't you be?";
        Dialogue->PlayerText = " ";
        Dialogue->MaxOptions = 5;
        Dialogue->OptionText[0] = "Fuck off Prickard.";
        Dialogue->OptionText[1] = "And a good day to you too!";
        Dialogue->OptionText[2] = "Would you take a breath and let me answer your fucking question?";
        Dialogue->OptionText[3] = "I'm fine, why wouldn't I be indeed! I just got fired from my fucking job";
        Dialogue->OptionText[4] = "(Say nothing)";
    }

    if (Dialogue->SelectedOption[0] == true)
    {
        Dialogue->PlayerText = Dialogue->OptionText[0];
        Dialogue->NPCtext = "Haha that's the spirit of the fighter in me that I love to see in others!";
        Dialogue->MaxOptions = 2;
        Dialogue->OptionText[0] = "You're the reason I'm leaving. (turn away and leave)";
        Dialogue->OptionText[0] = "it is fascinating how you managed to turn that into a compliment of yourself.";
        Player->AholeLevel = 1;
        for (int i = 0; i < 12; i++)
        {
            Dialogue->HighlightedOption[i] = false;
        }
        Dialogue->HighlightedOption[0] = true;
    }

    if (Dialogue->SelectedOption[1] == true) //-----------------
    {

        Dialogue->PlayerText = Dialogue->OptionText[1];
        Dialogue->NPCtext = "How are things?";
        Dialogue->MaxOptions = 3;
        Dialogue->OptionText[0] = "They suck ass I'm leaving the job here.";
        Dialogue->OptionText[1] = "Never been better!";
        Dialogue->OptionText[2] = "Does it matter?";
        Player->AholeLevel = 2;
        for (int i = 0; i < 12; i++)
        {
            Dialogue->HighlightedOption[i] = false;
        }
        Dialogue->HighlightedOption[0] = true;
    }

    if (Dialogue->SelectedOption[2] == true)
    {
        Dialogue->PlayerText = Dialogue->OptionText[2];
        Dialogue->NPCtext = "Woow, someone's crancky today! is everythinh alright? of course they are! why wouldn't they be?";
        Dialogue->MaxOptions = 2;
        Dialogue->OptionText[0] = "I will punch you in the throat if you don't stop answering your own questions.";
        Dialogue->OptionText[1] = "you're useless. (turn away and leave)";
        Player->AholeLevel = 3;
        for (int i = 0; i < 12; i++)
        {
            Dialogue->HighlightedOption[i] = false;
        }
        Dialogue->HighlightedOption[0] = true;
    }
    
    if (Dialogue->SelectedOption[3] == true)
    {
        Dialogue->PlayerText = Dialogue->OptionText[2];
        Dialogue->NPCtext = "Oh bummer, good thing I still have a job haha!";
        Dialogue->MaxOptions = 2;
        Dialogue->OptionText[0] = "You're a major douche.";
        Dialogue->OptionText[1] = "Talk more and you won't have kneecaps.";
        Player->AholeLevel = 4;
        for (int i = 0; i < 12; i++)
        {
            Dialogue->HighlightedOption[i] = false;
        }
        Dialogue->HighlightedOption[0] = true;
    }
    if (Dialogue->SelectedOption[4] == true)
    {
        Dialogue->PlayerText = Dialogue->OptionText[2];
        Dialogue->NPCtext = "I am so amazing! aren't I?";
        Dialogue->MaxOptions = 5;
        Dialogue->OptionText[0] = "No you are not.";
        Dialogue->OptionText[1] = "Suck a dick Ahole.";
        Dialogue->OptionText[2] = "Suck MY dick Ahole. (Romance)";
        Dialogue->OptionText[3] = "Suck a bag of dicks Ahole. (Kinky)";
        Dialogue->OptionText[4] = "Suck a vagina Ahole. (LGBTQ)";
        Player->AholeLevel = 5;
        for (int i = 0; i < 12; i++)
        {
            Dialogue->HighlightedOption[i] = false;
        }
        Dialogue->HighlightedOption[0] = true;
    }
}