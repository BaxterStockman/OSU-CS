// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/23/2013 12:12:32 PM
//  Last Modification Date:     07/30/2013 09:37:00 AM
//  Filename:                   combination.cpp
//
//  Overview:
//      This program is designed to elucidate differences in completion time between
//      a function that calculates the number of ways of choosing 'r' distinct items
//      from a set containing 'n' items using iteration and a function that accomplishes
//      the same task using recursion.
//
//  Input:
//      The user inputs the size of the number used as the base for the factorial
//      computation by passing a command line argument in the form
//      './factor_recurse_alts -n 20 -r 3', where -'n 20' signifies the cardinality of the set
//      and '-r 3' signifies the number of items to be chosen
//
//  Output:
//      The program prints to the console the time in seconds or microseconds it takes for
//      each function to complete.
//
// =====================================================================================

#include <iostream>
#include <sys/time.h>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;

// Purpose: calculate C(n,r) using recursion
// Entry:   n and r are the values of n and r in C(n,r), and nLessR is the value
//          of r subtracted from n
// Exit:    returns the value of C(n,r)
long double factor_recurse(long double n, long double r, long double nLessR);

// Purpose: calculate C(n,r) using recursion
// Entry:   n and r are the values of n and r in C(n,r)
// Exit:    returns the value of C(n,r)
long double factor_iterate(long double n, long double r);

// Purpose: calculate C(n,r) using a different method of recursion
// Entry:   n and r are the values of n and r in C(n,r)
// Exit:    returns the value of C(n,r)
long double factor_recurse_alt(long double n, long double r);

int main()
{
    long double choiceRecurse;      // holds the value of C(n,r) as calculated recursively
    long double choiceRecurseAlt;   // holds the value of C(n,r) as calculated by an alternate recursive method
    long double choiceIterate;      // holds the value of C(n,r) as calculated iteratively

    long double setItems;       // holds the number of items in the set
    long double choiceItems;    // holds the number of items to choose from the set
    long double itemDiff;       // holds the difference between the number of items in the set and the number to choose

    cout << "How many items in the set? ";
    cin >> setItems;

    cout << "How many items to pick from the set? ";
    cin >> choiceItems;

    itemDiff = setItems - choiceItems;

    typedef struct timeval time;
    time stop, start;

    // time how long it takes the recursive function to calculate C(n,r)
    gettimeofday(&start, NULL);
    choiceRecurse = factor_recurse(setItems, choiceItems, itemDiff);
    gettimeofday(&stop, NULL);

    cout << "The number of ways to choose " << choiceItems << " from " << setItems << " is " << choiceRecurse << endl;

    if(stop.tv_sec > start.tv_sec)
        cout << "It took " << stop.tv_sec-start.tv_sec << " seconds for the recursive function to return its result." << endl;
    else
        cout << "It took " << stop.tv_usec-start.tv_usec << " microseconds for the recursive function to return its result." << endl;

    // time how long it takes the iterative function to calculate C(n,r)
    gettimeofday(&start, NULL);
    choiceIterate = factor_iterate(setItems, choiceItems);
    gettimeofday(&stop, NULL);

    cout << "The number of ways to choose " << choiceItems << " from " << setItems << " is " << choiceIterate << endl;

    if(stop.tv_sec > start.tv_sec)
        cout << "It took " << stop.tv_sec-start.tv_sec << " seconds for the iterative function to return its result." << endl;
    else
        cout << "It took " << stop.tv_usec-start.tv_usec << " microseconds for the iterative function to return its result." << endl;

    // time how long it takes the alternate recursive function to calculate C(n,r)
    gettimeofday(&start, NULL);
    choiceRecurseAlt = factor_recurse_alt(setItems, choiceItems);
    gettimeofday(&stop, NULL);

    cout << "The number of ways to choose " << choiceItems << " from " << setItems << " is " << choiceRecurseAlt << endl;

    if(stop.tv_sec > start.tv_sec)
        cout << "It took " << stop.tv_sec-start.tv_sec << " seconds for the recursive function I found online and still don't understand to return its result." << endl;
    else
        cout << "It took " << stop.tv_usec-start.tv_usec << " microseconds for the recursive function I found online and still don't understand to return its result." << endl;

    return 0;
}

// I made some changes to this function so that it is capable of handling all cases
// of inequalities between n, r, and nLessR.  The previous version worked only
// if r was less than both n and nLessR, or if all values were equal.
long double factor_recurse(long double n, long double r, long double nLessR)
{
    if (n > 1 && r > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, r-1, nLessR-1);
    else if(n > 1 && r > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, r-1, 1);
    else if (n > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, 1, nLessR-1);
    else if (n > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, 1, 1);
    else if (r > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, r-1, nLessR-1);
    else if (r > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, r-1, 1);
    else if (nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, 1, nLessR-1);
    else
        return 1;
}

long double factor_iterate(long double n, long double r)
{
    long double choices = 1;

    for(long double rIndex = r; rIndex > 0; rIndex--)
    {
        choices /= rIndex;
    }
    for(long double diffIndex = n-r; diffIndex > 0; diffIndex--)
    {
        choices /= diffIndex;
    }
    for(long double nIndex = n; nIndex > 0; nIndex--)
    {
        choices *= nIndex;
    }
    return choices;
}

long double factor_recurse_alt(long double n, long double r)
{
    cout << "Recursing..." << endl;
    if( n==r || r==0)
        cout << "Returning 1..." << endl;
        return 1;
    return factor_recurse_alt(n-1, r) + factor_recurse_alt(n-1, r-1);   // What is going on here?
}

