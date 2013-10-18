// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/21/2013 11:14:30 AM
//  Last Modification Date:     07/21/2013 07:18:10 PM
//  Filename:                   curve_area.cpp
//
//  Overview:
//      This program calculates the area under a specified interval of a user-defined
//      function using either or both of the rectangle or trapezoid-based finite
//      difference methods.  The program then outputs the calculated area to the
//      console, and asks the user if he/she wants to repeat using a new function.
//
//      N.B. The program treats rectangles/trapezoids underneath the x-axis as if
//           they represent negative area, i.e., they are subtracted from the overall
//           area.
//
//  Input:
//      The user will be prompted for the following input:
//      (1) The number of terms of the polynomial whose area he/she wishes to calculate
//      (2) The coefficient of each term of the polynomial
//      (3) The exponent of each term of the polynomial
//      (4) As an alternative to (1)-(3), the user can select from predefined functions
//      (5) The type of finite difference method to use
//      (6) The starting and ending points within the function's domain to use
//          as endpoints for calculating the area under the function
//      (7) The number of rectangles and/or number of trapezoids to use to calculate
//          the area under the function
//      (8) A yes/no answer to whether to repeat the program using new values
//
//  Output:
//
//      ********************************************************************************
//      Welcome to the Riemann Sum calculator!
//      ********************************************************************************
//
//      This program calculates the area under a polynomial function of your choice.
//
//      You can choose from several predefined functions, or you can define your own.
//      Please select from one of the following:
//          1. f1(x) = 2x^5 + x^3 - 10x + 2
//          2. f2(x) = 6x^2 - x + 10
//          3. f3(x) = 5x + 3
//          4. f4(x) = 2x^3 + 120
//          5. f5(x) = 2x^2
//          6. Define your own function
//      Any other selection quits this program.
//      How would you like to continue? 2
//
//      Which method would you like to use to calculate the area under the
//      curve? Press:
//          1: trapezoid method;
//          2: rectangle method;
//          3: both trapezoid and rectangle methods.
//      Any other selection quits this program.
//      How would you like to continue? 2
//
//      Please enter the parameters of the calculation.
//          How many rectangles do you want? 100
//          Please select a starting point: 0
//          Please select an ending point: 100
//
//      Using the rectangle method, the area under 6x^2 - x + 10 from 0 to 100 is 1.96615e+06
//
//      Continue with new values (Y/n)? y
//
//      Please select from one of the following:
//          1. f1(x) = 2x^5 + x^3 - 10x +2
//          2. f2(x) = 6x^2 - x + 10
//          3. f3(x) = 5x + 3
//          4. f4(x) = 2x^3 + 120
//          5. f5(x) = 2x^2
//          6. Define your own function
//      Any other selection quits this program.
//      How would you like to continue? 6
//
//      Please enter a polynomial of up to ten (10) terms.
//          How many terms are in the polynomial?     Please enter a positive integer of 10 or under: 2
//          What is the exponent of term 1? (Enter 0 if term is a constant): 2
//          What is the coefficient of term 1? (Enter term itself if term is a constant): 3
//          What is the exponent of term 2? (Enter 0 if term is a constant): 0
//          What is the coefficient of term 2? (Enter term itself if term is a constant): 20
//
//      Which method would you like to use to calculate the area under the
//      curve? Press:
//          1: trapezoid method;
//          2: rectangle method;
//          3: both trapezoid and rectangle methods.
//      Any other selection quits this program.
//      How would you like to continue? 1
//
//      Please enter the parameters of the calculation.
//          How many trapezoids do you want? 40
//          Please select a starting point: -20
//          Please select an ending point: 0
//
//      Using the trapezoid method, the area under your polynomial from -20 to 0 is 8402.5
//
//      Continue with new values (Y/n)? n
//
// =====================================================================================

#include<iostream>  // for cout and cin
#include<cmath>     // for pow()
#include<cstring>   // for string parsing functions
#include<cstdlib>   // for atoi()
#include<iomanip>   // fot setw() and setfill()

using namespace std;

