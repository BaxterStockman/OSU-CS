// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/13/2013 11:22:04 AM        
//  Last Modification Date:
//  Filename:                   summation.cpp    
//
//  Overview:        
//      This program takes an integer 'n' as inputted by the user and calculates the sum of
//      the expressions x^5 + 10 for all x <= n.
//
//      The purpose of the program is, inter alia, to familiarize the student with
//      the use of flow control, in particular for loops and while loops
//       
//  Input:       
//      The user is prompted to enter an integer, and is prompted to choose whether to
//      calculate a new sum after the previous iteration of the program is complete.
//         
//  Output:   
//      The program outputs the sum described above. 
//
// =====================================================================================

#include<iostream>  // included for cout and cin
#include<cmath>     // included for pow()

using namespace std;

/***************************************************************************************
 * Entry:   Called for all values {1, 2, 3, ..., x}, where x is the value chosen by
 *          the user.
 *
 * Exit:    Exits when calculation complete
 *
 * Purpose: Compute the value of a function f(x) for inputted int x
 *
 **************************************************************************************/

long calcExp(int varValue)
{
    long funcOut = pow(varValue, 5) + 10;
    return funcOut;
}

/***************************************************************************************
 * Entry:   None
 *
 * Exit:    Exit when user inputs correct selection(s) for terminating for loop
 *
 * Purpose: Calculates the sums of f(x) for values of x from {1, 2, 3, ..., x}, where x is
 *          the integer inputted by the user.
 **************************************************************************************/

int main()
{   
    int inputValue;         // value inputted by user
    long outputValue = 0;   // value of the sum
    string keepGoing;       // used to hold user selection of whether to continue program
    
    // This is the main part of the program.  It continues while the value of the Boolean
    // 'contProg' is true and exits when it is false.
    for (bool contProg = true; contProg == true;)
    {
        cout << "Enter the number 'x' to calculate f(x) = (1)^5 + 10 + (2)^5 + 10 + ... + (x)^5 + 10: ";
        cin >> inputValue;

        // This loop verifies that the user has in fact entered an int.
        // The expression 'cin' returns truth value 1 whenever the value
        // inputted is a valid instance of the variable type to which
        // it is assigned.  Thus, the loop continues so long as the
        // value inputted is NOT a valid instance of the variable type
        // to which is is assigned.
        for(bool inputValid = cin; inputValid == false;)
        {
            if(!cin)
            {
                cout << "That input is invalid.  Try again: ";
                cin.clear();                // This clears the cin error flag
                cin.ignore(10000, '\n');    // This moves to next \n to make way for new input
                cin >> inputValue; 
            }
            else
            {  
                inputValid = true;
            }
        }

        // This loop calls the function 'calcExp()' for all values {1, 2, 3, ... inputValue}
        // adding the returned value to outputValue.  It exits when the int baseValue exceeds
        // inputValue
        for(int baseValue = 1; baseValue <= inputValue; baseValue++)
        {
            outputValue += calcExp(baseValue);
        }

        cout << "The sum of the series is: " << outputValue << endl; // Print the series sum
       
        cin.ignore(10000, '\n'); 
        
        cout << "Continue (Y/n)?: ";    // ask the user whether to start loop again
        getline (cin, keepGoing);       // store user's answer in string keepGoing
        

        // check whether keepGoing matches particular values; restart main loop for empty string
        // or any of four strings signifying 'yes'; set contProg to false for any of four strings
        // signifying 'no'; loop until user enters valid selection.
        for(;;)
        {
            if(keepGoing == "no" || keepGoing == "No" || keepGoing == "n" || keepGoing == "N")
            {
                contProg = false;
                break;
            }
            else if(keepGoing == "" || keepGoing == "yes" || keepGoing == "y" || keepGoing == "Y")
            {
                outputValue = 0;
                break;
            }
            else
            {
                cout << "I'm sorry, I don't recognize that.  Continue (Y/n)?: ";
                getline (cin, keepGoing);
                continue;
            }
        } 
    }
    return 0;
}
