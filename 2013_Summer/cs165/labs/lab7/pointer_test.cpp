// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/29/2013 04:00:03 PM
//  Last Modification Date:
//  Filename:                   pointer_test.cpp
//
//  Overview:
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

using namespace std;

int main()
{
    double *d;  // Only the very breast pointers for this program!
    double **dp; // Talk about 'overloading'!

    double d1, d2, d3;

    d1 = 7.8;
    d2 = 10.0;
    d3 = .009;

    dp = &d;

    cout << endl;

    d = &d1;
    cout << "*d = " << *d << endl;
    cout << "d = " << d << endl;
    cout << "**dp = " << **dp << endl;
    cout << "*dp = " << *dp << endl;
    cout << "dp = " << dp << endl;
    cout << "d1 = " << d1 << endl;
    cout << "&d1 = " << &d1 << endl;
    cout << endl;

    d = &d2;
    cout << "*d = " << *d << endl;
    cout << "d = " << d << endl;
    cout << "**dp = " << **dp << endl;
    cout << "*dp = " << *dp << endl;
    cout << "dp = " << dp << endl;
    cout << "d2 = " << d2 << endl;
    cout << "&d2 = " << &d2 << endl;
    cout << endl;

    d = &d3;
    cout << "*d = " << *d << endl;
    cout << "d = " << d << endl;
    cout << "**dp = " << **dp << endl;
    cout << "*dp = " << *dp << endl;
    cout << "dp = " << dp << endl;
    cout << "d3 = " << d3 << endl;
    cout << "&d3 = " << &d3 << endl;
    cout << endl;


    return 0;
}
