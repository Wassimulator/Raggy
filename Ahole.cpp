#pragma once
#include "raggy.hpp"
#include "source.cpp"
#include "dialogues.cpp"
using namespace std;

void AholeDialogue(dialogues *Dialogue, player *Player, dialogueNPC *Ahole, bool *firstrun)
{
    string FileName = "data/texts/ahole.txt";
    ifstream Input;

    if (firstrun)
    {
        bool NoneSelected = true;
        for (int i = 0; i < 12; i++)
        {
            if (Dialogue->SelectedOption[i] == true)
            {
            }
            else
            {
                NoneSelected = true;
            }
        }
        if (NoneSelected = true)
        {
            Input.open(FileName);

            string buffer;
            getline(Input, buffer, '\n'); //skip first line
            getline(Input, buffer, ',');
            char *charbuffer = new char[buffer.length() + 1];
            strcpy(charbuffer, buffer.c_str());
            Dialogue->NPCtext = charbuffer;
            //delete[] charbuffer;

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
                    //delete[] charbuffer;
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
            firstrun = false;
            Input.close();
        }
    }

    //cout << Dialogue->Option[0].Text << " firstrun = " << firstrun << endl;

    //

    /*Dialogue->DialogueTitle = "Pricksoin Ahole";
    Dialogue->View = Ahole->IdleView;

    FILE *TextFile;
    char *Token;
    char Buffer[1024];
    int TargetNode;
    int count = 0;
    int ID;
    TextFile = fopen("data/texts/ahole.txt", "r");

    //--------------------assign all NextNodes-------------------------------
    fgets(Buffer, sizeof(Buffer), TextFile); //skip first line
    while (fgets(Buffer, sizeof(Buffer), TextFile))
    {
        Token = strtok(Buffer, ",");
        Token = strtok(NULL, ",");
        for (int i = 0; i < 12; i++)
        {
            Token = strtok(NULL, ",");
            Token = strtok(NULL, ",");
            if (Token != NULL)
            {
                ID = atoi(Token);
                Dialogue->Option[i].NextNode = &Node[ID];
                Dialogue->Option[i].NextNodeID = atoi(Token);
                Dialogue->Option[i].NextNodeID = Node[ID].ID;
            }
        }
    }
    rewind(TextFile);

    //----------------------Read Base Node----------------------------
    bool NoneSelected = true;
    for (int i = 0; i < 12; i++)
    {
        if (Dialogue->SelectedOption[i] == true)
        {
        }
        else
        {
            NoneSelected = true;
        }
    }

    if (NoneSelected = true)
    {
        for (int i = 0; i < 12; i++)
        {
            Dialogue->SelectedOption[i] = false;
        }
        Dialogue->PlayerText = " ";
        fgets(Buffer, sizeof(Buffer), TextFile);
        fgets(Buffer, sizeof(Buffer), TextFile);

        Token = strtok(Buffer, ","); //starts with the line we got
        Dialogue->NPCtext = Token;
        Token = strtok(NULL, ","); //NULL means it starts where the last one stopped
        Dialogue->MaxOptions = atoi(Token);
        for (int i = 0; i < 12; i++)
        {
            Token = strtok(NULL, ",");
            Dialogue->Option[i].Text = Token;
            Token = strtok(NULL, ",");
            if (Token != NULL)
            {
                ID = atoi(Token);
                Dialogue->Option[i].NextNode = &Node[ID];
                Dialogue->Option[i].NextNodeID = atoi(Token);
                Node[ID].ID = Dialogue->Option[i].NextNodeID;
            }
        }

        rewind(TextFile);
    }

    //------------------------------------------------------------------

    for (int i = 0; i < 12; i++)
    {
        if (Dialogue->SelectedOption[i] == true)
        {
            TargetNode = Dialogue->Option[i].NextNodeID;
            Dialogue->PlayerText = Dialogue->Option[i].Text;

            while (fgets(Buffer, sizeof(Buffer), TextFile) != NULL)
            {
                if (count == TargetNode)
                {
                    Token = strtok(Buffer, ","); //starts with the line we got
                    Dialogue->NPCtext = Token;
                    Token = strtok(NULL, ","); //NULL means it starts where the last one stopped
                    Dialogue->MaxOptions = atoi(Token);
                    for (int i = 0; i < Dialogue->MaxOptions; i++)
                    {
                        Token = strtok(NULL, ",");
                        Dialogue->Option[i].Text = Token;
                        Token = strtok(NULL, ",");
                        ID = atoi(Token);
                        Dialogue->Option[i].NextNode = &Node[ID];
                    }
                }
                else
                {
                    count++;
                }
            }
            rewind(TextFile);
            count = 0;
        }
    }

    fclose(TextFile);*/
}