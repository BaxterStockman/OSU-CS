// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/06/2013 unknown time
//  Last Modification Date:     07/07/2013 11:10:15 PM
//  Filename:                   function.cpp    
//
//  Overview:        
//      This program computes the value of a set of different expressions.  Its purpose is to
//      familiarize the student (that's me!) with some of the basic mathematical functions included
//      in the <cmath> library. 
//       
//  Input:       
//      There is no input from the user; all variables are defined using values set within the
//      source code. 
//         
//  Output:   
//      The program should output the size in bytes of variable types char, int, float, long,
//      short, double, and bool
//
// =====================================================================================


#include<iostream>

#define NUM_BITS_IN_BYTE 8  // define a constant to hold the number of bits in byte

using namespace std;

// Print to the console the size in bits of variable types char, int, float, long,
// short, double, and bool
int main()
{
cout << "A char takes up " << sizeof(char) * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A int takes up " << sizeof(int)  * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A float takes up " << sizeof(float)  * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A long takes up " << sizeof(long)  * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A short takes up " << sizeof(short)  * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A double takes up " << sizeof(double)  * NUM_BITS_IN_BYTE << " bits of memory\n";
cout << "A bool takes up " << sizeof(bool)  * NUM_BITS_IN_BYTE << " bits of memory\n";
}
