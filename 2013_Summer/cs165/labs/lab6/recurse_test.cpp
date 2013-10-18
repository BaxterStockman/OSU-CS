// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/23/2013 01:24:34 PM
//  Last Modification Date:
//  Filename:                   recurse_test.cpp
//
//  Overview:
//
//
//
//  Input:
//      <+text+>
//
//  Output:
//      <+text+>
//
// =====================================================================================

#include<iostream>
#include<cstdlib>

using namespace std;

int factor(int n, int r, int diff, bool recurse);

int main()
{
    int n;
    int r;
    int diff;
    int factorial;
    int choice;

    cout << "Enter the number of items in the set: ";
    cin >> n;
    cout << "Enter the number of items to choose: ";
    cin >> r;

    diff = n - r;

    choice = factor(n, r, diff, false);

    //cout << "The factorial of " << base << " is " << factorial << endl;

    cout << "The number of ways to choose " << r << " items from a set of " << n << " items is: " << choice << endl;
    return 0;
}

int factor(int n, int r, int diff, bool recurse)
{
    int nFactor;
    int rFactor;
    int diffFactor;

    if(diff == 0)
        return 1;

    if(recurse == false)
    {
        if(n == 1)
            nFactor = 1;
        else
        {
            nFactor = n * factor(n - 1, 1, 0, 1);
        }
    }
    else
    {
        if(n == 1)
            return 1;
        else
        {
            nFactor = n * factor(n - 1, 1, 0, 1);
            return nFactor;
        }
    }
    cout << "The factorial of " << n << " is: " << nFactor << endl;

    if(recurse == false)
    {
        if(r == 1)
            rFactor = 1;
        else
        {
            rFactor = r * factor(0, r - 1, 1, 1);
        }
    }
    else
    {
        if(r == 1)
            return 1;
        else
        {
            rFactor = r * factor(0, r - 1, 1, 1);
            return rFactor;
        }
    }
    cout << "The factorial of " << r << " is: " << rFactor << endl;

    if(recurse == false)
    {
        if(diff == 1)
            diffFactor = 1;
        else
        {
            diffFactor = diff * factor(0, 0, diff - 1, 1);
        }
    }
    else
    {
        if(diff == 1)
            return 1;
        else
        {
            diffFactor = diff * factor(0, 0, diff - 1, 1);
            return diffFactor;
        }
    }
    cout << "The factorial of " << diff << " is: " << diffFactor << endl;

    return (nFactor / (rFactor * diffFactor));

}
