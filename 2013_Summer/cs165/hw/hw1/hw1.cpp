// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               06/26/2213 11:44:09 PM       
//  Last Modification Date:
//  Filename:                   function.cpp    
//
//  Overview:        
//      
//      This program outputs the highest and lowest values of the
//          - short;
//          - unsigned short;
//          - int;
//          - unsigned int;
//          - long; and
//          - unsigned long
//      macros from the <climits> library.  It also (attempts) to compute the values directly
//      using the pow() exponentiation function, and also to demonstrate the values returned
//      when the maximum and minimum values are overflowed.
//
//  Input:       
//      
//      There is no input from the user; all variables are defined using values set within the
//      source code. 
//         
//  Output:   
//      
//      The program outputs the various maximum and minimum values in decimal, hexadecimal, and
//      octal notation, and also outputs in decimal notation the calculated maximum and minimum
//      values, as well as the first overflow value (i.e. the value of the maximum plus one, or
//      minimum minus one).
//
//  Sample output:
//  
//      ------------------------------------------------
//      | The value of the highest unsigned short int: |
//      |                                              |
//      | In decimal:                            65535 |
//      | In hexadecimal:                         ffff |
//      | In octal:                             177777 |
//      |                                              |
//      | Calculated decimal:                    65535 |
//      |                                              |
//      | Overflows to:                              0 |
//      ------------------------------------------------ 
//
// =====================================================================================

#include<typeinfo>
#include<iostream>
#include<iomanip>
#include<cmath>
#include<climits>

#define NUM_BITS_IN_BYTE 8  // constant to hold the number of bits in a byte

using namespace std;    // This is here so that we don't have to use 'std::cout' every time
                        // we want to write to the console


// Declaring and assigning variables

short int maxShort = SHRT_MAX;                  // highest value of short int
short int minShort = SHRT_MIN;                  // lowest value of short int
unsigned short int maxShortUnsgn = USHRT_MAX;   // highest value of unsigned short int
int maxInt = INT_MAX;                           // highest value of int
int minInt = INT_MIN;                           // lowest value of int
unsigned int maxIntUnsgn = UINT_MAX;            // higest value of unsigned int
long int maxLong = LONG_MAX;                    // highest value of long int
long int minLong = LONG_MIN;                    // lowest value of long int
unsigned long int maxLongUnsgn = ULONG_MAX;     // highest value of unsigned long int


// Thanks to the fourth video, Daniel Loughlin and Nathan Anderson for the
// following means of determining the proper way to calculate the largest
// and smallest values of the macros.
//
// This invokes the sizeof() function to determine the number of bytes
// occupied by each type of macro, then converts these to bits, which are
// then used as the exponent in the pow() function.
//
// For signed macros, the bit values are decreased by one to account
// for the fact that one bit is occupied by the positive or negative sign.


// Calculating the proper exponents for use in calculating max and min values.

short shortBits = (NUM_BITS_IN_BYTE * sizeof(short)) - 1;           // bits occupied by short int
short shortBitsUnsgn = NUM_BITS_IN_BYTE * sizeof(unsigned short);   // bits occupied by unsigned short int
short intBits = (NUM_BITS_IN_BYTE * sizeof(int)) - 1;               // bits occupied by int
short intBitsUnsgn = NUM_BITS_IN_BYTE * sizeof(unsigned int);       // bits occupied by unsigned int
short longBits = (NUM_BITS_IN_BYTE * sizeof(long)) - 1;             // bits occupied by long int
short longBitsUnsgn = NUM_BITS_IN_BYTE * sizeof(unsigned long);     // bits occupied by unsigned long int


// Declaring variables for holding max and min values;
// calculating max and min values.
//
// I have not followed the convention of http://www.cplusplus.com/reference/climits/
// in adding one to get the lowest values, because this produces a number that is too
// high by one, i.e. -32767 rather than -32768 for the lowest short.  I suspect that
// this may be due to the positive-signed values starting at 0 and the negative-
// signed values starting at -1: since '0' is not counted among the negative numbers,
// there is room for one more number at the low end.

