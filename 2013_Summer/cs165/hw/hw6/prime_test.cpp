// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/19/2013 02:00:42 PM
//  Last Modification Date:     08/25/2013 11:12:31 PM
//  Filename:                   prime_test.cpp
//
//  Overview:
//      A test program to check for proper functionality of class prime and
//      associated functions and operators.
//
//  Input:
//      No input is required.
//
//  Output:
//      Outputs to the console descriptive messages and the results of calling
//      functions and operators on prime objects.
//
// =====================================================================================

#include<iostream>
#include<sstream>
#include "prime.h"

int main()
{
    schreibm::prime num;

    std::cout << "The first ten prime numbers:" << std::endl;
    for(int i = 1; i <= 10; i++)
    {
        std::cout << num++ << " ";
    }

    std::cout << std::endl;

    std::cout << "Now counting down:" << std::endl;
    for(int i = 1; i <= 10; i++)
    {
        std::cout << --num << " ";
    }

    std::cout << std::endl;

    schreibm::prime firstPre, secondPre;

    std::cout << "Prime number = ++" << secondPre << std::endl;
    firstPre = ++secondPre;
    std::cout << "First number: " << firstPre << "; new value of number assigned: " << secondPre << std::endl;

    schreibm::prime firstPost, secondPost;

    std::cout << "Prime number = " << secondPost <<"++" << std::endl;
    firstPost = secondPost++;
    std::cout << "First number: " << firstPost << "; new value of number assigned: " << secondPost << std::endl;

    std::istringstream iss("7");
    schreibm::prime test;

    std::cout << "Streaming '7' into a prime variable..." << std::endl;
    iss >> test;
    std::cout << "Prime number = " << test << std::endl;

    return 0;
}
