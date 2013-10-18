// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/22/2013 12:23:34 PM
//  Last Modification Date:     08/25/2013 10:53:45 PM
//  Filename:                   complex_test.cpp
//
//  Overview:
//      Compares the operators and functions from the standard library's implementation
//      of complex numbers to those from my implementation of complex numbers.
//
//  Input:
//      Requires no input.
//
//  Output:
//      Outputs descriptive messages to the console along with the results of various
//      operations on complex objects.
//
// =====================================================================================

#include<iostream>
#include<cstdlib>       // for rand()
#include<sstream>
#include<string>
#include<complex>
#include "complex.h"

using namespace schreibm;

double rand_double();

int main()
{
    srand(time(NULL));

    double randDb1 = rand_double();
    double randDb2 = rand_double();
    double randDb3 = rand_double();
    double randDb4 = rand_double();
    double randDb5 = rand_double();
    double randDb6 = rand_double();

    double randPolar1 = rand_double();
    double randPolar2 = rand_double();

    complex<double> myCmplx1(randDb1, randDb2);
    complex<double> myCmplx2(randDb3, randDb4);
    complex<double> myCmplx3;

    std::complex<double> stdCmplx1(randDb1, randDb2);
    std::complex<double> stdCmplx2(randDb3, randDb4);
    std::complex<double> stdCmplx3;

    std::stringstream ss;

    std::cout << '\n' << myCmplx1 << " == " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  ";
        if(stdCmplx1 == stdCmplx2)
            std::cout << stdCmplx1 << " equals " << stdCmplx2 << std::endl;
        else
            std::cout << stdCmplx1 << " does not equal " << stdCmplx2 << std::endl;
    std::cout << "My implementation: ";
        if(stdCmplx1 == stdCmplx2)
            std::cout << myCmplx1 << " equals " << myCmplx2 << '\n' << std::endl;
        else
            std::cout << myCmplx1 << " does not equal " << myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " != " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  ";
        if(stdCmplx1 != stdCmplx2)
            std::cout << stdCmplx1 << " does not equal " << stdCmplx2 << std::endl;
        else
            std::cout << stdCmplx1 << " equals " << stdCmplx2 << std::endl;
    std::cout << "My implementation: ";
        if(stdCmplx1 != stdCmplx2)
            std::cout << myCmplx1 << " does not equal " << myCmplx2 << '\n' << std::endl;
        else
            std::cout << myCmplx1 << " equals " << myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " - " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  " << stdCmplx1 - stdCmplx2 << std::endl;
    std::cout << "My implementation: " << myCmplx1 - myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " + " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  " << stdCmplx1 + stdCmplx2 << std::endl;
    std::cout << "My implementation: " << myCmplx1 + myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " - " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  " << stdCmplx1 - stdCmplx2 << std::endl;
    std::cout << "My implementation: " << myCmplx1 - myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " * " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  " << stdCmplx1 * stdCmplx2 << std::endl;
    std::cout << "My implementation: " << myCmplx1 * myCmplx2 << '\n' << std::endl;

    std::cout << myCmplx1 << " / " << myCmplx2 << " ---" << std::endl;
    std::cout << "Standard library:  " << stdCmplx1 / stdCmplx2 << std::endl;
    std::cout << "My implementation: " << myCmplx1 / myCmplx2 << '\n' << std::endl;

    std::cout << "real" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << std::real(stdCmplx1) << std::endl;
    std::cout << "My implementation: " << real(myCmplx1) << '\n' << std::endl;

    std::cout << "imag" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << std::imag(stdCmplx1) << std::endl;
    std::cout << "My implementation: " << imag(myCmplx1) << '\n' << std::endl;

    std::cout << "abs" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << std::abs(stdCmplx1) << std::endl;
    std::cout << "My implementation: " << abs(myCmplx1) << '\n' << std::endl;

    std::cout << "arg" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << std::arg(stdCmplx1) << std::endl;
    std::cout << "My implementation: " << arg(myCmplx1) << '\n' << std::endl;

    std::cout << "conj" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << std::conj(stdCmplx1) << std::endl;
    std::cout << "My implementation: " << conj(myCmplx1) << '\n' << std::endl;

    std::cout << "polar" << "(" << randPolar1 << "," << randPolar2 << ")" << " --- " << std::endl;
    std::cout << "Standard library:  " << std::polar(randPolar1, randPolar2) << std::endl;
    std::cout << "My implementation: " << polar(randPolar1, randPolar2) << '\n' << std::endl;

    std::cout << "polar" << "(" << 0 << "," << 0 << ")" << " --- " << std::endl;
    std::cout << "Standard library:  " << std::polar(0,0) << std::endl;
    std::cout << "My implementation: " << polar(0,0) << '\n' << std::endl;

    ss << "(" << randDb5 << "," << randDb6 << ")";
    std::cout << ss.str() << " >> complex<double> ---" << std::endl;
    ss >> stdCmplx3;
    ss.seekg(0);
    ss >> myCmplx3;
    std::cout << "Standard library:  " << stdCmplx3 << std::endl;
    std::cout << "My implementation: " << myCmplx3 << '\n' << std::endl;

    ss.str("");
    ss.clear();

    ss << "(" << randDb5 << ")";
    std::cout << ss.str() << " >> complex<double> ---" << std::endl;
    ss >> stdCmplx3;
    ss.seekg(0);
    ss >> myCmplx3;
    std::cout << "Standard library:  " << stdCmplx3 << std::endl;
    std::cout << "My implementation: " << myCmplx3 << '\n' << std::endl;

    std::cout << "-" << myCmplx1 << " --- " << std::endl;
    std::cout << "Standard library:  " << -stdCmplx1 << std::endl;
    std::cout << "My implementation: " << -myCmplx1 << '\n' << std::endl;

    return 0;
}

double rand_double()
{
    return static_cast<double>(rand() % 20);
}
