// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/20/2013 12:01:44 PM
//  Last Modification Date:
//  Filename:                   rational_test.cpp
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
#include<cstdlib>       // for rand()
#include<sstream>
#include<string>
#include "rational.h"

using namespace schreibm;

int rand_num();

rational<int>::rational get_rat(std::string prompt);

int main()
{
    srand(time(NULL));

    rational<int> ratOne(rand_num(), rand_num());
    rational<int> ratTwo(rand_num(), rand_num());
    rational<int> ratThree;

    int wholeNum = rand_num();
    int insNum1 = rand_num();
    int insNum2 = rand_num();

    std::stringstream ss;

    std::cout << "\nWelcome to the rational number manipulator!"
              << "\nThis is where rational numbers get put through their paces.\n" << std::endl;

    std::cout << "Here are two rational numbers, " << ratOne << " and " << ratTwo << "."
              << "\nHere's how they combine:\n" << std::endl;

    ratThree = ratOne + ratTwo;
    std::cout << ratOne << " + " << ratTwo << " = " << ratThree << std::endl;

    ratThree = ratOne - ratTwo;
    std::cout << ratOne << " - " << ratTwo << " = " << ratThree << std::endl;

    ratThree = ratOne * ratTwo;
    std::cout << ratOne << " * " << ratTwo << " = " << ratThree << std::endl;

    ratThree = ratOne / ratTwo;
    std::cout << ratOne << " / " << ratTwo << " = " << ratThree << std::endl;


    std::cout << "\nThis also works for whole numbers:" << std::endl;

    ratThree = ratOne + wholeNum;
    std::cout << ratOne << " + " << wholeNum << " = " << ratThree << std::endl;

    ratThree = ratOne - wholeNum;
    std::cout << ratOne << " - " << wholeNum << " = " << ratThree << std::endl;

    ratThree = ratOne * wholeNum;
    std::cout << ratOne << " * " << wholeNum << " = " << ratThree << std::endl;

    ratThree = ratOne / wholeNum;
    std::cout << ratOne << " / " << wholeNum << " = " << ratThree << std::endl;


    std::cout << "\nEven when they're on the left side:" << std::endl;

    ratThree = wholeNum + ratOne;
    std::cout << wholeNum << " + " << ratOne << " = " << ratThree << std::endl;

    ratThree = wholeNum - ratOne;
    std::cout << wholeNum << " - " << ratOne << " = " << ratThree << std::endl;

    ratThree = wholeNum * ratOne;
    std::cout << wholeNum << " * " << ratOne << " = " << ratThree << std::endl;

    ratThree = wholeNum / ratOne;
    std::cout << wholeNum << " / " << ratOne << " = " << ratThree << std::endl;


    std::cout << "\nHere's how " << ratOne << " and " << ratTwo << " compare:" << std::endl;

    if(ratOne == ratTwo)
        std::cout << ratOne << " is equal to " << ratTwo << std::endl;

    if(ratOne != ratTwo)
        std::cout << ratOne << " is not equal to " << ratTwo << std::endl;

    if(ratOne < ratTwo)
        std::cout << ratOne << " is less than " << ratTwo << std::endl;

    if(ratOne <= ratTwo)
        std::cout << ratOne << " is less than or equal to " << ratTwo << std::endl;

    if(ratOne > ratTwo)
        std::cout << ratOne << " is greater than " << ratTwo << std::endl;

    if(ratOne >= ratTwo)
        std::cout << ratOne << " is greater than or equal to " << ratTwo << std::endl;


    std::cout << "\nThe stream insertion and exctraction operators work, too:" << std::endl;
    ss << insNum1 << "/" << insNum2;
    std::cout << ss.str() << " >> a rational number: ";
    ss >> ratThree;
    std::cout << ratThree << '\n' << std::endl;

    return 0;
}

int rand_num()
{
    return rand() % 20 + 1;
}


