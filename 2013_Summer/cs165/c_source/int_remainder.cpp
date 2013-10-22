#include <iostream>
using namespace std;

int main( ) 
{
    int wholeNum1, wholeNum2;
    
    cout << "Enter a whole number ";
    cin >> wholeNum1;

    cout << "Enter another whole number ";
    cin >> wholeNum2;

    cout << "The value of the first number divided by the second is " << wholeNum1 / wholeNum2 << "\n";
    cout << "and the remainder is " << wholeNum1 % wholeNum2 << "\n";

    return 0;
}

