#include <iostream>
#include "ComplexNumber.h"
#include <cmath>

using namespace std;

// the default constructor
ComplexNumber::ComplexNumber()
{
    imag = 0;
    real = 0;
}

// the constructor with 2 arguments
ComplexNumber::ComplexNumber(double r, double i)
{
    imag = i;
    real = r;
}

// gets the real part of the complex number
double ComplexNumber::GetRealPart() const
{
    return this->real;
}

// gets the imaginary part of the complex number
double ComplexNumber::GetImaginaryPart() const
{
    return this->imag;
}

// the absolute value of a complex number
double ComplexNumber::AbsoluteValue() const
{
    return sqrt(this->imag*this->imag + this->real*this->real);
}

// operator + overload
ComplexNumber operator+(const ComplexNumber& c1, const ComplexNumber& c2)
{ 
    return ComplexNumber(c1.real + c2.real, c1.imag + c2.imag);
}

// operator - overload
ComplexNumber operator-(const ComplexNumber& c1, const ComplexNumber& c2)
{ 
    return ComplexNumber(c1.real - c2.real, c1.imag - c2.imag);
}
 // operator * overload
ComplexNumber operator*(const ComplexNumber& c, double s)
{ 
    return ComplexNumber(c.real * s, c.imag * s);
}

void ComplexNumber::PrintComplexNumber() const
{
    cout << "real part " << this->real << " \n";
    cout << "imag part " << this->imag << "i \n\n";
}