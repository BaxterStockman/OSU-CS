// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 12:46:07 PM
//  Last Modification Date:
//  Filename:                   change_val.cpp
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

void change_values(int x, int y[])
{
    x = 3;
    y[0] = 3;
}

int main()
{
    int x = 0;
    int y[1];

    change_values(x, y);

    cout << "x: " << x << endl << "y[0]: " << y[0] << endl;
    return 0;

}
