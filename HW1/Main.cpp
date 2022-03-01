#include <iostream>
#include "ComplexNumber.h"
#include <vector>

using namespace std;

// insertion sort for complex numbers
void InsertionSort(vector<ComplexNumber> array)
{
    for(int k=0; k<array.size(); ++k)   
    {  
        ComplexNumber temp = array[k];  
        int j= k-1;  
        while(j>=0 && temp.AbsoluteValue() <= array[j].AbsoluteValue())  
        {  
            array[j+1] = array[j];   
            j = j-1;  
        }  
        array[j+1] = temp;  
    }  

    cout<<"\nSorted list of complex numbers by absolute value \n";
    for(int i=0;i<=array.size()-1;i++)  
    {  
        cout << "absolute value of " << i << "number \n";
        cout << array[i].AbsoluteValue() << "\n";
        array[i].PrintComplexNumber();  
    }
}

// start point for app
int main()
{
    // declare result
    ComplexNumber result;

    // init complex numbers
    ComplexNumber c1 = ComplexNumber(1,-1);
    ComplexNumber c2 = ComplexNumber(0,3);
    ComplexNumber c3 = ComplexNumber(1,-3);
    ComplexNumber c4 = ComplexNumber(7,3);
    ComplexNumber c5 = ComplexNumber(-10,3);
    ComplexNumber c6 = ComplexNumber(100,3);

    cout << "real part " << c1.GetRealPart()<< " \n";
    cout << "imag part " << c1.GetImaginaryPart() << "i \n";

    // test + operator
    result = c1 + c2;
    cout << "+ operator overload \n";
    result.PrintComplexNumber();

    // test - operator
    result = c1 - c2;
    cout << "- operator overload \n";
    result.PrintComplexNumber();

    // test * operator
    result = c1 * 5;
    cout << "* operator overload \n";
    result.PrintComplexNumber();

    // get absolute value
    cout << "absolute value \n";
    cout << c1.AbsoluteValue() << "\n";

    // create a vector of complex numbers
    vector<ComplexNumber> complexNumbers;
    complexNumbers.push_back(c1);
    complexNumbers.push_back(c2);
    complexNumbers.push_back(c3);
    complexNumbers.push_back(c4);
    complexNumbers.push_back(c5);
    complexNumbers.push_back(c6);

    // sort vector of complex numbers
    InsertionSort(complexNumbers);
    return 0;
};