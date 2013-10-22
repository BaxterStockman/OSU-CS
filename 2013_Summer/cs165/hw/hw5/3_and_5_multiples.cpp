// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/08/2013 08:58:12 AM
//  Last Modification Date:
//  Filename:                   3_and_5_multiples.cpp
//
//  Overview:
//      This program prints to the console the multiples of three from zero to a
//      specified value.
//
//  Input:
//      No input is required; however, the number of multiples of three printed by
//      the program can be changed by altering the defined UPPER_BOUND, below.
//
//  Output:
//      Prints the multiples of three and five beneath UPPER_BOUND, defined below,
//      to the console.
//
// =====================================================================================

#include<iostream>
#include<vector>    // for vector data type
#include<cstdlib>   // for int abs()
#include<iomanip>   // for setw() and left

#define UPPER_BOUND 1000    // number at which to stop calculations

using namespace std;

// This function comes from:
// http://www.geeksforgeeks.org/write-an-efficient-method-to-check-if-a-number-is-multiple-of-3/
// Purpose: check whether a number is a multiple of three
// Entry:   int n is any int
// Exit:    returns true if n is a multiple of three, false if not
bool mult_of_three(int n);

// Purpose: check whether a number is a multiple of five
// Entry:   int n is any int
// Exit:    returns true if n is a multiple of five, false if not
bool mult_of_five(int n);

int main()
{
    int wordWidth = 5;              // width to allocate to each element printed to console
    int upperBound = UPPER_BOUND;   // number at which to stop calculation
    vector<int> mults_of_three;     // vector to store multiples of three
    vector<int> mults_of_five;      // vector to store multiples of five

    for(int i = 0; i <= upperBound; i++)
    {
        if(mult_of_three(i))                // if i is a multiple of three, add to vector
            mults_of_three.push_back(i);    // if i is a multiple of five, add to vector
        if(mult_of_five(i))
            mults_of_five.push_back(i);
    }

    cout << endl<< "Multiples of three: " << endl;
    for(int i = 1; i <= mults_of_three.size(); i++)
    {
        cout << setw(wordWidth) << left << mults_of_three.at(i-1);
        if((i%20) == 0)     // display 20 elements per line
            cout << endl;
    }

    cout << endl;

    cout << endl << "Multiples of five: " << endl;
    for(int i = 1; i <= mults_of_five.size(); i++)
    {
        cout << setw(wordWidth) << left << mults_of_five.at(i-1);
        if((i%20) == 0)     // display 20 elements per line
            cout << endl;
    }

    cout << endl << endl;

    return 0;
}

// If the difference between the number of odd bits set and even bits set equals
// a multiple of three, then the number itself is a multiple of three.  The following
// function checks whether that is the case.
bool mult_of_three(int n)
{
    int odd_count = 0;  //number of bits set at odd positions in binary representation of n
    int even_count = 0; //number of bits set at even positions in binary representation of n

    if(n < 0)       // prevents overflow
        n = -n;
    if(n == 0)      // 0 is divisible by three
        return 1;
    if(n == 1)      // 1 is not divisible by three
        return 0;

    while(n)    // loop so long as n != 0
    {
        if(n & 1)           // if rightmost bit is set, increment odd_count
            odd_count++;
        n = n >> 1;         // shift n to right by one bit

        if(n & 1)           // if rightmost bit is set, increment even count (this works because
            even_count++;   // rightmost bit was even position in original, non-shifted n)
        n = n >> 1;         // shift n to right by one bit
    }

    // once all set bits have been checked, run function recursively to check whether difference
    // between odd_count and even_count is a multiple of three.  Recursion stops when when
    // difference is either 1 or 0.
    return mult_of_three(abs(odd_count - even_count));
}

bool mult_of_five(int n)
{
    if(((n*2) % 10) == 0)   // multiples of five are multiples of 10 divided by 2
        return true;        // I don't know if this method really saves any time
    else                    // or resources over using %5
        return false;
}
