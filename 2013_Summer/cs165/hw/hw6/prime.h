// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/19/2013 10:18:18 AM
//  Last Modification Date:     08/25/2013 10:54:08 PM
//  Filename:                   rational.cpp
//
//  Overview:
//
//      Header file for class prime; contains declaration of class, member functions,
//      operators, and associated non-member functions and operators.
//
//      Please see file prime.inl.h for definitions and documentation.
//
// =====================================================================================

#ifndef PRIME_H
#define PRIME_H

#include<iostream>

namespace schreibm
{

    bool check_prime(const unsigned int &num);

    class prime
    {
        public:

            // Constructors
            prime(unsigned int num);
            prime();

            // Getter and setter functions
            unsigned int get() const;
            unsigned int get(const prime& num) const;
            void set(const unsigned int num);

            // Member operators
            // Assignment
            prime& operator=(const prime &rhs);
            prime& operator=(const unsigned int &rhs);

            // Increment and decrement
            prime& operator++();
            prime operator++(int ignored);
            prime& operator--();
            prime operator--(int ignored);

            friend std::ostream& operator<<(std::ostream &out, const prime& rhs);
            friend std::istream& operator>>(std::istream &in, prime& rhs);

        private:

            unsigned int val;
            void catch_not_prime(const unsigned int &num);
    };

}

#include "prime.inl.h"

#endif
