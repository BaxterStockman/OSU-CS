// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/13/2013 07:03:38 PM        
//  Last Modification Date:
//  Filename:                   sum_overflow.cpp    
//
//  Overview:        
//      This program adds to ints together, catching overflow.  It is a work in progress;
//      i.e. it needs progress before it works... 
//  Input:       
//      The program asks the user to input two numbers to add together.  If the program
//      detects overflow, it will ask the user whether to truncate the number, or to abort. 
//         
//  Output:   
//      The program outputs the sum of two numbers.  If the sum would overflow the int type,
//      and the user chooses to truncate, the program outputs the max int value from climits
//      and the amount by which the sum of the two numbers exceeds that. 
//
// =====================================================================================

#include<iostream>
#include<climits>

using namespace std;

const int intMax = INT_MAX;

int main() {
    
    int leftHandNum;
    int rightHandNum;
    string truncOrNot;
    
    // solicit user input for two numbers to add together
    cout << "Please enter the first number to add: ";
    cin >> leftHandNum;

    cout << "Please enter the second number to add: ";
    cin >> rightHandNum;

    int overflowVal = leftHandNum - intMax + rightHandNum;

    if ( overflowVal > 0) {
        cout << "This addition will produce overflow." << endl;
        cout << "Select Y to truncate, N to abort (Y/n): ";
        
        cin.ignore(10000, '\n');
        getline (cin, truncOrNot);
   
        for(;;) {
            if(truncOrNot == "no" || truncOrNot == "No" || truncOrNot == "n" || truncOrNot == "N")
            {
                return 0; 
            }
            else if(truncOrNot == "" || truncOrNot == "yes" || truncOrNot == "y" || truncOrNot == "Y")
            {
                cout << "The sum of those two numbers is " << intMax << " plus " << overflowVal << "." << endl;
                cout << "You'll have to do the rest by hand." << endl;
                return 0;
            }
            else
            {
                cout << "I'm sorry, I don't recognize that.  Truncate (Y/n)?: ";
                getline (cin, truncOrNot);
                continue;
            }

        }
    }
    else {
        int sum = leftHandNum + rightHandNum;
        cout << "The sum of those two numbers is " << sum << endl;
    }
return 0;
}
