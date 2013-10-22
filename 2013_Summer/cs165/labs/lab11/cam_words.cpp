// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/25/2013 03:09:00 PM
//  Last Modification Date:
//  Filename:                   cam_words.cpp
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
#include<fstream>
int main()
{
    int t=0,n,w;
    std::string u;
    std::fstream f("words.txt");
    while(f>>u)
    {
        w=0;
        for(n=0;n<u.length()-2;)
            //std::cout << u.at(n) - 64 << std::endl;
            w+=u.at(++n)-64;
            if(n <= u.length())
                std::cout << u.at(n) << std::endl;
            //w+=u[++n]-64;
        for(n=0;w>=(n*++n)/2;)
            if(w==(n*n+n)/2)
                t++;
    }
    std::cout<<t<<(w?" words":" file error")<<"\n";
    f.close();
}
