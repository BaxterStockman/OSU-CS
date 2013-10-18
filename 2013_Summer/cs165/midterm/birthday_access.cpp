// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 08:01:51 AM
//  Last Modification Date:
//  Filename:                   birthday_access.cpp
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
#include <iostream>
using namespace std;
class DayOfYear
{
public:
    void input();
    void output();
    // other public members
private:
    int month;
    int day;
    // other private members
};

void DayOfYear::input()
{
    int i = 0;
}

int main()
{
    DayOfYear birthDay;
//    birthDay.input(); // a)
    birthDay.day = 25; // b)
    //cout << birthDay.month; // c)
    //if(birthDay.month == 1) // d)
    //    cout << "January\n";
}

