// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 07:38:08 AM
//  Last Modification Date:
//  Filename:                   person_struct.cpp
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

struct Date
{
    int year;
};

struct Person
{
    Date birthDay;
};

int main()
{
    Person Bill;
    Bill.birthDay.year = 1983;

    cout << "Bill was born in " << Bill.birthDay.year << endl;

    return 0;
}
