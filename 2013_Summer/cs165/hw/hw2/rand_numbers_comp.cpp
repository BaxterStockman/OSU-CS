// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/14/2013 09:58:44 PM        
//  Last Modification Date:     07/14/2013 01:43:23 PM
//  Filename:                   rand_numbers_comp.cpp    
//
//  Overview:        
//      This program asks the user to choose a number 0 < x < 50, and gives the computer
//      five tries to guess it.  If the computer fails after five tries, the program exits; 
//      the program also exits if the computer guesses correctly.
//       
//  Input:       
//      The user must indicate whether the computer's guess is high, low, or correct,
//      any other input causes the program to raise an exception. 
//         
//  Output:   
//      The program informs users of its guesses by printing messages to the console; 
//      the program also informs users of invalid inputs. 
//
// =====================================================================================

#include<iostream>  // included to allow for cout and cin
#include<ctime>     // included to allow time()
#include<cstdlib>   // included to allow rand(), srand() and atoi()
#include<string.h>  // included to allow strlen()

using std::cout;
using std::cin;
using std::endl;
using std::string;

/***************************************************************************************
 *  Entry:      Called by int main() to check the value of certain user input
 *
 *  Exit:       Exits when input confirmed to be within specified parameters
 *
 *  Purpose:    This function verifies that the user's input is a one-digit number
 *              from 1 to 3.  It returns false for any other value, including when the 
 *              input contains alphas. 
 *
 *  Source: http://answers.yahoo.com/question/index?qid=20090130125311AAHSc74
 *  Answer provider: BobearQSI 
 **************************************************************************************/
bool CheckInput(const char *str)
{
    if (strlen(str) == 0 || strlen(str) > 1)    // check if string is empty or longer than one character
        return false;
    while (*str < '0' || *str > '9')
    {
        return false;                           // returns false when character not a digit
        str++;                                  // move to next character
    } 
    if (atoi(str) < 1 || atoi(str) > 3)         // convert to number and check whether in proper range
        return false;
    else
        return true;
}

/***************************************************************************************
 * Entry:   Called by int main() to generate random guess subsequent to preset first
 *          guess
 *
 * Exit:    Exits after having calculated and returned a random nubmer within 
 *          a specified range
 *
 * Purpose: Generate a random number falling within (but not equalling) certain upper
 *          and lower bounds.
 **************************************************************************************/
int RandGuess(int lowNum, int highNum)
{
    int x;              // int to hold random number
    srand(time(NULL));  // seeds random number generator

    int range = highNum - lowNum;               // range to use for modulus
    x = (rand() % (range - 1)) + lowNum + 1;    // ensures that random number always lies within range
                                                // but not at either upper or lower bound
    return x;   // return random number
}  

/***************************************************************************************
 * Entry:       None.
 *
 * Exit:        Exits, returning 0, after either (1) a correct guess, or (2) 5 incorrect
 *              guesses.
 *
 * Purpose:     The computer attempts to guess a user-chosen number from 1 to 49.
 ***************************************************************************************/
int main() 
{
    int g_count = 0;        // keeps track of the number of guesses
    int lowEnd = 0;         // lower bound of guess range
    int highEnd = 50;       // upper bound of guess range
    int guess = 25;         // computer's guess value, initialized to 25
    char user_input[80];    // holds the user's input as to whether guess is low, high, or correct
    string guessOrd;        // holds the ordinal value of the computer's guess

    cout << "Think of a number between 1 and 49, and I will attempt to guess it." << endl;
    cout << "Press [ENTER] when you are ready.";
    cin.ignore();           // hold up until user presses enter

    // This while loop continues so long as the computer's guesses do not exceed 5.  
    // Upon a fifth incorrect guess, the loop exits.  The loop also exits if the user
    // indicates that the computer guessed correctly.  The loop solicits new user input
    // if the user has not selected one of three valid values. 
    while(g_count <= 4) 
    {
        // The following if/else if block changes the ordinal value
        // outputted when the computer prints its guess, i.e. 'My
        // **first** guess is...'
        if(g_count == 0)
        {
            guessOrd = "first";
        }
        else if(g_count == 1)
        {
            guessOrd = "second";
        }
        else if(g_count == 2)
        {
            guessOrd = "third";
        }
        else if(g_count == 3)
        {
            guessOrd = "fourth";
        }
        else if(g_count == 4)
        {
            guessOrd = "fifth";
        }

        // Prompt the user to tell the computer where its guess falls
        // in relation to the number chosen by the user
        cout << "My " << guessOrd << " guess is " << guess << "." << endl;
        cout << "Enter 1 for too low, 2 for perfect, 3 for too high. ";
        cin >> user_input;

        // Reject invalid answers
        if(CheckInput(user_input) == false) 
        {
            cout << "That input is invalid.  Please enter 1 for too low, 2 for perfect, 3 for too high. "; 
            cin >> user_input;
        }
        
        // Print "Yeah!" and exit if user indicates correct guess.
        // 
        // Currently configured, for low guesses, to set the lower
        // bound to the low guess, then make a random guess in the
        // new range.  For high guesses, sets the upper bound to the
        // high guess then makes a random guess in the new range.
        //
        // By commenting out the lines with 'guess = randomGuess()'
        // and uncommenting the subsequent line, can be changed as follows:
        // For low guesses, set the lower bound for the next guess to
        // the value of the low guess, and set the new guess value to 
        // halfway between the lower and upper bounds.  For high guesses,
        // set the high end to the value of the high guess, and set the new
        // guess value to halfway between the lower and upper bounds.
        else
        {
            int user_input_num = atoi(user_input);
            if(user_input_num == 2) 
            {
                cout << "Yeah!" << endl; 
                return 0;
            } 
            else if(user_input_num == 1) 
            {
                lowEnd = guess;
                guess = RandGuess(lowEnd, highEnd);
                // guess = guess + ((highEnd - lowEnd) / 2);
                g_count ++;
            }
            else 
            {
                highEnd = guess;
                guess = RandGuess(lowEnd, highEnd);
                //guess = guess - ((highEnd - lowEnd) / 2);
                g_count ++;
            }
        }
    }
    cout << "You win! :(" << endl;
    return 0;
}
