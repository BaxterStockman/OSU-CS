// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/06/2013 09:02:18 PM
//  Last Modification Date:
//  Filename:                   matrix_product.cpp
//
//  Overview:
//      Calculates the matrix product of two matrices of identical dimensions, the
//      prints the product to the console
//
//  Input:
//      The user must enter the values at each position of the two matrices to be multiplied
//
//  Output:
//      Prints to the console the matrices entered by the user, as well as the product
//      of the first matrix into the second.
//
//  Sample output:
//      Please enter the values for the first matrix.
//
//      What value for matrix at row 0, column 0?  1
//      What value for matrix at row 0, column 1?  2
//      What value for matrix at row 1, column 0?  3
//      What value for matrix at row 1, column 1?  4
//
//      Please enter the values for the first matrix.

//      What value for matrix at row 0, column 0?  1
//      What value for matrix at row 0, column 1?  2
//      What value for matrix at row 1, column 0?  3
//      What value for matrix at row 1, column 1?  4
//
//      The matrix product of
//      [ 1   2   ]
//      [ 3   4   ]
//
//      and
//      [ 1   2   ]
//      [ 3   4   ]
//
//      is
//      [ 7   10  ]
//      [ 15  22  ]
//
// =====================================================================================

#include<iostream>
#include<iomanip>   // for setw()
#include<string>    // for string object
#include<sstream>   // for stringstream

#define ROWS_AND_COLS 10

using namespace std;

const int rows = ROWS_AND_COLS; // number of rows in matrices
const int cols = ROWS_AND_COLS; // number of columns in matrices

// Purpose: prompt for and get user values for all elements of matrix
// Entry:   matrix is a 2d array of double values
// Exit:    matrix has been filled with user-selected values
void get_matrix_values(double matrix[][cols]);

// Purpose: calculate the matrix product of two matrices of identical dimensions
// Entry:   all arguments are 2d arrays of double values
// Exit:    prod contains the matrix product of lhs into rhs
void matrix_product(double lhs[][cols], double rhs[][cols], double prod[][cols]);

// Purpose: print contents of matrix to console
// Entry:   matrix is a 2d array of double values
// Exit:    values in matrix printed to console
void print_matrix(double matrix[][cols]);

int main()
{
    double lhs[rows][cols], rhs[rows][cols], prod[rows][cols];  // declare three 2d arrays of doubles

    cout << endl << "Please enter the values for the first matrix." << endl << endl;
    get_matrix_values(lhs);
    cout << endl << "Please enter the values for the second matrix." << endl << endl;
    get_matrix_values(rhs);
    cout << endl;

    matrix_product(lhs, rhs, prod);

    cout << "The matrix product of" << endl;
    print_matrix(lhs);
    cout << endl << "and" << endl;
    print_matrix(rhs);
    cout << endl << "is" << endl;
    print_matrix(prod);
    cout << endl;

    return 0;
}

void get_matrix_values(double matrix[][cols])
{
    string input;

    for(int yInd = 0; yInd < rows; yInd++)
    {
        for(int xInd = 0; xInd < cols; xInd++)
        {
            reloop:
            cout << "What value for matrix at row " << yInd << ", column " << xInd <<"?  ";
            // input checker: makes sure that user's entry contains only numbers and periods.
            // not foolproof by any means, but at least prevents empty strings and strings
            // containing letters
            while(getline(cin, input))
            {
                if(input.find_first_not_of("0123456789.") == string::npos && input.length() != 0)
                {
                    stringstream ss(input);
                    ss >> matrix[xInd][yInd];
                    break;
                }
                else
                {
                    cout << "That input is invalid." << endl;
                    goto reloop;
                }
            }
        }
    }
}

void matrix_product(double lhs[][cols], double rhs[][cols], double prod[][cols])
{
    // performs matrix multiplication
    for(int yInd = 0; yInd < rows; yInd++)
    {
        for(int xInd = 0; xInd < cols; xInd++)
        {
            // x-dimension of lhs matrix corresponds to y-dimension of rhs matrix
            // for matrix multiplication
            for(int multI = 0; multI < rows; multI++)
                prod[xInd][yInd] += lhs[multI][yInd]*rhs[xInd][multI];
        }
    }
}

void print_matrix(double matrix[][cols])
{
    // Loops through final matrix, prints all values separated by tabs
    for(int yInd = 0; yInd < rows; yInd++)
    {
        cout << "[ ";
        for(int xInd = 0; xInd < cols; xInd++)
        {
             cout << setw(4) << left << matrix[xInd][yInd];
        }
        cout << "]" << endl;
    }
}
