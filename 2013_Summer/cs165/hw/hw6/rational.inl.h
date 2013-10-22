// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/19/2013 07:22:10 PM
//  Last Modification Date:     08/25/2013 10:55:10 PM
//  Filename:                   rational.cpp
//
//  Overview:
//      Library file containing the definitions of constructors, operators, member
//      functions, and associated non-member functions of class rational.
//
// =====================================================================================

#ifndef RATIONAL_INL_H
#define RATIONAL_INL_H

#include<iostream>
#include<string>
#include<sstream>
#include<type_traits>
#include<stdexcept>
#include<exception>

namespace schreibm
{


    //-----------------------------------------------------------------------------
    //  Constructors
    //-----------------------------------------------------------------------------

    template<typename T>
    rational<T>::rational(T numerator, T denominator)
    {
        check_integral(numerator);
        check_integral(denominator);

        this->numer = numerator;
        this->denom = denominator;

        this->normalize();
    }

    template<typename T>
    rational<T>::rational()
    {
        this->numer = 0;
        this->denom = 1;
    }

    // Copy constructor
    template<typename T>
    rational<T>::rational(const rational<T> &src)
    {
        this->numer = src.numer;
        this->denom = src.denom;
    }


    //-----------------------------------------------------------------------------
    //  Getter and setter functions
    //-----------------------------------------------------------------------------

    template<typename T>
    T rational<T>::get_numer() const
    {
        return this->numer;
    }

    template<typename T>
    T rational<T>::get_denom() const
    {
        return this->denom;
    }

    template<typename T>
    rational<T>& rational<T>::set_numer(const T num)
    {
        check_integral(num);

        this->numer = num;
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::set_denom(const T num)
    {
        check_integral(num);

        this->denom = num;
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::set_numer_and_denom(const T numerNum, const T denomNum)
    {
        check_integral(numerNum);
        check_integral(denomNum);

        this->numer = numerNum;
        this->denom = denomNum;
        this->normalize();
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Misc. member functions
    //-----------------------------------------------------------------------------

    template<typename T>
    rational<T>& rational<T>::normalize()
    {
        T numerator = this->numer;
        T denominator = this->denom;

        if(numerator < 0 && denominator < 0)
        {
            numerator *= -1;
            denominator *=-1;
        }

        if(numerator >= 0 && denominator < 0)
        {
            numerator *= -1;
            denominator *=-1;
        }

        for(T div = 2; div <= numerator; div++)
        {
            if(numerator != 1 && numerator % div == 0 && denominator % div == 0)
            {
                numerator /= div;
                denominator /= div;
                div--;
            }
        }

        this->numer = numerator;
        this->denom = denominator;
        return *this;
    }

    template<typename T>
    void rational<T>::check_integral(T input)
    {
        if(std::is_same<T,char>::value || !(std::is_integral<T>::value))
            throw std::invalid_argument("attempt to assign non-integral value");
    }


    //-----------------------------------------------------------------------------
    //  Member operators
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Unary assignment operators
    //-----------------------------------------------------------------------------

    template<typename T>
    rational<T>& rational<T>::operator=(const rational<T> &rhs)
    {
        this-> set_numer(rhs.get_numer());
        this-> set_denom(rhs.get_denom());
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator=(const T &rhs)
    {
        *this = rational<T>(rhs);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Binary assignment operators
    //-----------------------------------------------------------------------------

    template<typename T>
    rational<T>& rational<T>::operator+=(const rational<T>& rhs)
    {
        this->set_numer((this->get_numer() * rhs.get_denom()) + (this->get_denom() * rhs.get_numer()));
        this->set_denom(this->get_denom() * rhs.get_denom());
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator+=(const T& rhs)
    {
        *this += rational<T>(rhs);
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator-=(const rational<T>& rhs)
    {
        this->set_numer((this->get_numer() * rhs.get_denom()) - (this->get_denom() * rhs.get_numer()));
        this->set_denom(this->get_denom() * rhs.get_denom());
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator-=(const T& rhs)
    {
        *this -= rational<T>(rhs);
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator*=(const rational<T>& rhs)
    {
        this->set_numer(this->get_numer() * rhs.get_numer());
        this->set_denom(this->get_denom() * rhs.get_denom());
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator*=(const T& rhs)
    {
        *this *= rational<T>(rhs);
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator/=(const rational<T>& rhs)
    {
        this->set_numer(this->get_numer() * rhs.get_denom());
        this->set_denom(this->get_denom() * rhs.get_numer());
        this->normalize();
        return *this;
    }

    template<typename T>
    rational<T>& rational<T>::operator/=(const T& rhs)
    {
        *this /= rational<T>(rhs);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Non-member operators
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Arithmetic operators
    //-----------------------------------------------------------------------------

    template<typename T>
    rational<T> operator+(const rational<T> &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) += rhs;
    }

    template<typename T>
    rational<T> operator+(const rational<T> &lhs, const T &rhs)
    {
        return rational<T>(lhs) += rhs;
    }

    template<typename T>
    rational<T> operator+(const T &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) += rhs;
    }

    template<typename T>
    rational<T> operator-(const rational<T> &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) -= rhs;
    }

    template<typename T>
    rational<T> operator-(const rational<T> &lhs, const T &rhs)
    {
        return rational<T>(lhs) -= rhs;
    }

    template<typename T>
    rational<T> operator-(const T &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) -= rhs;
    }

    template<typename T>
    rational<T> operator*(const rational<T> &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) *= rhs;
    }

    template<typename T>
    rational<T> operator*(const rational<T> &lhs, const T &rhs)
    {
        return rational<T>(lhs) *= rhs;
    }

    template<typename T>
    rational<T> operator*(const T &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) *= rhs;
    }

    template<typename T>
    rational<T> operator/(const rational<T> &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) /= rhs;
    }

    template<typename T>
    rational<T> operator/(const rational<T> &lhs, const T &rhs)
    {
        return rational<T>(lhs) /= rhs;
    }

    template<typename T>
    rational<T> operator/(const T &lhs, const rational<T> &rhs)
    {
        return rational<T>(lhs) /= rhs;
    }

    //-----------------------------------------------------------------------------
    //  Boolean operators
    //-----------------------------------------------------------------------------

    template<typename T>
    bool operator==(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(lhs.get_numer() * rhs.get_denom() == rhs.get_numer() * lhs.get_denom())
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator==(const rational<T> &lhs, const T &rhs)
    {
        if(lhs == rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator==(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) == rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator!=(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(!(lhs == rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator!=(const rational<T> &lhs, const T &rhs)
    {
        if(lhs == rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator!=(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) == rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(lhs.get_numer() * rhs.get_denom() < rhs.get_numer() * lhs.get_denom())
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<(const rational<T> &lhs, const T &rhs)
    {
        if(lhs < rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) < rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<=(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(lhs < rhs || lhs == rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<=(const rational<T> &lhs, const T &rhs)
    {
        if(lhs <= rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator<=(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) <= rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(!(lhs <= rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>(const rational<T> &lhs, const T &rhs)
    {
        if(lhs > rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) > rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>=(const rational<T> &lhs, const rational<T> &rhs)
    {
        if(lhs > rhs || lhs == rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>=(const rational<T> &lhs, const T &rhs)
    {
        if(lhs >= rational<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator>=(const T &lhs, const rational<T> &rhs)
    {
        if(rational<T>(lhs) >= rhs)
            return true;
        else
            return false;
    }
}

#endif
