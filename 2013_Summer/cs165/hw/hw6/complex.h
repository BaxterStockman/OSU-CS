// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/21/2013 03:52:15 PM
//  Last Modification Date:     08/25/2013 10:52:01 PM
//  Filename:                   complex.h
//
//  Overview:
//      Header file containing the declaration of the function class as well as member
//      functions, operators, and associated non-member functions
//
// =====================================================================================

#ifndef COMPLEX_H
#define COMPLEX_H

#include<sstream>

namespace  schreibm
{

    //-----------------------------------------------------------------------------
    //  Forward declarations
    //-----------------------------------------------------------------------------


    // =====================================================================================
    //        Class:    complex
    //  Description:    A class instantiating complex numbers, with associated functions and
    //                  operator definitions
    // =====================================================================================
    template<typename T> class complex;


    //-----------------------------------------------------------------------------
    //  Non-member functions
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Mathematical functions on complex
    //-----------------------------------------------------------------------------

    // Name:    real()
    // Purpose: return the real part of a complex number
    template<typename T> T real(const complex<T>& argNum);
    template<typename T> double real(T argNum);

    // Name:    imag()
    // Purpose: return the coefficient of the imaginary part of a complex number
    template<typename T> T imag(const complex<T>& argNum);
    template<typename T> double imag(T argNum);

    // Name:    abs()
    // Purpose: return the absolute value/modulus of a complex number
    template<typename T> T abs(const complex<T>& argNum);

    // Name:    arg()
    // Purpose: return the phase angle
    template<typename T> T arg(const complex<T>& argNum);
    template<typename T> double arg(T argNum);

    // Name:    norm()
    // Purpose: returns the norm of a complex number, i.e. its modulus squared
    template<typename T> T norm(const complex<T>& argNum);
    template<typename T> double norm(T argNum);

    // Name:    conj()
    // Purpose: returns the conjugate of a complex number, i.e. the number with the
    //          sign of the imaginary coefficient negated
    template<typename T> complex<T> conj(const complex<T>& argNum);
    template<typename T> complex<double> conj(T argNum);

    // Name:    polar()
    // Purpose: returns a complex object based on the input of polar coordinates
    // Entry:   rho is the modulus of the complex number; theta is its phase angle
    template<typename T> complex<T> polar(const T& rho, const T& theta);

    // Misc. functions

    // Name:    assert_is_arithmetic()
    // Purpose: checks to make sure that argument is an arithmetic data type
    template<typename T> void assert_is_arithmetic(const T& argNum);


    //-----------------------------------------------------------------------------
    //  Arithmetic operators
    //-----------------------------------------------------------------------------

    // Unary arithmetic operators
    template<typename T> complex<T> operator+(const complex<T>& rhs);
    template<typename T> complex<T> operator-(const complex<T>& rhs);

    // Binary arithmetic operators
    template<typename T> complex<T> operator+(const complex<T>& lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator+(const complex<T>& lhs, const T& rhs);
    template<typename T> complex<T> operator+(const T& lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator-(const complex<T>&lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator-(const complex<T>& lhs, const T& rhs);
    template<typename T> complex<T> operator-(const T& lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator*(const complex<T>&lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator*(const complex<T>& lhs, const T& rhs);
    template<typename T> complex<T> operator*(const T& lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator/(const complex<T>&lhs, const complex<T>& rhs);
    template<typename T> complex<T> operator/(const complex<T>& lhs, const T& rhs);
    template<typename T> complex<T> operator/(const T& lhs, const complex<T>& rhs);


    //-----------------------------------------------------------------------------
    //  Boolean operators
    //-----------------------------------------------------------------------------

    template<typename T> bool operator==(const complex<T>& lhs, const complex<T>& rhs);
    template<typename T> bool operator==(const complex<T>& lhs, const T& rhs);
    template<typename T> bool operator==(const T& lhs, const complex<T>& rhs);
    template<typename T> bool operator!=(const complex<T>& lhs, const complex<T>& rhs);
    template<typename T> bool operator!=(const complex<T>& lhs, const T& rhs);
    template<typename T> bool operator!=(const T& lhs, const complex<T>& rhs);


    template<typename T>
    class complex
    {
        public:

            // Constructors
            complex(T realPart, T imagPart = 0);
            complex();

            // Copy constructor
            complex(const complex &srcComplex);

            // Getter and setter functions
            T real() const;
            T imag() const;
            complex& set_real(T realNum);
            complex& set_imag(T imagNum);

            // Unary assignment operators
            complex& operator=(const complex& rhs);
            complex& operator=(const T& rhs);

            // Binary assignment operators
            complex& operator+=(const complex& rhs);
            complex& operator+=(const T& rhs);
            complex& operator-=(const complex& rhs);
            complex& operator-=(const T& rhs);
            complex& operator*=(const complex& rhs);
            complex& operator*=(const T& rhs);
            complex& operator/=(const complex& rhs);
            complex& operator/=(const T& rhs);

            // Stream operators
            friend std::ostream& operator<<(std::ostream& out, const complex& outComplex)
            {
                out << "(" << outComplex.real() << "," << outComplex.imag() << ")";

                return out;
            }

            friend std::istream& operator>>(std::istream& in, complex& inComplex)
            {
                std::string inputStr;
                std::size_t parenOne;
                std::size_t parenTwo;
                std::size_t commaLoc;
                T inReal;
                T inImag;

                std::getline(in, inputStr);

                parenOne = inputStr.find("(");
                parenTwo = inputStr.rfind(")");
                commaLoc = inputStr.find(",");

                // make sure that input contains only certain characters, that the input
                // begins and ends with parenthesis, and that there's only one comma, if any
                if(inputStr.find_first_not_of("0123456789(),") == std::string::npos
                    && parenOne == 0 && parenTwo == inputStr.length() - 1
                    && commaLoc == inputStr.rfind(","))
                {
                    std::stringstream ssReal(inputStr.substr(parenOne + 1, commaLoc));
                    std::stringstream ssImag(inputStr.substr(commaLoc + 1, parenTwo));

                    ssReal >> inReal;
                    ssImag >> inImag;

                    inComplex.set_real(inReal);
                    inComplex.set_imag(inImag);
                }

                return in;
            }

        private:

            T realPart;
            T imagPart;
    };
}

#include "complex.inl.h"

#endif
