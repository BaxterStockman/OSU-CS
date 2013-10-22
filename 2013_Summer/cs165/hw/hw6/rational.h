// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/19/2013 07:22:10 PM
//  Last Modification Date:     08/25/2013 10:54:54 PM
//  Filename:                   rational.h
//
//  Overview:
//      This is a header file containing the declaration of the rational class,
//      as well as declarations of member functions, constructors, operators, and
//      associated non-member functions.  It also contains definitions of the
//      stream insertion and extraction operators.
//
// =====================================================================================

#ifndef RATIONAL_H
#define RATIONAL_H

#include<stdexcept>
#include<exception>

namespace schreibm
{

    template<typename T> class rational;


    // Boolean operators
    // Arithmetic operators
    template<typename T> rational<T> operator+(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator+(const rational<T> &lhs, const T &rhs);
    template<typename T> rational<T> operator+(const T &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator-(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator-(const rational<T> &lhs, const T &rhs);
    template<typename T> rational<T> operator-(const T &lhs, const T &rhs);
    template<typename T> rational<T> operator*(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator*(const rational<T> &lhs, const T &rhs);
    template<typename T> rational<T> operator*(const T &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator/(const rational<T> &lhs, const T &rhs);
    template<typename T> rational<T> operator/(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> rational<T> operator/(const T &lhs, const T &rhs);

    // Boolean operators
    template<typename T> bool operator==(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator==(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator==(const T &lhs, const rational<T> &rhs);
    template<typename T> bool operator!=(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator!=(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator!=(const T &lhs, const rational<T> &rhs);
    template<typename T> bool operator<(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator<(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator<(const T &lhs, const rational<T> &rhs);
    template<typename T> bool operator<=(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator<=(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator<=(const T &lhs, const rational<T> &rhs);
    template<typename T> bool operator>(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator>(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator>(const T &lhs, const rational<T> &rhs);
    template<typename T> bool operator>=(const rational<T> &lhs, const rational<T> &rhs);
    template<typename T> bool operator>=(const rational<T> &lhs, const T &rhs);
    template<typename T> bool operator>=(const T &lhs, const rational<T> &rhs);

    template<typename T>
    class rational
    {

        public:

            // Constructors
            rational(T numerator, T denominator = 1);
            rational();

            // Copy constructor
            rational(const rational &src);

            // Getter and setter functions
            T get_numer() const;
            T get_denom() const;
            rational& set_numer(const T num);
            rational& set_denom(const T num);
            rational& set_numer_and_denom(const T numerNum, const T denomNum);

            // Misc. member functions
            rational& normalize();
            static void check_integral(T input);

            // Unary assignment operators
            rational& operator=(const rational& rhs);
            rational& operator=(const T& rhs);

            // Binary assignment operators
            rational& operator+=(const rational& rhs);
            rational& operator+=(const T& rhs);
            rational& operator-=(const rational& rhs);
            rational& operator-=(const T& rhs);
            rational& operator*=(const rational& rhs);
            rational& operator*=(const T& rhs);
            rational& operator/=(const rational& rhs);
            rational& operator/=(const T& rhs);

            // Stream operators
            friend std::ostream& operator<<(std::ostream &out, const rational &rat)
            {
                if(rat.get_denom() == 1)
                    out << rat.get_numer();
                else
                    out << rat.get_numer() << "/" << rat.get_denom();
                return out;
            }

            friend std::istream& operator>>(std::istream &in, rational &rat)
            {
                std::string inputStr;
                std::size_t slashLoc;
                T numerator;
                T denominator;

                std::getline(in, inputStr);

                slashLoc = inputStr.find("/");

                if(inputStr.find_first_not_of("0123456789/") != std::string::npos
                        || slashLoc != inputStr.rfind("/"))
                {
                    throw std::invalid_argument("attempt to assign a non-rational value");
                }

                if(inputStr.find_first_not_of("0123456789/") == std::string::npos
                    && slashLoc == inputStr.rfind("/"))
                {
                    if(slashLoc != std::string::npos)
                    {
                        std::stringstream ssNumer(inputStr.substr(0, slashLoc));
                        std::stringstream ssDenom(inputStr.substr(slashLoc + 1, std::string::npos));
                        ssNumer >> numerator;
                        ssDenom >> denominator;
                    }
                    else
                    {
                        std::stringstream ssNumer(inputStr.substr(0, slashLoc));
                        ssNumer >> numerator;
                        denominator = 1;
                    }
                }

                rat.set_numer_and_denom(numerator, denominator);
                return in;
            }

        private:

            T numer;
            T denom;
    };
}

#include "rational.inl.h"

#endif
