/*   
    This lexer tokenizes text in similar ways to C compilers. 
    
    -Requirements: C++ standard library. or whatever is standard these days.

    -Limitations:
    comments within quotes makes the lexer shit itself
    don't use // or /* or * / within quotes. otherwise you're good to go.

    -------example to use:----------
   string EndString;

    string TempString = Lexer_ReadFileIntoMemory("file.txt");
    stringstream Input;
    token Token;

    Lexer_RemoveComments(&TempString);
    Lexer_RemoveWhitespace(&TempString);

    Input.str(TempString);

    while (Input.eof() == false)
    {
        Token = Lexer_GetToken(&Input);
    }
*/

#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string Lexer_ReadFileIntoMemory(char *File)
{
    string Result;

    cout << "Loading RXT file... ";
    string Buffer;
    string FileName = File;
    ifstream Input;
    Input.open(FileName);
    getline(Input, Buffer, '%');
    Result = Buffer;
    
    switch (Input.good())
    {
    case true:
        cout << "Successful" << endl;
        break;
    case false:
        cout << "Failed!" << endl;
    }

    return (Result);
}

enum type
{
    Asterix,
    QuestionMark,
    ExclamationMark,
    Plus,
    Minus,
    Equals,
    Hash,
    Comma,
    SemiColon,
    Colon,
    OpenParenthese,
    ClosedParenthese,
    OpenBrace,
    ClosedBrace,
    OpenSquareBrace,
    ClosedSquareBrace,
    ForwardSlash,
    BackSlash,
    SmallerThan,
    LargerThan,
    Identifier,
    Number,
    Text
};

struct token
{
    string Text;
    string At;
    type Type;
};

void Lexer_RemoveWhitespace(string *String)
{
    String->erase(remove(String->begin(), String->end(), '\n'), String->end());
}

void Lexer_RemoveComments(string *String)
{
    while (String->find("/*") != -1)
    {
        int n = String->find("/*");
        for (n; n < String->size();)
        {
            if (String->at(n) == '*' && String->at(n + 1) == '/')
            {
                String->erase(String->begin() + n);
                String->erase(String->begin() + n);
                break;
            }
            else
            {
                String->erase(String->begin() + n);
            }
        }
    }
    while (String->find("//") != -1)
    {
        int n = String->find("//");
        for (n; n < String->size();)
        {
            if (String->at(n) == '\n')
            {
                String->erase(String->begin() + n);
                break;
            }
            else
            {
                String->erase(String->begin() + n);
            }
        }
    }
}

void Lexer_GetIdentifierToken(stringstream *Input, token *ResultToken)
{
    char c;
    bool GettingToken = true;
    while (GettingToken)
    {
        if (Input->eof())
        {
            break;
        }
        if (isalpha(Input->peek()) == false)
        {
            break;
        }
        Input->get(c);
        ResultToken->Text += c;
    }
    ResultToken->Type = Identifier;
}

void Lexer_GetNumberToken(stringstream *Input, token *ResultToken)
{
    char c;
    bool GettingToken = true;
    while (GettingToken)
    {
        if (Input->eof())
        {
            break;
        }
        if (isdigit(Input->peek()) == false)
        {
            break;
        }
        Input->get(c);
        ResultToken->Text += c;
    }
    ResultToken->Type = Number;
}

void Lexer_GetQuotedTextToken(stringstream *Input, token *ResultToken)
{
    //gets the contents of a quoted text without the quotes
    char c;
    bool GettingToken = true;
    Input->get(); //skip open quote
    while (GettingToken)
    {
        if (Input->eof())
        {
            break;
        }
        if (Input->peek() == '"')
        {
            break;
        }
        Input->get(c);
        ResultToken->Text += c;
    }
    Input->get(); //skip closed quote
    ResultToken->Type = Text;
}

token Lexer_GetToken(stringstream *Input)
{
    token ResultToken;

    char c;
    bool isChar = false;

    bool GettingToken = true;
    while (GettingToken)
    {
        if (isalpha(Input->peek()))
        {
            Lexer_GetIdentifierToken(Input, &ResultToken);
            GettingToken = false;
            break;
        }
        if (isdigit(Input->peek()))
        {
            Lexer_GetNumberToken(Input, &ResultToken);
            GettingToken = false;
            break;
        }
        if (Input->peek() == '"')
        {
            Lexer_GetQuotedTextToken(Input, &ResultToken);
            GettingToken = false;
            break;
        }
        if (Input->peek() == '"')
        {
            Lexer_GetQuotedTextToken(Input, &ResultToken);
            GettingToken = false;
            break;
        }

        if (Input->eof())
        {
            break;
        }

        Input->get(c);

        switch (c)
        {
        case '(':
            ResultToken.Type = OpenParenthese;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case ')':
            ResultToken.Type = ClosedParenthese;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '{':
            ResultToken.Type = OpenBrace;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '}':
            ResultToken.Type = ClosedBrace;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '[':
            ResultToken.Type = OpenSquareBrace;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case ']':
            ResultToken.Type = ClosedSquareBrace;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '/':
            ResultToken.Type = ForwardSlash;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '\\':
            ResultToken.Type = BackSlash;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '<':
            ResultToken.Type = SmallerThan;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '>':
            ResultToken.Type = LargerThan;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case ':':
            ResultToken.Type = Colon;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case ';':
            ResultToken.Type = SemiColon;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case ',':
            ResultToken.Type = Comma;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '+':
            ResultToken.Type = Plus;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '-':
            ResultToken.Type = Minus;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '=':
            ResultToken.Type = Equals;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '#':
            ResultToken.Type = Hash;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '?':
            ResultToken.Type = QuestionMark;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '!':
            ResultToken.Type = ExclamationMark;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        case '*':
            ResultToken.Type = Asterix;
            GettingToken = false;
            ResultToken.Text = c;
            break;
        default:
            break;
        }
    }
    return ResultToken;
}

string Lexer_FileToString(char *file)
{
    string FileString = Lexer_ReadFileIntoMemory(file);
    Lexer_RemoveComments(&FileString);
    Lexer_RemoveWhitespace(&FileString);
    return FileString;
}