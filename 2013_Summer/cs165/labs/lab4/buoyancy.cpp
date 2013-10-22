// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/13/2013 03:50:18 PM        
//  Last Modification Date:
//  Filename:                   buoyancy.cpp    
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

#include<iostream>                                  // error: iostream misspelled
#include<cmath>

//Defines the specific weight of water
#define WATER_WEIGHT 62.4                           // error: no semicolon needed 
#define PI 3.14159

using std::cout;
using std::cin;

int main() {                                        // error: no '()'
    //Variables needed to calculate buoyancy
    float radius; 
    float bforce; 
    float volume;                   // bad form to declare > 1 variable per line
    float weight;

    //Prompt user for weight and radius of sphere
    cout << "Please enter the weight (lbs): ";      // error: no terminating semicolon
    cin >> weight;                                  // error: weight no declared

    cout << "Please enter the radius: ";            // error: << and >> reversed 
    cin >> radius;

    // Calculate the volume and buoyancy force
    volume = 4./3. * PI * pow(radius, 3);           // error: no such function as power(); 'r' undefined
    bforce = volume * WATER_WEIGHT;                 // error: various misspellings

    //Determine if the sphere will sink or float
    if(bforce >= weight) {                          // error: no such operator as '>=='
        cout << "The sphere will float!\n";         // error: missing closing brace; operator<< reversed
    }
    else {                                          // error: anomalous semicolon after 'else'; no opening brace
        cout << "The sphere will sink!\n";
    }                                               // error: missing closing brace
    return 0;
}
//return;                                           // error: return outside of scope of 
                                                    // scope of main(); no value specified