// Purpose: get values for the exponents and coefficients of a user-defined polynomial
// Entry:   terms is the number of terms the polynomial contains, as selected by the user
// Exit: the exp[] array has been filled with n=terms exponents, and the coeff[] array
// has been filled with n=terms coefficients
void get_poly_values(int& terms, double exp[], double coeff[]);

// Purpose: gets the number of trapezoids the user would like to use, as well as the
//          start and end points for calculating the area under the curve
// Entry:   none; arguments are empty until filled by the function.
// Exit:    all arguments have been filled with values of their specified data type
void get_trap_values(int& shapeNum, double& lowerBound, double& upperBound, double& step);

// Purpose: gets the number of rectangles the user would like to use, as well as the
//          start and end points for calculating the area under the curve
// Entry:   none; arguments are empty until filled by the function.
// Exit:    all arguments have been filled with values of their specified data type
void get_rect_values(int& shapeNum, double& lowerBound, double& upperBound, double& step);

// Purpose: calculates the y-value of a polynomial function at a given x-value
// Entry:   terms is the number of terms in the polynomial under consideration, exp[] contains
//          the values of the exponents in the polynomial, coeff[] contains the coefficients
//          in the polynomial, and xVal contains the x-value for which the y-value is to be
//          calculated
// Exit:    returns a double value that represents the y-value of the function; none of the
//          arguments have had their values altered
double value_at(const int terms, const double exp[], const double coeff[], const double xVal);

// Purpose: calculates the area under a function using the trazpezoid method of finite differences
// Entry:   terms is the number of terms in the polynomial under consideration, exp[] contains
//          the values of the exponents in the polynomial, coeff[] contains the coefficients
//          in the polynomial, lowerBound contains the starting x-value for the calculation,
//          lowerBound contains the ending point for the calculation, and step is the height of
//          each trapezoid
// Exit:    returns a double value that represents the area under the curve for the chosen interval;
//          none of the arguments have had their values altered
double trap_area(const int terms, const double exp[], const double coeff[], const double lowerBound, const double upperBound, const double step);


// Purpose: calculates the area under a function using the rectangle method of finite differences
// Entry:   terms is the number of terms in the polynomial under consideration, exp[] contains
//          the values of the exponents in the polynomial, coeff[] contains the coefficients
//          in the polynomial, lowerBound contains the starting x-value for the calculation,
//          lowerBound contains the ending point for the calculation, and step is the height of
//          each trapezoid
// Exit:    returns a double value that represents the area under the curve for the chosen interval;
//          none of the arguments have had their values altered
double rect_area(const int terms, const double exp[], const double coeff[], const double lowerBound, const double upperBound, const double step);

// Purpose: checks whether the user's input it a positive integer
// Entry:   input is a string containing user-inputted value
// Exit:    returns true only if the string contains a positive integer,
//          otherwise returns false
bool is_positive_int(string str);

// Purpose: solicits user to enter a positive integer
// Entry:   none
// Exit:    returns user input stored as a string object
int get_positive_int();

