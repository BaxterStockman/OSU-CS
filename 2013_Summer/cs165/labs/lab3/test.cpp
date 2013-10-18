// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/12/2013 07:24:59 PM        
//  Last Modification Date:     07/12/2013 07:34:13 PM
//  Filename:                   test.cpp    
//
//  Overview:        
//      This program asks the user whether he likes Vi iMproved or not. 
//       
//  Input:       
//      When prompted, the user must enter '0' for no/false or '1' for yes/true 
//         
//  Output:   
//      The program parrots the user's answer by printing a phrase to the console 
//
// =====================================================================================

#include<iostream>

using namespace std;

int main ()
{
    int likesvim;

    cout << "Do you like Vi iMproved? (0 = no, 1 = yes) ";

    cin >> likesvim;

    if(likesvim == 0)
        cout << "You hate Vim?!" << endl;
    else
        cout << "You love Vim and are obviously a person of impeccable discernment!" << endl;

return 0;
}