short int maxShortCalc = (pow(2, shortBits)) - 1;                   // calculated highest short int
short int minShortCalc = (-1 * pow(2, shortBits));                  // calculated lowest short int
unsigned short int umaxShortCalc = (pow(2, shortBitsUnsgn)) - 1;    // calculated highest unsigned short int
int maxIntCalc = (pow( 2, intBits)) - 1;                            // calculated highest int
int minIntCalc = (-1 * pow(2, intBits));                            // calculated lowest int
unsigned int umaxIntCalc = (pow(2, intBitsUnsgn)) - 1;              // calculated highest unsigned int
long int maxLongCalc = (pow(2, longBits)) - 1;                      // calculated highest long int
long int minLongCalc = (-1 * pow(2, longBits));                     // calculated lowest long int
unsigned long int umaxLongCalc = (pow(2, longBitsUnsgn + 1)) - 1;   // calculated highest unsigned long int
    
/* *******************************************************************************
 *  Entry:      None.
 *  
 *  Exit:       Exits when values are finished printing to the console
 *  
 *  Purpose:    Prints the values of the variables defined above to the console.
 *              Also prints the values returned when the maximum/minimum
 *              values are overflowed.
 *
 *  Errata:     Much of the code below is for making the output pretty; the code
 *              itself is anything but.  I promise my future programs will look
 *              nicer once I've learned some flow control!
 *
 * *******************************************************************************/
  
int main()
{
    // Values for the highest short int
    string preface ("| The value of the highest short int: |");
    int prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxShort << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxShort << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxShort << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << maxShortCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    // The following line prints the value of the largest short int plus one.
    // The 'static_cast' is necessary to prevent implicit casting as the
    // next largest macro type; in this case, int.  The other blocks of code
    // in the function include analogous statements.
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<short>(maxShort + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;
    
    // Values for the lowest short int
    preface.assign("| The value of the lowest short int: |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << minShort << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << minShort << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << minShort << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << minShortCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<short>(minShort - 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    // Values for the highest unsigned short int
    preface.assign("| The value of the highest unsigned short int: |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxShortUnsgn << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxShortUnsgn << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxShortUnsgn << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << umaxShortCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<unsigned short>(maxShortUnsgn + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    // Values for the highest int
    preface.assign("| The value of the highest int:   |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxInt << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxInt << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxInt << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << maxIntCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<int>(maxInt + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;
   
    // Values for the lowest int 
    preface.assign("| The value of the lowest int:      |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << minInt << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << minInt << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << minInt << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << minIntCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<int>(minInt - 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    // Values for the highest unsigned int
    preface.assign("| The value of the highest unsigned int: |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxIntUnsgn << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxIntUnsgn << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxIntUnsgn << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << umaxIntCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<unsigned int>(maxIntUnsgn + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    // Values for the highest long int
    preface.assign("| The value of the highest long int:          |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxLong << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxLong << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxLong << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << maxLongCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<long>(maxLong + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    // Values for the lowest long int
    preface.assign("| The value of the lowest long int:          |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << minLong << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << minLong << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << minLong << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << minLongCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<long>(minLong - 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;
    
    // Values for the highest unsigned long int 
    preface.assign("| The value of the highest unsigned long int:        |");
    prefLen = preface.size();
    cout << setfill('-') << setw(prefLen) << "-" << endl;
    cout << setfill(' ') << preface << endl; 
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| In decimal: " << setw(prefLen - 24) << right << dec << maxLongUnsgn << " |" << endl;
    cout << setw(22) << left << "| In hexadecimal: " << setw(prefLen - 24) << right << hex << maxLongUnsgn << " |" << endl;
    cout << setw(22) << left << "| In octal: " << setw(prefLen - 24) << right << oct << maxLongUnsgn << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Calculated decimal: " << setw(prefLen - 24) << right << dec << umaxLongCalc << " |" << endl;
    cout << setw(prefLen - 1) << left << "|" << right << "|" << endl;
    cout << setw(22) << left << "| Overflows to: " << setw(prefLen - 24) << right << dec << static_cast<unsigned long>(maxLongUnsgn + 1) << " |" << endl; 
    cout << setfill('-') << setw(prefLen) << "-" << endl << endl;

    
    // return 0 and exit
    return 0;
}



