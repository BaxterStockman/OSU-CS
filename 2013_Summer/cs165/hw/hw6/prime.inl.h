// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/19/2013 10:18:18 AM
//  Last Modification Date:     08/25/2013 10:54:24 PM
//  Filename:                   rational.inl.h
//
//  Overview:
//
//      Contains the definitions of the constructors and member functions of class
//      prime, as well as associated operators and non-member functions.
//
// =====================================================================================

#ifndef PRIME_INL_H
#define PRIME_INL_H

#include<iostream>
#include<stdexcept>
#include<exception>

namespace schreibm
{


    //-----------------------------------------------------------------------------
    //  Constructors
    //-----------------------------------------------------------------------------

    prime::prime(unsigned int num)
    {
        catch_not_prime(num);    // Ensure that number is prime
        this->val = num;
    }

    prime::prime()
    {
        this->val = 2;
    }


    //-----------------------------------------------------------------------------
    //  Getter and setter functions
    //-----------------------------------------------------------------------------

    unsigned int prime::get() const
    {
        return this->val;
    }

    unsigned int prime::get(const prime& num) const
    {
        return num.val;
    }

    void prime::set(const unsigned int num)
    {
        this->val = num;
    }


    //-----------------------------------------------------------------------------
    //  Member operator definitions
    //-----------------------------------------------------------------------------

    prime& prime::operator=(const prime &rhs)
    {
        // Prevent self-assignment by checking address of 'this' against
        // address of 'rhs'
        if(this == &rhs)
        {
            std::bad_alloc mem_except;
            throw mem_except;
        }

        this->set(rhs.get());
        return *this;
    }

    // calls the above-defined operator=
    prime& prime::operator=(const unsigned int &rhs)
    {
        *this = prime(rhs);
        return *this;
    }

    prime& prime::operator++()
    {
        unsigned int curr = this->get() + 1;

        while(!(check_prime(curr)))
            ++curr;

        this->set(curr);
        return *this;
    }

    // calls the above-defined operator++
    prime prime::operator++(int ignored)
    {
        prime returned = *this;

        ++(*this);

        return returned;
    }

    prime& prime::operator--()
    {
        unsigned int curr = this->get() - 1;

        // Since there are no prime numbers below 1, throw exception
        // if there is an attempt to decrement beneath 1.
        if(curr < 1)
        {
            throw std::out_of_range("attempt to assign non-prime value");
        }

        while(!(check_prime(curr)))
            --curr;

        this->set(curr);

        return *this;
    }

    // calls the above-defined operator--
    prime prime::operator--(int ignored)
    {
        prime returned = *this;

        --(*this);

        return returned;
    }


    //-----------------------------------------------------------------------------
    //  Input and output stream operators
    //-----------------------------------------------------------------------------

    std::ostream& operator<<(std::ostream &out, const prime& rhs)
    {
        out << rhs.get();
        return out;
    }

    std::istream& operator>>(std::istream &in, prime& rhs)
    {
        unsigned int temp;
        in >> temp;
        rhs.set(temp);
        return in;
    }


    //-----------------------------------------------------------------------------
    //  Miscellaneous member functions
    //-----------------------------------------------------------------------------

    // this member function is meant for throwing exceptions when there is an
    // attempt to assign a non-prime number to a prime object
    void prime::catch_not_prime(const unsigned int& num)
    {
        // no primes below 1
        if(!(check_prime(num)));
            throw std::invalid_argument("attempt to assign non-prime value");
    }


    //-----------------------------------------------------------------------------
    //  Miscellaneous non-member functions
    //-----------------------------------------------------------------------------

    // Checks that num is a prime number
    bool check_prime(const unsigned int& num)
    {
        if(num <= 1)
            return false;

        if(num == 2)
            return true;

        for(unsigned int div = 2; div < num; div++)
        {
            if(num % div == 0)
                return false;
        }

        return true;
    }
}

#endif
