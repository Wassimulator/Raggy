#pragma once
#include "raggy.hpp"
#include "source.cpp"
#include "dialogues.cpp"
using namespace std;

void LoadAholeSound(NPCsounds *AholeS, bool *AholeSLoaded)
{
    cout << "Loading sound files... ";
    for (int i = 0; i < 100; i++)
    {
        char File[30];
        sprintf(File, "data/sounds/ahole/%i.WAV", i);
        AholeS->Node[i] = Mix_LoadWAV(File);
    }
    cout << "Successful" << endl;
    *AholeSLoaded = true;
}
void AholeDialogue(dialogues *Dialogue, player *Player, dialogueNPC *Ahole, NPCsounds *AholeS,
                   bool *firstrun, bool *refresh, bool *isTalking, string FileString)
{

    stringstream Input;
    token T;
    Input.str(FileString);

    if (Mix_Playing(2) == 0)
    {
        *isTalking = false;
    }
    if (*isTalking == false)
    {
        Dialogue->View = Ahole->IdleView;
    }

    if (*refresh)
    {

        int TargetNode;
        int count = 0;

        bool NoneSelected = true;
        for (int i = 0; i < 12; i++)
        {
            if (Dialogue->SelectedOption[i] == true)
            {
                NoneSelected = false;
            }
        }
        if (firstrun)
        {

            if (NoneSelected == true)
            {
                DP_FindAndParseHomeNode(&T, &Input, Dialogue, FileString);
                Mix_PlayChannel(2, AholeS->Node[0], 0); //use Mix_Haltchannel() to stop, also, Player uses channel 1, NPCs on channel 2.
                Dialogue->View = Ahole->TalkView;
                *isTalking = true;

                for (int i = 0; i < 12; i++)
                {
                    Dialogue->SelectedOption[i] = false;
                }
                Dialogue->PlayerText = " ";
                *firstrun = false;
            }

            if (NoneSelected == false)
            {
                for (int i = 0; i < 12; i++)
                {
                    if (Dialogue->SelectedOption[i] == true)
                    {

                        //use Mix_Haltchannel() to stop, also, Player uses channel 1, NPCs on channel 2.
                        Mix_PlayChannel(2, AholeS->Node[Dialogue->Option[i].NextNodeID], 0);

                        Dialogue->PlayerText = Dialogue->Option[i].Text;
                        if (!DP_ParseNextNode(i, Dialogue, &Input, &T, FileString))
                        {
                            cout << "ERROR: Node not found!" << endl;
                            break;
                        }

                        Dialogue->View = Ahole->TalkView;
                        *isTalking = true;
                    }
                }
            }
            refresh = false;
        }
    }
}
void AholeMain(bool *AholeSLoaded, NPCsounds *AholeS, dialogues *Dialogue, player *Player, dialogueNPC *Ahole,
               bool *firstrun, bool *refresh, bool *isTalking, string *FileString)
{
    if (*AholeSLoaded == false)
    {
        LoadAholeSound(AholeS, AholeSLoaded);
        *FileString = Lexer_FileToString("data/texts/ahole2.rxt");
    }
    AholeDialogue(Dialogue, Player, Ahole, AholeS, firstrun, refresh, isTalking, *FileString);
}

/*void AholeDialogue(dialogues *Dialogue, player *Player, dialogueNPC *Ahole, NPCsounds *AholeS, bool *firstrun, bool *refresh, bool *isTalking)
{

    if (Mix_Playing(2) == 0)
    {
        *isTalking = false;
    }
    if (*isTalking == false)
    {
        Dialogue->View = Ahole->IdleView;
    }

    if (*refresh)
    {
        string FileName = "data/texts/ahole.txt";
        ifstream Input;
        Dialogue->DialogueTitle = "Pricksoin Ahole";
        int TargetNode;
        int count = 0;

        bool NoneSelected = true;
        Input.open(FileName);
        for (int i = 0; i < 12; i++)
        {
            if (Dialogue->SelectedOption[i] == true)
            {
                NoneSelected = false;
            }
        }
        if (firstrun)
        {
            if (NoneSelected == true)
            {
                Mix_PlayChannel(2, AholeS->Node[2], 0); //use Mix_Haltchannel() to stop, also, Player uses channel 1, NPCs on channel 2.
                Dialogue->View = Ahole->TalkView;
                *isTalking = true;

                for (int i = 0; i < 12; i++)
                {
                    Dialogue->SelectedOption[i] = false;
                }
                Dialogue->PlayerText = " ";

                string buffer;
                getline(Input, buffer, '\n'); //skip first line
                getline(Input, buffer, ',');
                char *charbuffer = new char[buffer.length() + 1];
                strcpy(charbuffer, buffer.c_str());
                Dialogue->NPCtext = charbuffer;

                getline(Input, buffer, ',');
                Dialogue->MaxOptions = stoi(buffer);

                for (int i = 0; i < Dialogue->MaxOptions; i++)
                {
                    getline(Input, buffer, ',');
                    if (buffer.empty() == false)
                    {
                        char *charbuffer = new char[buffer.length() + 1];
                        strcpy(charbuffer, buffer.c_str());
                        Dialogue->Option[i].Text = charbuffer;
                    }

                    getline(Input, buffer, ',');
                    if (buffer.empty() == false)
                    {
                        Dialogue->Option[i].NextNodeID = stoi(buffer);
                    }

                    if (i == (Dialogue->MaxOptions - 1))
                    {
                        delete[] charbuffer;
                    }
                }

                buffer.clear();
                Input.clear();
                Input.seekg(0, ios_base::beg);
            }
            *firstrun = false;
        }

        if (NoneSelected == false)
        {
            for (int i = 0; i < 12; i++)
            {
                if (Dialogue->SelectedOption[i] == true)
                {
                    while (1)
                    {
                        TargetNode = Dialogue->Option[i].NextNodeID;
                        Dialogue->ID = TargetNode;
                        Dialogue->PlayerText = Dialogue->Option[i].Text;

                        string buffer;
                        if ((count + 1) == TargetNode)
                        {
                            getline(Input, buffer, ',');
                            char *charbuffer = new char[buffer.length() + 1];
                            strcpy(charbuffer, buffer.c_str());
                            Dialogue->NPCtext = charbuffer;

                            getline(Input, buffer, ',');
                            Dialogue->MaxOptions = stoi(buffer);

                            for (int i = 0; i < Dialogue->MaxOptions; i++)
                            {
                                getline(Input, buffer, ',');
                                if (buffer.empty() == false)
                                {
                                    char *charbuffer = new char[buffer.length() + 1];
                                    strcpy(charbuffer, buffer.c_str());
                                    Dialogue->Option[i].Text = charbuffer;
                                }

                                getline(Input, buffer, ',');
                                if (buffer.empty() == false)
                                {
                                    Dialogue->Option[i].NextNodeID = stoi(buffer);
                                }

                                if (i == (Dialogue->MaxOptions - 1))
                                {
                                    delete[] charbuffer;
                                }
                            }
                            break;
                            count = 0;
                        }
                        else
                        {
                            getline(Input, buffer, '\n');
                            count++;
                        }
                    }
                    //use Mix_Haltchannel() to stop, also, Player uses channel 1, NPCs on channel 2.
                    Mix_PlayChannel(2, AholeS->Node[TargetNode], 0);
                    Dialogue->View = Ahole->TalkView;
                    *isTalking = true;
                }
            }
        }
        refresh = false;
        Input.close();
    }
}*/