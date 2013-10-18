// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/13/2013 06:17:53 PM        
//  Last Modification Date:
//  Filename:                   primes.cpp    
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
#define PROMPT "Please enter a whole number:"
#define NOT_PRIME "The number is not a prime number./n"
#define PRIME "The number is a prime number./n"
#define DONE 0         /*  ends successful program */
#define FIRST_FACTOR 3 /*  initial value in for loop */

using std::cout;
using std::cin;
using std::endl;

int main(){
    int i; /*  loop counter */
    int number; /*  number provided by user */
    bool isPrime;
    bool upToHalf;

    cout << "Please enter a number to check for primeness: "; /*  promt user */
    cin >> number; /*  wait for user input */
    
    /*  Prime numbers are defined as any number
     *  * greater than one that is only divisible
     *  * by one and itself. Dividing the number
     *  * by two shortens the time it takes to
     *  * complete. */
    
    for(i = FIRST_FACTOR; i <= number/2; ++i) {
        //  cout << "The loop index is: " << i << endl;

        // cout << number << " modulus " << i << " is " << number%i << endl;
       
        // upToHalf = (i <= number/2);
        // cout << "If the following digit is '1', the loop index " << i << " is less than or equal to one half of " << number << ": " << upToHalf << endl;
 
        // isPrime = (number%i == 0);
        // cout << "If the following digit is '1', " << number << " is not prime: " << isPrime << endl;
 
        if( number%i == 0 ){ /*  if divisible */
            cout << "This is not a prime number: " << number << endl; /*  not prime */
            return DONE; /*  exit program */
        }
    }

    /*  if number is not divisible by anything
     *  * than it must be prime */
    
    cout << "This is a prime number: " << number << endl;
    return DONE; /*  exit program */ 
} 
