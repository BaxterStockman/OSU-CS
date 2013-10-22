// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 07:55:20 AM
//  Last Modification Date:
//  Filename:                   gecko.cpp
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
    char letter[5] = {'o', 'k', 'c', 'e', 'g'};
    for(int i = 4; i >= 0; i-- )
    cout << letter[i];
    cout << endl;

    return 0;
}
