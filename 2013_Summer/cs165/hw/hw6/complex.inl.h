// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/22/2013 12:20:07 PM
//  Last Modification Date:     08/25/2013 10:51:42 PM
//  Filename:                   complex.hxx
//
//  Overview:
//      Header files containing the definitions of member functions, operators, and
//      related non-member functions of the complex class.
//
// =====================================================================================

#ifndef COMPLEX_INL_H
#define COMPLEX_INL_H

#include<iostream>
#include<type_traits>
#include<assert.h>
#include<ctgmath>

#define PI 3.141592654

namespace schreibm
{

    //-----------------------------------------------------------------------------
    //  Constructors
    //-----------------------------------------------------------------------------

    template<typename T>
    complex<T>::complex(T inRealPart, T inImagPart)
    {
        assert_is_arithmetic(inRealPart);
        assert_is_arithmetic(inRealPart);

        this->realPart = inRealPart;
        this->imagPart = inImagPart;
    }

    template<typename T>
    complex<T>::complex()
    {
        this->realPart = 0;
        this->imagPart = 0;
    }

    // Copy constructor
    template<typename T>
    complex<T>::complex(const complex<T>& srcComplex)
    {
        this->realPart = srcComplex.realPart;
        this->imagPart = srcComplex.imagPart;
    }


    //-----------------------------------------------------------------------------
    //  Getter and setter functions
    //-----------------------------------------------------------------------------

    template<typename T>
    T complex<T>::real() const
    {
        return this->realPart;
    }

    template<typename T>
    T complex<T>::imag() const
    {
        return this->imagPart;
    }

