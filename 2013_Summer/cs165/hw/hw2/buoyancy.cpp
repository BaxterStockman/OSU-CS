// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/14/2013 08:52:02 AM        
//  Last Modification Date:
//  Filename:                   buoyancy.cpp    
//
//  Overview:        
//      Calculates whether a sphere with parameters entered by the user will sink or
//      float when placed in water. 
//       
//  Input:       
//      The user enters the weight and radius of the sphere. 
//         
//  Output:   
//      If the sphere will float according to the buoyancy equation, the program
//      prints a message to that effect to the console; otherwise the program prints
//      that the sphere will not float. 
//
// =====================================================================================

#include<iostream>  // for cout and cin
#include<math.h>    // for constant M_PI
#include<cmath>     // for pow()

#define WTR_SPEC_WT 62.4

using namespace std;

/************************************************************************************
 * Entry:   none
 *
 * Exit:    Exits when the user indicates that he no longer wishes to calculate
 *          whether a certain sphere will float in water.
 *
 * Purpose: Prompt the user to enter the weight and radius of a sphere, then
 *          calculate whether it will float in water.  Restart, prompting for
 *          new input, until user indicates exit.
 ***********************************************************************************/
int main()
{
    float weight;
    float radius;
    float volume;
    float buoyForce;
    bool cont = true;
    string keepGoing;

    while(cont == true)
    {
        // Ask user for sphere parameters
        cout << "Enter the weight of the sphere: ";
        cin >> weight;

        cout << "Enter the radius of the sphere: ";
        cin >> radius;

        // calculate sphere volume
        volume = (4.0 / 3.0) * M_PI * pow(radius, 3);

        // calculate the sphere's buoyancy force
        buoyForce = volume * WTR_SPEC_WT;

        // if sphere will float, say so; if not, say not
        if(buoyForce >= weight)
        {
            cout << "This sphere will float." << endl;
        }
        else
        {
            cout << "This sphere will not float." << endl;
        }

        cin.ignore(10000, '\n');    // clear stream for new input

        cout << "Start again with new sphere (Y/n)? ";
        getline(cin, keepGoing);
 
        // check whether keepGoing matches particular values; restart main loop for empty string
        // or any of four strings signifying 'yes'; set contProg to false for any of four strings
        // signifying 'no'; loop until user enters valid selection.
        while(1)
        {
            if(keepGoing == "no" || keepGoing == "No" || keepGoing == "n" || keepGoing == "N")
            {
                cont = false;
                break;
            }
            else if(keepGoing == "" || keepGoing == "yes" || keepGoing == "y" || keepGoing == "Y")
            {
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