int main()
{
    double *exp;            // pointer to user's chosen exponent array
    double *coeff;          // pointer to user's chosen coefficient array
    int polyTerms;          // stores the number of terms in the polynomial

    double polyExp[10];     // 10-place array that contains the value of the polynomial's exponents
    double polyCoeff[10];   // 10-place array that contains the value of the polynomial's coefficients

    // The following arrays store the values of the exponent and coefficients of the predetermined
    // function.  For f1(x), e.g., the exponents are stored in funcOneExp[] and the coefficients
    // are stored in funcOneCoeff[]
    double funcOneExp[]= { 5, 3, 1, 0};
    double funcOneCoeff[] = { 2, 1, -10, 2};
    double funcTwoExp[] = { 2, 1, 0};
    double funcTwoCoeff[] = { 6, -1, 10};
    double funcThreeExp[] = { 1, 0};
    double funcThreeCoeff[] = { 5, 3};
    double funcFourExp[] = { 3, 0};
    double funcFourCoeff[] = { 2, 120};
    double funcFiveExp[] = { 2};
    double funcFiveCoeff[] = { 2};

    int funcChoice;         // store's the user's choice of the function whose area gets calculated
    int calcMethod;         // stores the user's choice of whether to use the trapezoid or rectangle
                            // method, or both

    int trapNum;            // stores the number of trapezoids
    double trapLowerBound;  // stores the lower bound of the interval
    double trapUpperBound;  // stores the upper bound of the interval
    double trapStep;        // stores the height of each trapezoid,
                            // i.e. (trapUpperBound - trapLower Bound) / trapNum

    int rectNum;            // stores the number of rectangles
    double rectLowerBound;  // stores the lower bound of the interval
    double rectUpperBound;  // stores the upper bound of the interval
    double rectStep;        // stores the height of each rectangle,
                            // i.e. (rectUpperBound - rectLower Bound) / trapNum

    double trapArea;        // stores the area under the curve using the trapezoid method
    double rectArea;        // stores the area under the curve using the rectangle method

    string funcToken;       // stores the string representing the function under evaluation

    string cont;            // stores the value of the user's answer whether to continue with new
                            // values

    cout << endl;
    cout << setfill('*') << setw(80) << "*" << endl;
    cout << "Welcome to the Riemann Sum calculator!" << endl;
    cout << setfill('*') << setw(80) << "*" << endl;
    cout << endl;
    cout << "This program calculates the area under a polynomial function of your choice." << endl;
    cout << "You can choose from several predefined functions, or you can define your own." << endl;
    cout << endl;

    reinit:

    cout << "Please select from one of the following:" << endl;
    cout << setfill(' ');
    cout << setw(4) << "" << "1. f1(x) = 2x^5 + x^3 - 10x + 2" << endl;
    cout << setw(4) << "" << "2. f2(x) = 6x^2 - x + 10" << endl;
    cout << setw(4) << "" << "3. f3(x) = 5x + 3" << endl;
    cout << setw(4) << "" << "4. f4(x) = 2x^3 + 120" << endl;
    cout << setw(4) << "" << "5. f5(x) = 2x^2" << endl;
    cout << setw(4) << "" << "6. Define your own function" << endl;
    cout << "Any other selection quits this program." << endl;
    cout << "How would you like to continue? ";
    cin >> funcChoice;

    // allow user to select from among five predefined functions,
    // or to define his/her own, or exit
    switch(funcChoice)
    {
        case 1:
            polyTerms = 4;
            exp = funcOneExp;
            coeff = funcOneCoeff;
            funcToken = "2x^5 + x^3 - 10x + 2";
            break;
        case 2:
            polyTerms = 3;
            exp = funcTwoExp;
            coeff = funcTwoCoeff;
            funcToken = "6x^2 -x + 10";
            break;
        case 3:
            polyTerms = 2;
            exp = funcThreeExp;
            coeff = funcThreeCoeff;
            funcToken = "5x + 3";
            break;
        case 4:
            polyTerms = 2;
            exp = funcFourExp;
            coeff = funcFourCoeff;
            funcToken = "2x^3 + 120";
            break;
        case 5:
            polyTerms = 1;
            exp = funcFiveExp;
            coeff = funcFiveCoeff;
            funcToken = "2x^2";
            break;
        case 6:
            cout << endl;
            get_poly_values(polyTerms, polyExp, polyCoeff);
            exp = polyExp;
            coeff = polyCoeff;
            funcToken = "your polynomial";
            break;
        default:
            return 0;
    }

    cout << endl;

    cout << "Which method would you like to use to calculate the area under the curve? Press:" << endl;
    cout << setfill(' ') << setw(4) << "" << left << "1: trapezoid method;" << endl;
    cout << setw(4) << "" << "2: rectangle method;" << endl;
    cout << setw(4) << "" << "3: both trapezoid and rectangle methods." << endl;
    cout << "Any other selection quits this program." << endl;
    cout << "How would you like to continue? ";
    cin >> calcMethod;
    cout << endl;

    // Parses the user's input stored in calcMethod.  Default behavior is to exit.
    // If calcMethod == 1, calculates using trapezoid method
    // If calcMethod == 2, calculates using rectangle method
    // If calcMethod == 3, calculates using trapezoid and rectangle methods
    switch(calcMethod)
    {
        case 1:
            get_trap_values(trapNum, trapLowerBound, trapUpperBound, trapStep);
            //trap_area(polyTerms, polyExp, polyCoeff, trapLowerBound, trapUpperBound, trapStep);
            trapArea = trap_area(polyTerms, exp, coeff, trapLowerBound, trapUpperBound, trapStep);
            cout << "Using " << trapNum << " trapezoid(s), the area under " << funcToken << " between " << trapLowerBound
                 << " and " << trapUpperBound << " is " << trapArea << endl;
            cout << endl;
            break;
        case 2:
            get_rect_values(rectNum, rectLowerBound, rectUpperBound, rectStep);
            //rect_area(polyTerms, polyExp, polyCoeff, rectLowerBound, rectUpperBound, rectStep);
            rectArea = rect_area(polyTerms, exp, coeff, rectLowerBound, rectUpperBound, rectStep);
            cout << "Using " << rectNum << " rectangle(s), the area under " << funcToken << " between " << rectLowerBound
                 << " and " << rectUpperBound << " is " << rectArea << endl;
            cout << endl;
            break;
        case 3:
            get_trap_values(trapNum, trapLowerBound, trapUpperBound, trapStep);
            get_rect_values(rectNum, rectLowerBound, rectUpperBound, rectStep);
            //trap_area(polyTerms, polyExp, polyCoeff, trapLowerBound, trapUpperBound, trapStep);
            trapArea = trap_area(polyTerms, exp, coeff, trapLowerBound, trapUpperBound, trapStep);
            //rect_area(polyTerms, polyExp, polyCoeff, rectLowerBound, rectUpperBound, rectStep);
            rectArea = rect_area(polyTerms, exp, coeff, rectLowerBound, rectUpperBound, rectStep);
            cout << "Using " << trapNum << " trapezoid(s), the area under " << funcToken << " between " << trapLowerBound
                 << " and " << trapUpperBound << " is " << trapArea << endl;
            cout << "Using " << rectNum << " rectangle(s), the area under " << funcToken << " between " << rectLowerBound
                 << " and " << rectUpperBound << " is " << rectArea << endl;
            cout << endl;
            break;
        default:
            return 0;
    }

    cin.ignore(10000, '\n');
    cout << "Continue with new values (Y/n)? ";
    getline(cin, cont);
    cout << endl;

    reask:

    // Loop until a valid yes or no answer received; goto reask, supra, input invalid;
    // goto reinit, supra, if yes; exit if no.
    if(cont == "" || cont == "Y" || cont == "y" || cont == "Yes" || cont == "yes")
    {
        goto reinit;
    }
    else if(cont == "N" || cont == "n" || cont == "No" || cont == "no")
    {
        return 0;
    }
    else
    {
        cout << "I don't understand that. Continue (Y/n)? ";
        getline(cin, cont);
        goto reask;
    }
}

