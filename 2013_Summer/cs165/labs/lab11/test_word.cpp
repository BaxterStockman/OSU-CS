// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/25/2013 03:30:57 PM
//  Last Modification Date:
//  Filename:                   test_word.cpp
//
//  Overview:
//
//
//  Input:
//      <+text+>
//
//  Output:
//      <+text+>
//
// =====================================================================================

#include<iostream>
#include<string>

int main()
{
    std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i = 0; i < alpha.length(); i++)
    {
        std::cout << alpha.at(i) << " = " << alpha.at(i) - 64 << std::endl;
    }

    for(int i = 0; i <= 128; i++)
    {
        std::cout << "char number " << i << " is " << static_cast<char>(i) << std::endl;
    }
    return 0;
}
