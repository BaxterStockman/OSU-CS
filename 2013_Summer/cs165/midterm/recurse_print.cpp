// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 07:48:40 AM
//  Last Modification Date:
//  Filename:                   recurse_print.cpp
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

void recursive(int i);

int main()
{
    int num = 5;
    recursive(num);

    return 0;
}

void recursive( int i )
{
    if ( i < 8 )
    {
        i++;
        recursive(i);
        cout << i << " ";
    }
}
