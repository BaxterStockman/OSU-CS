// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               09/03/2013 02:00:23 PM
//  Last Modification Date:
//  Filename:                   recur.cpp
//
//  Overview:
//
//
//  Input:
//
//
//  Output:
//      <+text+>
//
// =====================================================================================

#include<iostream>

int diver(int x, int y)
{
    if(x <= y)
    {
        y -= (x/10);
        return diver(x, y);
    }
    else
    {
        return x - y;
    }
}

int main()
{
    int final = diver(15, 30);

    std::cout << "Final number is: " << final << std::endl;

    return 0;
}
