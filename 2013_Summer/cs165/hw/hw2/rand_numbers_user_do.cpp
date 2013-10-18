// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/12/2013 07:41:27 PM        
//  Last Modification Date:     07/14/2013 01:44:33 PM        
//  Filename:                   rand_numbers_user.cpp    
//
//  Overview:        
//      This program generates a random number < 50 and gives the user five tries
//      to guess it.  If the user fails after five tries, the program exits; the program
//      also exits if the user guesses correctly.
//       
//  Input:       
//      The user must enter a number less than 50 and greater than or equal to zero,
//      otherwise the program raises an exception. 
//         
//  Output:   
//      The program informs users of correct and incorrect guesses by printing messages
//      to the console; the program also informs users of guess that are out-of-bounds. 
//
// =====================================================================================

#include<iostream>  // included to allow for cout and cin
#include<ctime>     // included to allow time()
#include<cstdlib>   // included to allow rand(), srand() and atoi()
#include<string.h>  // included to allow strlen()

using std::cout;
using std::cin;
using std::endl;

/******************************************************************************************
 *  Entry:      Called by int main() to check the value of certain user input
 *
 *  Exit:       Exits when input confirm to be within specified parameters 
 *
 *  Purpose:    This function verifies that the user's input is a one- or two-digit number
 *              from 0 to 49.  It returns false for any other value, including when the 
 *              input contains alphas. 
 *
 *  Source: http://answers.yahoo.com/question/index?qid=20090130125311AAHSc74
 *  Answer provider: BobearQSI 
 *****************************************************************************************/
bool CheckInput(const char *str)
{
    if (strlen(str) == 0 || strlen(str) > 2)    // check if string is empty or longer than two characters
        return false;
    while (*str < '0' || *str > '9')
    {
        return false;                           // returns false when character not a digit
        str++;                                  // move to next character
    } 
    if (atoi(str) < 0 || atoi(str) > 49)        // convert to number and check whether in proper range
        return false;
    else
        return true;
}  

/******************************************************************************************
 * Entry:       None.
 *
 * Exit:        Exits, returning 0, after either (1) a correct guess, or (2) 5 incorrect
 *              guesses.
 *
 * Purpose:     Get the user to guess a random number from 0 to 49
 ******************************************************************************************/
int main() 
{
    int randChoice;                  // variable to hold random integer
    srand(time(NULL));      // seeds random number generator.  Do this just once.

    randChoice = rand() % 50;        // set x to the remainder of some random number divided by 50

    int g_count = 0;        // keeps track of the number of guesses
    char user_guess[80];    // holds the value of the user's guess to compare to x

    //cout << "My number is " << x << endl;     // <----- debugging

    cout << "Guess a number between 0 and 49: ";
    cin >> user_guess; 

    // This while loop continues so long as the user's in-bounds guesses (i.e. guesses with
    // a value from 0 to 49) do not exceed 5.  Upon a fifth incorrect guess, the loop exits.  The
    // loop also exits if the user guesses correctly.  The loop solicits new input if the user has
    // not entered a number from 0 to 49.
    while(g_count <= 4)
    {
        if(CheckInput(user_guess) == false) 
        {
            cout << "That input is invalid.  Guess again: ";
            cin >> user_guess;
        }
        else
        {
            int user_guess_num = atoi(user_guess);  // converts char to int for comparison to random number
            if(user_guess_num == randChoice)     // enters if guess is correct 
            {
                cout << "That's right!  You're either lucky, psychic, or have reverse-engineered the C++ random number generator." << endl;
                return 0;
            } 
            if(g_count < 4)     // only want to enter this if user has guessed under five times
            {
                if(user_guess_num > randChoice) 
                {
                    cout << user_guess << " is too high; guess again: ";
                    cin >> user_guess;
                    g_count ++;
                }
                else 
                {
                    cout << user_guess << " is too low; guess again: ";
                    cin >> user_guess;
                    g_count ++;
                }
            }
            else            // iterate g_count to exit loop upon fifth incorrect guess
            {               // could also have used 'break' or 'return 0'
                g_count++;
            }
        }
    }
    cout << "You lose!  No hard feelings, chump. :)" << endl;
    return 0;
}
