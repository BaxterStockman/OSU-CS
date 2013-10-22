// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/15/2013 07:27:40 PM        
//  Last Modification Date:
//  Filename:                   read_file.cpp    
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
#include<string>

using namespace std;

int main()
{
    string firstName, lastName;
    int score;
    fstream inputStream;

    inputStream.open("player.txt");

    inputStream >> score;
    inputStream >> firstName >> lastName;

    cout << "Name: " << firstName << " " << lastName << endl;
    cout << "Score: " << score << endl;
    inputStream.close();

    return 0;
}