void get_poly_values(int& terms, double exp[], double coeff[])
{
    cout << "Please enter a polynomial of up to ten (10) terms." << endl;
    cout << setfill(' ');
    cout << setw(4) << "" << "How many terms are in the polynomial? ";
    terms = get_positive_int(); // call get_positive_int() to solicit number of terms

    // Loops for x==terms times, storing exponents and coefficients in separate arrays,
    // but at same array index
    for(int index = 0; index < terms; index++)
    {
        cout << setw(4) << "" << "What is the exponent of term " << index + 1 << "? (Enter 0 if term is a constant): ";
        cin >> exp[index];
        cout << setw(4) << "" << "What is the coefficient of term " << index + 1 << "? (Enter term itself if term is a constant): " << setw(0);
        cin >> coeff[index];
    }
    cout << endl;
}

void get_trap_values(int& shapeNum, double& lowerBound, double& upperBound, double& step)
{
    cout << "Please enter the parameters of the calculation." << endl;
    cout << setfill(' ') << setw(4);
    cout << setw(4) << "" << "How many trapezoids do you want? ";
    cin >> shapeNum;
    cout << setw(4) << "" << "Please select a starting point: ";
    cin >> lowerBound;
    cout << setw(4) << "" << "Please select an ending point: " << setw(0);
    cin >> upperBound;
    cout << endl;

    // deal with lower bounds that are higher than upper bound
    if(lowerBound > upperBound)
    {
        double temp = lowerBound;
        lowerBound = upperBound;
        upperBound = temp;
    }

    step = ((upperBound - lowerBound) / shapeNum);  // the height of each trapezoid
}

