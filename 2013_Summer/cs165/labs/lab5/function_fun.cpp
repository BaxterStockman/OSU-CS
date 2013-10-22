// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/15/2013 09:59:17 AM
//  Last Modification Date:     07/21/2013 06:51:50 PM
//  Filename:                   function_fun.cpp
//
//  Overview:
//      This program includes several functions, as follows:
//      (1) Takes an int as an argument and counts from zero to that int
//      (2) Takes an int as an argument and counts down from that int to zero
//      (3) Takes two ints as arguments and either (a) counts up from the first number to the second
//          number, if the first number is smaller, or (b) counts down from the first to the second,
//          if the first is larger.
//
//  Input:
//      User inputs two ints
//
//  Output:
//      Prints out an incremental series of numbers, e.g.:
//
//      Please enter a positive integer: 10
//      Please enter a positive integer: 4
//      0 1 2 3 4 5 6 7 8 9 10
//      10 9 8 7 6 5 4 3 2 1
//      10 9 8 7 6 5 4
//
//      Continue with new values (Y/n)?
//
// =====================================================================================

#include<iostream>
#include<string>    // allows use of string objects
#include<cstdlib>   // allows use of atoi()

using namespace std;

// Purpose: counts from 0 to the int argument, printing to the console
// Precondition: input is an int entered by the user
void CountUp(int);

// Purpose: counts down from the int argument to 0, printing to the console.
// Precondition: input is an int entered by the user
void CountDown(int);

// Purpose: counts between two integers entered by the user.  If the first int entered is largest,
// counts down from the first to the second.  If the first is smallest, counts up from the first to
// the second.
// Precondition: inputs are two ints entered by the user
void CountBetween(int, int);

// Purpose: checks whether the user's input is a positive int
// Precondition: input is a string containing user-inputted value
// Postcondition: returns true only if the string contains a positive integer,
// otherwise returns false
bool is_positive_int(string str);

// Purpose: solicits user to enter a positive integer.
// Precondition: none
// Postcondition: returns user input stored as string object
int get_positive_integer();

// Purpose: calls the above functions so that the program gets two
// user-entered integers, then prints to the console the output described
// above for CountUp(), CountDown(), and CountBetween().  It then asks the
// user whether to continue with new values, looping until a valid yes
// or no response is received.
// Precondition: none
// Postcondition: exits returning 0
int main()
{
    reinit:

    int firstInt = get_positive_integer();  // value to use for CountUp(), CountDown(), and CountBetween()
    int secondInt = get_positive_integer(); // second value to use in CountBetween()
    string cont;    // This stores the value of the user's answer of whether to continue
                    // (see for loop directly after reask:, infra)

    CountUp(firstInt);
    CountDown(firstInt);
    CountBetween(firstInt, secondInt);


    cout << "Continue with new values (Y/n)? ";
    //cin.ignore(10000, '\n');
    getline(cin, cont);
    //cout << cont << endl;

    reask:

    // Loop until a valid yes or no answer received; goto reask, supra, input invalid;
    // goto reinit, supra, if yes; exit if no.
    if(cont == "" || cont == "Y" || cont == "y" || cont == "Yes" || cont == "yes")
    {
        goto reinit;
    }
    else if(cont == "N" || cont == "n" || cont == "No" || cont == "no")
    {
        return 0;
    }
    else
    {
        cout << "I don't understand that. Continue (Y/n)? ";
        getline(cin, cont);
        goto reask;
    }
}

void CountUp(int upperBound)
{
    // Loop until upCount = upperBound, iterating upCount by +1
    // and printing current value of upCount to console each loop
    for(int upCount = 0; upCount <= upperBound; upCount++)
    {
        cout << upCount << " ";
    }
    cout << endl;
}

void CountDown(int upperBound)
{
    // Loop until downCount = lowerBound, iterating downCount by -1
    // and printing current value of downCount to console each loop
    for(int downCount = upperBound; downCount >= 0; downCount--)
    {
        cout << downCount << " ";
    }
    cout << endl;
}

void CountBetween(int firstBound, int secondBound)
{
    // if firstBound > secondBound, imitate CountDown() with lowerBound
    // set to secondBound
    if(firstBound > secondBound)
    {
        for(int downCount = firstBound; downCount >= secondBound; downCount--)
            cout << downCount << " ";
    }
    // if firstBound < secondBound, imitate CountUp() with lowerBound
    // set to secondBound
    else if(firstBound < secondBound)
    {
        for(int upCount = firstBound; upCount <= secondBound; upCount++)
            cout << upCount << " ";
    }
    // No counting to do here!
    else
    {
        cout << "The numbers are the same!";
    }
    cout << endl;
}

bool is_positive_int(string str)
{
    int stringSize = str.size(); // stores the size of the user-entered string
    //debugging --> cout << "The size of that string is " << stringSize << endl;

    // reject empty strings
    if(str == "")
        return false;
    // for non-empty strings, check that character at each position of the string
    // is greater than or equal to 0 and less than or equal to 9;
    // reject for any other values
    for(int ltrIndex = 0; ltrIndex < stringSize; ltrIndex++)
    {
        //debugging --> cout << "The character at position " << ltrIndex << " is " << str.at(ltrIndex) << endl;
        if(str.at(ltrIndex) < '0' || str.at(ltrIndex) > '9')
        {
            return false;
        }
    }
    return true;    // returns true only if string passes the above test
}

int get_positive_integer()
{
    string strInt;  // stores value of user input

    retry:

    // Prompts for user input, stores in strInt
    cout << "Please enter a positive integer: ";
    getline(cin, strInt);

    // Repeats user prompt and value storage for any
    // value of strInt that is not a positive integer
    if(!is_positive_int(strInt))
    {
        cout << "That input is invalid." << endl;
        goto retry;
    }

    // converts strInt to a C str for user with atoi();
    // uses atoi() to get an int value which is then
    // returned
    int intStr = atoi(strInt.c_str());
    return intStr;
}
