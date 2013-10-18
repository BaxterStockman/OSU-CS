/*
 * =====================================================================================
 *
 *       Filename:  hw1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/26/2013 11:44:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include<iostream>
#include<cmath>
#include<climits>

using namespace std;

int main() {

    // Declaring and assigning variables

    // short ints
    short int maxShort = SHRT_MAX;
    short int minShort = SHRT_MIN;
    
    // unsigned short ints
    unsigned short int umaxShort = USHRT_MAX;
    
    // ints
    int maxInt = INT_MAX;
    int minInt = INT_MIN;
    
    // unsigned ints
    unsigned int umaxInt = UINT_MAX;
    
    // long ints
    long int maxLong = LONG_MAX;
    long int minLong = LONG_MIN;
    
    // unsigned long ints
    unsigned long int umaxLong = ULONG_MAX;
    
    cout << "\n";
    cout << "I will now print the largest and smallest values of the macros\n";
    cout << "short, int and long, in decimal, hexadecimal, and octal notation.\n";
    cout << "\n";
    
    // Print the values of the variables to the console in decimal format
    cout << "largest short integer is " << dec << maxShort << " in decimal.\n";
    cout << "smallest short integer is " << dec << minShort << " in decimal.\n";
    cout << "largest unsigned short integer is " << dec << umaxShort << " in decimal.\n";
    cout << "largest integer is " << dec << maxInt << " in decimal\n";
    cout << "smallest integer is " << dec << minInt << " in decimal.\n";
    cout << "largest unsigned integer is " << dec << umaxInt << " in decimal.\n";
    cout << "largest long integer is " << dec << maxLong << " in decimal.\n";
    cout << "smallest long integer is " << dec << minLong << " in decimal.\n";
    cout << "largest unsigned long integer is " << dec << umaxLong << " in decimal.\n";
    cout << "\n";

    // Print the values of the variables to the console in hexadecimal format
    cout << "largest short integer is " << hex << maxShort << " in hexadecimal.\n";
    cout << "smallest short integer is " << hex << minShort << " in hexadecimal.\n";
    cout << "largest unsigned short integer is " << hex << umaxShort << " in hexadecimal.\n";
    cout << "largest integer is " << hex << maxInt << " in hexadecimal\n";
    cout << "smallest integer is " << hex << minInt << " in hexadecimal.\n";
    cout << "largest unsigned integer is " << hex << umaxInt << " in hexadecimal.\n";
    cout << "largest long integer is " << hex << maxLong << " in hexadecimal.\n";
    cout << "smallest long integer is " << hex << minLong << " in hexadecimal.\n";
    cout << "largest unsigned long integer is " << hex << umaxLong << " in hexadecimal.\n";
    cout << "\n";

    // Print the values of the variables to the console in octal format
    cout << "largest short integer : " << oct << maxShort << " in octal.\n";
    cout << "smallest short integer is " << oct << minShort << " in octal.\n";
    cout << "largest unsigned short integer is " << oct << umaxShort << " in octal.\n";
    cout << "largest integer is " << oct << maxInt << " in octal\n";
    cout << "smallest integer is " << oct << minInt << " in octal.\n";
    cout << "largest unsigned integer is " << oct << umaxInt << " in octal.\n";
    cout << "largest long integer is " << oct << maxLong << " in octal.\n";
    cout << "smallest long integer is " << oct << minLong << " in octal.\n";
    cout << "largest unsigned long integer is " << oct << umaxLong << " in octal.\n";
    cout << "\n";
   
    // Print the values of the variables to the console in decimal format
    // adding 1 to the largest values and subtracting 1 from the smallest
    cout << "\n";
    cout << "When you add one to the largest or subtract one from the smallest,\n"; 
    cout << "\n";
    cout << "largest short integer is " << dec << static_cast<short>(maxShort + 1) << " in decimal.\n";
    cout << "smallest short integer is " << dec << static_cast<short>(minShort - 1) << " in decimal.\n";
    cout << "largest unsigned short integer is " << dec << static_cast<unsigned short>(umaxShort + 1) << " in decimal.\n";
    cout << "largest integer is " << dec << static_cast<int>(maxInt + 1) << " in decimal\n";
    cout << "smallest integer is " << dec << static_cast<int>(minInt - 1) << " in decimal.\n";
    cout << "largest unsigned integer is " << dec << static_cast<unsigned int>(umaxInt + 1) << " in decimal.\n";
    cout << "largest long integer is " << dec << static_cast<long>(maxLong + 1) << " in decimal.\n";
    cout << "smallest long integer is " << dec << static_cast<long>(minLong - 1) << " in decimal.\n";
    cout << "largest unsigned long integer is " << dec << static_cast<unsigned long>(umaxLong + 1) << " in decimal.\n";
    cout << "\n";


    // Declaring and assigning variables for the CALCULATED largest and smallest
    // values of the macros under consideration
   
    // short ints
    short int maxShortCalc = (pow (2, 15)) - 1;
    short int minShortCalc = (-1 * pow (2, 15)) + 1;
    
    // unsigned short ints
    unsigned short int umaxShortCalc = pow (2, 16) - 1;
    
    // ints
    int maxIntCalc = (pow (2, 15)) - 1;
    int minIntCalc = (-1 * pow (2, 15)) + 1;
    
    // unsigned ints
    unsigned int umaxIntCalc = pow (2, 16) - 1;
    
    // long ints
    long int maxLongCalc = pow (2, 31) - 1;
    long int minLongCalc = (-1 * pow (2, 31)) + 1;
    
    // unsigned long ints
    unsigned long int umaxLongCalc = pow (2, 32) - 1;
    
    cout << "\n";
    cout << "I will now calculate and print the largest and smallest values of the macros\n";
    cout << "short, int and long, in decimal, hexadecimal, and octal notation.\n";
    cout << "\n";

    // Print the values of the variables to the console in decimal format
    cout << "largest short integer is " << dec << maxShortCalc << " in decimal.\n";
    cout << "smallest short integer is " << dec << minShortCalc << " in decimal.\n";
    cout << "largest unsigned short integer is " << dec << umaxShortCalc << " in decimal.\n";
    cout << "largest integer is " << dec << maxIntCalc << " in decimal\n";
    cout << "smallest integer is " << dec << minIntCalc << " in decimal.\n";
    cout << "largest unsigned integer is " << dec << umaxIntCalc << " in decimal.\n";
    cout << "largest long integer is " << dec << maxLongCalc << " in decimal.\n";
    cout << "smallest long integer is " << dec << minLongCalc << " in decimal.\n";
    cout << "largest unsigned long integer is " << dec << umaxLongCalc << " in decimal.\n";
    cout << "\n";

    // return 0 and exit
    return 0;
}



