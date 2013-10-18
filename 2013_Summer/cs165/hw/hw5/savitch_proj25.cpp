// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 07:18:58 PM
//  Last Modification Date:
//  Filename:                   savitch_proj25.cpp
//
//  Overview:
//      This program solves the cryptarithmatic puzzle "TOO + TOO + TOO + TOO = GOOD",
//      where T, O, G, and D represent distinct numerals.
//
//  Input:
//      No input is required.
//
//  Output:
//      Prints various solutions to the console.
//
// =====================================================================================

#include<iostream>

using namespace std;

int main()
{
    // each of these nested loops iterates over the possible values of one of the letters
    // T, O, G, and D, ensuring that every possible combination of values of the four letters
    // gets tested to see if it satisfies the given equality
    for(int t = 0; t < 10; t++)
    {
        for(int o = 0; o < 10; o++)
        {
            for(int g = 0; g < 10; g++)
            {
                for(int d = 0; d < 10; d++)
                {
                    // checks to make sure that values are distinct, and that they satisfy the
                    // stipulated equality
                    if(t != o && t != g && t != d && o != g && o!= d && g != d && (400*t) == ((1000*g)+(66*o)+d))
                        cout << "Solution: " << "(T: " << t << ", O: " << o << ", G: " << g << ", D: " << d << ")" << endl;
                }
            }
        }
    }

    return 0;
}
