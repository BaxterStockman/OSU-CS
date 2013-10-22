// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 07:56:46 AM
//  Last Modification Date:
//  Filename:                   matrix.cpp
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
    int matrix[2][3];
    int k = 0;

    for(int i =0; i < 2; i++)
        for (int j=0; j < 3; j++)
            matrix[i][j] = k++;

    for(int i =0; i < 2; i++)
    {
        for (int j=0; j < 3; j++)
            cout << matrix[i][j] << '\t';
        cout << endl;
    }

    return 0;
}
