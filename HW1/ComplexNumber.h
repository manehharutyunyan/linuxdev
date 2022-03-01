// the class of complex numbers
class ComplexNumber
{
    private:
        // the real part
        double real;

        // the imaginary part
        double imag;

    public:
        // default constructor
        ComplexNumber();

        // constructor with two arguments
        ComplexNumber(double r, double i);

        // get real part
        double GetRealPart() const;

        // get imaginary part
        double GetImaginaryPart() const;

        // the absolute value of a complex number
        double AbsoluteValue() const;

        // print the complex number
        void PrintComplexNumber() const;

        // operator + overload
        friend ComplexNumber operator+(const ComplexNumber& c1, const ComplexNumber& c2);

        // operator - overload
        friend ComplexNumber operator-(const ComplexNumber& c1, const ComplexNumber& c2);

        // operator * overload
        friend ComplexNumber operator*(const ComplexNumber& c, double s);
};