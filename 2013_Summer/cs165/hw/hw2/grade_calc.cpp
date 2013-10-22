// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/14/2013 07:23:18 AM        
//  Last Modification Date:     07/14/2013 08:49:41 AM
//  Filename:                   grade_calc.cpp    
//
//  Overview:        
//      This program calculates the overall student grade based upon the student's
//      performance on the various assignments.
//       
//  Input:       
//      The user inputs (1) the total number of assignments to be considered; (2) the
//      score received on each assignment, and (3) the total points possible for the
//      assignment. 
//         
//  Output:   
//      The program outputs the total score expressed as (1) number of points earned
//      out of possible points, and (2) a percentage 
//
// =====================================================================================

#include<iostream>  // to enable cout and cin
#include<iomanip>   // to enable fixed() and setprecision()

using namespace std;


/***************************************************************************************
 * Entry:   None.
 *
 * Exit:    Exits when calculation of grade is complete
 *
 * Purpose: Solicits the user to enter (1) the number of assignments, (2) the points
 *          received on each assignment, and (3) the points possible on each assignment.
 *          Calculates the total number of points received and the total number of points
 *          possible, and then expresses these as a percentage grade.
 **************************************************************************************/

int main()
{
    int totalExer;      // holds the number of exercises
    int possPtsTotal;   // holds the total number of points possible
    int possPtsExer;    // holds the number of points possible on an exercise
    int studPtsTotal;   // holds the total number of points earned
    int studPtsExer;    // holds the number of points earned on an exercise
    float gradePercent; // holds the grade as a percentage

    cout << "How many exercises to input?: ";
    cin >> totalExer;

    // This loop iterates for the number of exercises entered by the user.
    // It solicits the score received on an exercise, adding this to the 
    // total points received.  It then solicits the points possible on an
    // exercise, adding this to the total number of points possible.
    // The points received and points possible on an exercise are then
    // reset to zero for the next loop.
    for(int exerNum = 1; exerNum <= totalExer; exerNum++)
    {
        cout << "Score received for exercise " << exerNum << ": ";
        cin >> studPtsExer;
        studPtsTotal += studPtsExer;
        studPtsExer = 0;

        cout << "Total points possible for exercise " << exerNum << ": ";
        cin >> possPtsExer;
        possPtsTotal += possPtsExer;
        possPtsExer = 0;
    }

    // This calculates the grade as a percentage, using static_cast<float> on
    // the numerator so that the fraction does not get rounded.
    gradePercent = 100 * ( static_cast<float>(studPtsTotal) / possPtsTotal );

    // Outputs raw score and percentage grade.  Uses fixed and setprecision(2)
    // so that the percentage grade will always be expressed in the form
    // '78.55'.
    cout << "Your total is " << studPtsTotal << " out of " << possPtsTotal
        << ", or " << fixed << setprecision(2) << gradePercent << "%" << endl;
    
    return 0;
}
