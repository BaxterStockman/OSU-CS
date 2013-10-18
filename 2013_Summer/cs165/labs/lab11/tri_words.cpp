// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/23/2013 04:21:39 PM
//  Last Modification Date:
//  Filename:                   tri_words.cpp
//
//  Overview:
//      This program reads from file containing English words and determines whether
//      they are triangle words.
//
//  Input:
//      The program reads from the file specified in #define FILE_NAME; it also prompts
//      the user to select whether to print the triangle words to the console.
//
//  Output:
//      The program outputs to the console the number of triangle words, and at the
//      user's choice prints the triangle words as well.
//
// =====================================================================================

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>     // for pow()

#define FILE_NAME "words.txt"

// Purpose: removes opening and closing quotation marks from a string
// Entry:   str is any string object
// Exit:    str no longer has quotation marks at the beginning and end
void strip_quotes(std::string &str);

// Purpose: determines the numerical value of the word based on the
//          assigned value of its consituent letters
int get_word_val(const std::string &str);

// Purpose: determines whether the inputted number is a triangle number
const bool is_tri_num(const int &num);

int main()
{
    int choice;

    std::string input;

    std::vector<std::string> triWords;

    std::ifstream words(FILE_NAME);

    if(!(words.is_open()))
    {
        std::cout << "Could not open file!" << std::endl;
        return 0;
    }

    while(words >> input)
    {
        strip_quotes(input);
        if(is_tri_num(get_word_val(input)))
            triWords.push_back(input);
    }

    std::cout << "There are " << triWords.size() << " triangle words in " << FILE_NAME << std::endl;
    std::cout << "Would you like me to list them? (0 = no, 1 = yes):  ";
    std::cin >> choice;

    switch(choice)
    {
        case 0:
            return 0;
        case 1:
            for(int index = 0; index < triWords.size(); index++)
                std::cout << triWords.at(index) << std::endl;
        default:
            return 0;
    }

    words.close();

    return 0;
}

void strip_quotes(std::string &str)
{
    int firstQt = str.find("\"");
    int lastQt = str.rfind("\"");

    str = str.substr(firstQt + 1, lastQt - 1);
}

int get_word_val(const std::string &str)
{
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int wordVal;

    for(int outer = 0; outer < str.length(); outer++)
    {
        for(int inner = 0; inner < alphabet.length(); inner++)
        {
            if(str.at(outer) == alphabet.at(inner))
                wordVal += inner + 1;
        }
    }

    return wordVal;
}

const bool is_tri_num(const int &num)
{
    int base = 1;
    int triNum = (std::pow(base, 2) + base) / 2;

    while(triNum <= num)
    {
        if(num == triNum)
            return true;
        base++;
        triNum = (std::pow(base, 2) + base) / 2;
    }

    return false;
}