    template<typename T>
    complex<T>& complex<T>::set_real(T realNum)
    {
        this->realPart = realNum;
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::set_imag(T imagNum)
    {
        this->imagPart = imagNum;
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Member operators
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Unary assignment operators
    //-----------------------------------------------------------------------------

    template<typename T>
    complex<T>& complex<T>::operator=(const complex<T>& rhs)
    {
        this->set_real(rhs.real());
        this->set_imag(rhs.imag());
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator=(const T& rhs)
    {
        *this = complex<T>(rhs);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Binary assignment operators
    //-----------------------------------------------------------------------------

    template<typename T>
    complex<T>& complex<T>::operator+=(const complex<T>& rhs)
    {
        this->set_real(this->real() + rhs.real());
        this->set_imag(this->imag() + rhs.imag());
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator+=(const T& rhs)
    {
        *this += complex<T>(rhs);
        return this;
    }

    template<typename T>
    complex<T>& complex<T>::operator-=(const complex<T>& rhs)
    {
        this->set_real(this->real() - rhs.real());
        this->set_imag(this->imag() - rhs.imag());
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator-=(const T& rhs)
    {
        *this -= complex<T>(rhs);
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator*=(const complex<T>& rhs)
    {
        const T newReal = (this->real() * rhs.real()) - (this->imag() * rhs.imag());
        const T newImag = (this->imag() * rhs.real()) + (this->real() * rhs.imag());
        this->set_real(newReal);
        this->set_imag(newImag);
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator*=(const T& rhs)
    {
        *this *= complex<T>(rhs);
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator/=(const complex<T>& rhs)
    {
        const T lhsReal = this->real();
        const T rhsReal = rhs.real();
        const T lhsImag = this->imag();
        const T rhsImag = rhs.imag();

        this->set_real((((lhsReal * rhsReal) + (lhsImag * rhsImag))/(pow(rhsReal, 2) + pow(rhsImag, 2))));
        this->set_imag((((lhsImag * rhsReal) - (lhsReal * rhsImag))/(pow(rhsReal, 2) + pow(rhsImag, 2))));
        return *this;
    }

    template<typename T>
    complex<T>& complex<T>::operator/=(const T& rhs)
    {
        *this /= complex<T>(rhs);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //  Unary arithmetic operators
    //-----------------------------------------------------------------------------

    template<typename T>
    complex<T> operator-(const complex<T>& argNum)
    {
        return complex<T>((argNum.real() * - 1), (argNum.imag() * - 1));
    }


    template<typename T>
    complex<T> operator+(const complex<T>& rhs)
    {
        return rhs;
    }


    //-----------------------------------------------------------------------------
    //  Non-member operators
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Binary arithmetic operators
    //-----------------------------------------------------------------------------

    template<typename T>
    complex<T> operator+(const complex<T>&lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) += rhs;
    }
    template<typename T>
    complex<T> operator+(const complex<T>& lhs, const T& rhs)
    {
        return lhs += complex<T>(rhs);
    }

    template<typename T>
    complex<T> operator+(const T& lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) += rhs;
    }

    template<typename T>
    complex<T> operator-(const complex<T>&lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) -= rhs;
    }

    template<typename T>
    complex<T> operator-(const complex<T>& lhs, const T& rhs)
    {
        return complex<T>(lhs) -= complex<T>(rhs);
    }

    template<typename T>
    complex<T> operator-(const T& lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) -= complex<T>(rhs);
    }

    template<typename T>
    complex<T> operator*(const complex<T>&lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) *= rhs;
    }

    template<typename T>
    complex<T> operator*(const complex<T>& lhs, const T& rhs)
    {
        return lhs * complex<T>(rhs);
    }

    template<typename T>
    complex<T> operator*(const T& lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) * rhs;
    }

    template<typename T>
    complex<T> operator/(const complex<T>&lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) /= rhs;
    }

    template<typename T>
    complex<T> operator/(const complex<T>& lhs, const T& rhs)
    {
        return complex<T>(lhs) /= complex<T>(rhs);
    }

    template<typename T>
    complex<T> operator/(const T& lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) /= complex<T>(rhs);
    }


    //-----------------------------------------------------------------------------
    //  Boolean operators
    //-----------------------------------------------------------------------------

    template<typename T>
    bool operator==(const complex<T>& lhs, const complex<T>& rhs)
    {
        if(lhs.real() == rhs.real() && lhs.imag() == rhs.imag())
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator==(const complex<T>& lhs, const T& rhs)
    {
        if(lhs == complex<T>(rhs))
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator==(const T& lhs, const complex<T>& rhs)
    {
        if(complex<T>(rhs) == rhs)
            return true;
        else
            return false;
    }

    template<typename T>
    bool operator!=(const complex<T>& lhs, const complex<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T>
    bool operator!=(const complex<T>& lhs, const T& rhs)
    {
        return lhs != complex<T>(rhs);
    }

    template<typename T>
    bool operator!=(const T& lhs, const complex<T>& rhs)
    {
        return complex<T>(lhs) != rhs;
    }


    //-----------------------------------------------------------------------------
    //  Functions on complex
    //-----------------------------------------------------------------------------

    template<typename T>
    T real(const complex<T>& argNum)
    {
        return argNum.real();
    }

    template<typename T>
    double real(T argNum)
    {
        return real(complex<double>(static_cast<double>(argNum)));
    }

    template<typename T>
    T imag(const complex<T>& argNum)
    {
        return argNum.imag();
    }

    template<typename T>
    double imag(T argNum)
    {
        return imag(complex<double>(static_cast<double>(argNum)));
    }

    template<typename T>
    T abs(const complex<T>& argNum)
    {
        return std::sqrt(std::pow(argNum.real(), 2) + std::pow(argNum.imag(), 2));
    }

    template<typename T>
    T arg(const complex<T>& argNum)
    {
        const T realPart = argNum.real();
        const T imagPart = argNum.imag();

        if(realPart > 0)
            return std::atan(imagPart / realPart);
        else if(realPart < 0 && imagPart >= 0)
            return std::atan(imagPart / realPart) + PI;
        else if(realPart < 0 && imagPart < 0)
            return std::atan(imagPart / realPart) - PI;
        else if(realPart == 0 && imagPart > 0)
            return PI / 2;
        else if(realPart == 0 && imagPart < 0)
            return (PI / 2) * -1;
        else if(realPart == 0 && imagPart == 0)
            return NULL;
        else
            return NULL;
    }

    template<typename T>
    double arg(T argNum)
    {
        return arg(complex<double>(static_cast<double>(argNum)));
    }

    template<typename T>
    T norm(const complex<T>& argNum)
    {
        return std::pow(abs(argNum));
    }

    template<typename T>
    double norm(T argNum)
    {
        return norm(complex<double>(static_cast<double>(argNum)));
    }

    template<typename T>
    complex<T> conj(const complex<T>& argNum)
    {
        return complex<T>(argNum.real(), (argNum.imag() * -1));
    }

    template<typename T>
    complex<double> conj(T argNum)
    {
        return conj(complex<double>(static_cast<double>(argNum)));
    }

    template<typename T>
    complex<T>polar(const T& rho, const T& theta)
    {
        return complex<T>((rho * cos(theta)), (rho * sin(theta)));
    }

    // Misc. functions
    template<typename T>
    void assert_is_arithmetic(const T&)
    {
        static_assert(std::is_arithmetic<T>::value, "argument data type must be arithmetic");
    }
}

#endif