void get_rect_values(int& shapeNum, double& lowerBound, double& upperBound, double& step)
{
    cout << "Please enter the parameters of the calculation." << endl;
    cout << setfill(' ') << setw(4);
    cout << setw(4) << "" << "How many rectangles do you want? ";
    cin >> shapeNum;
    cout << setw(4) << "" << "Please select a starting point: ";
    cin >> lowerBound;
    cout << setw(4) << "" << "Please select an ending point: " << setw(0);
    cin >> upperBound;
    cout << endl;

    // deal with lower bounds that are higher than upper bound
    if(lowerBound > upperBound)
    {
        double temp = lowerBound;
        lowerBound = upperBound;
        upperBound = temp;
    }

    step = ((upperBound - lowerBound) / shapeNum);  // the width of each rectangle
}

double value_at(const int terms, const double exp[], const double coeff[], const double xVal)
{
    double funcValue = 0;

    // iterates x==terms times, getting the exponent and coefficient for
    // the polynomial term stored at array[index], calling pow() to
    // successively add to funcValue until this reaches the y-value for
    // the polynomial at xVal
    for(int index = 0; index < terms; index ++)
    {
        //funcValue += fabs(coeff[index]*pow(xVal, exp[index]));
        funcValue += coeff[index]*pow(xVal, exp[index]);
    }

    return funcValue;
}

double trap_area(const int terms, const double exp[], const double coeff[], const double lowerBound, const double upperBound, const double step)
{
    double areaUnder = 0;

    // loops, calling value_at() to get y-values for use in calculating
    // the area of each successive trapezoid.  Adds this area to
    // areaUnder until it has added all of the user-selected number of
    // trapezoids.
    for(double xIndex = lowerBound; xIndex < upperBound; xIndex += step)
    {
        areaUnder += 0.5 * (value_at(terms, exp, coeff, xIndex) + value_at(terms, exp, coeff, (xIndex + step))) * step;
    }

    return areaUnder;
}

double rect_area(const int terms, const double exp[], const double coeff[], const double lowerBound, const double upperBound, const double step)
{
    double areaUnder = 0;

    // loops, calling value_at() to get y-values for use in calculating
    // the area of each successive rectangle.  Adds this area to
    // areaUnder until it has added all of the user-selected number of
    // rectangles.
    for(double xIndex = lowerBound; xIndex < upperBound; xIndex += step)
    {
        areaUnder += value_at(terms, exp, coeff, xIndex) * step;
    }

    return areaUnder;
}

bool is_positive_int(string str)
{
    int stringSize = str.size(); // stores the size of the user-entered string
    //debugging --> cout << "The size of that string is " << stringSize << endl;

    // reject empty strings
    if(str == "") // || str.at(1) >= 1)
        return false;

    // reject strings of value greater than 11
    if(stringSize >= 2 && str.at(1) >= 1)
        return false;

    // for non-empty strings, check that character at each position of the string
    // is greater than or equal to 0 and less than or equal to 9;
    // reject for any other values
    for(int ltrIndex = 0; ltrIndex < stringSize; ltrIndex++)
    {
        //debugging --> cout << "The character at position " << ltrIndex << " is " << str.at(ltrIndex) << endl;
        if(str.at(ltrIndex) < '0' || str.at(ltrIndex) > '9')
        {
            return false;
        }
    }
    return true;    // returns true only if string passes the above test
}

int get_positive_int()
{
    string strInt;  // stores value of user input
    cin.ignore(10000, '\n');

    retry:

    // Prompts for user input, stores in strInt
    cout << setfill(' ') << setw(4) << "" << "Please enter a positive integer of 10 or under: ";
    getline(cin, strInt);

    // Repeats user prompt and value storage for any
    // value of strInt that is not a positive integer
    if(!is_positive_int(strInt))
    {
        cout << setfill(' ') << setw(4) << "" << "That input is invalid." << endl;
        goto retry;
    }

    // converts strInt to a C str for user with atoi();
    // uses atoi() to get an int value which is then
    // returned
    int intStr = atoi(strInt.c_str());
    return intStr;
}
