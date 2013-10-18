// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/04/2013 02:35:32 PM
//  Last Modification Date:
//  Filename:                   argv_type.cpp
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
#include<typeinfo>
#include<cstring>

using namespace std;

int main(int argc, char **argv)
{
    for(int i1 = 0; i1 < argc; i1++)
    {
        for(int i2 = 0; i2 < strlen(argv[i1]); i2++)
        {
            cout << "The type of variable in argv[" << i1 << "][" << i2 << "] is " << typeid(argv[i1][i2]).name() << endl;
            cout << "and the contents of argv[" << i1 << "][" << i2 << "] are " << argv[i1][i2] << endl;
        }
    }
    return 0;
}
