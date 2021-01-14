#pragma once


struct dialogues
{
    bool Ahole = false;
    char *OptionText[12];
    int MaxOptions = 12;
    string DialogueTitle = "dialogue title";
    string NPCtext = "placeholder for NPC text (what they say)";
    char *PlayerText = "placeholder for Player text (what he says)";
    bool HighlightedOption[12];
    bool SelectedOption[12];
    int ID;

    struct options
    {
        string Text;
        dialogues *NextNode;
        int NextNodeID;
    };

    options Option[12];
};

void DP_Node(stringstream *Input, dialogues *D)
{
    token T;
    T = Lexer_GetToken(Input);
    if (T.Type == OpenBrace)
    {
        int i;
        while (true)
        {
            T = Lexer_GetToken(Input);
            if (T.Type == Identifier)
            {
                if (T.Text == "NPCprompt")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Text)
                        {
                            D->NPCtext = T.Text;
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
                if (T.Text == "Title")
                {
                    while (true)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == Text)
                        {
                            D->DialogueTitle = T.Text;
                        }
                        if (T.Type == SemiColon)
                        {
                            break;
                        }
                    }
                }
                if (T.Text == "Option")
                {
                    bool WithinOption = true;
                    while (WithinOption)
                    {
                        T = Lexer_GetToken(Input);
                        if (T.Type == OpenSquareBrace)
                        {
                            T = Lexer_GetToken(Input);
                            if (T.Type == Number)
                            {
                                i = stoi(T.Text);
                            }
                        }
                        if (T.Type == Text)
                        {
                            D->Option[i].Text = T.Text;
                        }
                        if (T.Type == SmallerThan)
                        {
                            while (true)
                            {
                                /////////////////////////copy this and paste afterwards for more modifiers
                                T = Lexer_GetToken(Input);
                                if (T.Type == Identifier)
                                {
                                    if (T.Text == "nextNode")
                                    {
                                        while (true)
                                        {
                                            T = Lexer_GetToken(Input);
                                            if (T.Type == Number)
                                            {
                                                D->Option[i].NextNodeID = stoi(T.Text);
                                            }
                                            if (T.Type == SemiColon)
                                            {
                                                break;
                                            }
                                        }
                                    }
                                    /////////////////////////////
                                }
                                if (T.Type == LargerThan)
                                {
                                    WithinOption = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if (T.Type == ClosedBrace)
            {
                D->MaxOptions = i + 1;
                break;
            }
        }
    }
}

void DP_ReloadStream(stringstream *Input, string FileString)
{
    stringstream tempstream;
    Input->swap(tempstream);
    Input->str(FileString);
}

int DP_ParseNextNode(int choice, dialogues *D, stringstream *Input, token *T, string FileString)
{
    string NextNode = to_string(D->Option[choice - 1].NextNodeID);

    int NodeFound = false;
    DP_ReloadStream(Input, FileString);
    while (true)
    {
        *T = Lexer_GetToken(Input);
        if (T->Type == Identifier)
        {
            if (T->Text == "Node")
            {
                *T = Lexer_GetToken(Input);
                if (T->Type == OpenSquareBrace)
                {
                    *T = Lexer_GetToken(Input);
                    if (T->Type == Number && T->Text == NextNode)
                    {
                        *T = Lexer_GetToken(Input);
                        DP_Node(Input, D);
                        NodeFound = true;
                        break;
                    }
                }
            }
            if (T->Text == "EOF")
            {
                break;
            }
        }
    }
    if (NodeFound)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


