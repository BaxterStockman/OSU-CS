// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/12/2013 07:41:27 PM        
//  Last Modification Date:     07/14/2013 01:38:00 PM
//  Filename:                   rand_numbers.cpp    
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
//      to the console; the program also informs users of guesses that are out-of-bounds. 
//
// =====================================================================================

#include<iostream>  // included to allow for cout and cin
#include<ctime>     // included to allow time()
#include<cstdlib>   // included to allow rand(), srand() and atoi()
#include<string.h>  // included to allow strlen()

using std::cout;
using std::cin;
using std::endl;

/***************************************************************************************
 *  Entry:      Called by int main() to check the value of certain user input
 *
 *  Exit:       Exits when input confirmed to be within specified parameters 
 *
 *  Purpose:    This function verifies that the user's input is a one- or two-digit number
 *              from 0 to 49.  It returns false for any other value, including when the 
 *              input contains alphas. 
 *
 *  Source: http://answers.yahoo.com/question/index?qid=20090130125311AAHSc74
 *  Answer provider: BobearQSI 
 **************************************************************************************/
bool checkInput(const char *str)
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

/***************************************************************************************
 * Entry:       None.
 *
 * Exit:        Exits, returning 0, after either (1) a correct guess, or (2), 5 incorrect
 *              guesses.
 *
 * Purpose:    The computer gives the user five chances to guess a number from 0 to 49 
 ***************************************************************************************/
int main() 
{
    int x;                  // variable to hold out random integer
    srand(time(NULL));      // seeds random number generator.  Do this just once.

    x = rand() % 50;        // set x to the remainder of some random number divided by 50

    int g_count = 0;        // keeps track of the number of guesses
    char user_guess[80];    // holds the value of the user's guess to compare to x

    cout << "Guess a number between 0 and 49: ";
    cin >> user_guess; 

    // This while loop continues so long as the user's in-bounds guesses (i.e. guesses with
    // a value from 0 to 49) do not exceed 5.  Upon a fifth incorrect guess, the loop exits.  The
    // loop also exits if the user guesses correctly.  The loop solicits new input if the user has
    // not entered a number from 0 to 49. 
    while(g_count <= 4) 
    {
        //cout << "This is guess " << g_count + 1 << endl;  // <---- debugging
        if(checkInput(user_guess) == false) 
        {
            cout << "That input is invalid.  Guess again: ";
            cin >> user_guess;
        }
        else
        {
            int user_guess_num = atoi(user_guess);
            if(user_guess_num == x) 
            {
                cout << "That's right!  You're either lucky, psychic, or have reverse-engineered the C++ random number generator." << endl;
                return 0;
            } 
            else 
            {
                if(g_count < 4)
                {
                    cout << "Nope, guess again: ";
                    cin >> user_guess;
                    g_count++;
                }
                else
                {
                    g_count++;
                }
            }
        }
    }
    cout << "You lose! No hard feelings, chump. :)" << endl;
    return 0;
}
