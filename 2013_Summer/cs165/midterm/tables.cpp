// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/03/2013 08:09:30 PM
//  Last Modification Date:     08/05/2013 09:14:40 PM
//  Filename:                   tables.cpp
//
//  Overview:
//      This program creates and prints to the console multiplication and division tables
//      of length and width specified by the user.
//
//  Input:
//      The number of rows and columns are entered as command line arguments, as follows:
//      ./tables <rows> <columns>.  The program assumes that the values input as rows
//      and columns can be converted to integers; bad stuff happens if they cannot.
//
//  Output:
//      The program outputs one multiplication table and one division table, where the
//      multiplicands are the number of the row by the number of the column, and where
//      the number of the row is the dividend and the number of the column is the divisor.
//
//  Sample Output:
//
//  ./tables 10 10
//
//  A multiplication table of 10 rows by 10 columns:
//
//  1       2       3       4       5       6       7       8       9       10
//  2       4       6       8       10      12      14      16      18      20
//  3       6       9       12      15      18      21      24      27      30
//  4       8       12      16      20      24      28      32      36      40
//  5       10      15      20      25      30      35      40      45      50
//  6       12      18      24      30      36      42      48      54      60
//  7       14      21      28      35      42      49      56      63      70
//  8       16      24      32      40      48      56      64      72      80
//  9       18      27      36      45      54      63      72      81      90
//  10      20      30      40      50      60      70      80      90      100
//
//
//  A division table of 10 rows by 10 columns:
//
//  1.00    0.50    0.33    0.25    0.20    0.17    0.14    0.12    0.11    0.10
//  2.00    1.00    0.67    0.50    0.40    0.33    0.29    0.25    0.22    0.20
//  3.00    1.50    1.00    0.75    0.60    0.50    0.43    0.38    0.33    0.30
//  4.00    2.00    1.33    1.00    0.80    0.67    0.57    0.50    0.44    0.40
//  5.00    2.50    1.67    1.25    1.00    0.83    0.71    0.62    0.56    0.50
//  6.00    3.00    2.00    1.50    1.20    1.00    0.86    0.75    0.67    0.60
//  7.00    3.50    2.33    1.75    1.40    1.17    1.00    0.88    0.78    0.70
//  8.00    4.00    2.67    2.00    1.60    1.33    1.14    1.00    0.89    0.80
//  9.00    4.50    3.00    2.25    1.80    1.50    1.29    1.12    1.00    0.90
//  10.00   5.00    3.33    2.50    2.00    1.67    1.43    1.25    1.11    1.00
//
// =====================================================================================

#include<iostream>
#include<iomanip>   // for setprecision()
#include<cstdlib>

using namespace std;

// This struct contains an int variable for holding the multiplication table values,
// and a float variable for holding the division table values.
struct mult_div_values
{
    int mult;
    float div;
};

// Purpose: This function creates a table of mult_div_values structs with
//          m rows and n columns
// Entry:   m and n are integers converted from chars entered as command line
//          arguments
// Exit:    Returns a table of mult_div_values
mult_div_values** create_table_twoD(int m, int n);

void add_columns(mult_div_values *retTable[], int m, int n);

// Purpose: This function loops through all positions of a table of m rows
//          by n columns, setting the value of mult_div_values.mult for each
//          position.
// Entry:   m and n are integers converted from chars entered as command line
//          arguments
// Exit:    The table now contains a mult_div_values.mult value for each
//          position.
void set_mult_values(mult_div_values **table, int m, int n);

// Purpose: This function loops through all positions of a table of m rows
//          by n columns, setting the value of mult_div_values.div for each
//          position.
// Entry:   m and n are integers converted from chars entered as command line
//          arguments
// Exit:    The table now contains a mult_div_values.div value for each
//          position.
void set_div_values(mult_div_values **table, int m, int n);

void print_table(mult_div_values **table, int m, int n);

int main(const int argc, char **argv)
{
    // Convert command line arguments to ints
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    mult_div_values array1[rows][cols], *array2[cols], **array3;

    // Create an instance of mult_div_values
    array3 = create_table_twoD(rows, cols);

    add_columns(array2, rows, cols);

    // set .mult and .div values
//    set_mult_values(array1, rows, cols);
//    set_div_values(array1, rows, cols);

    set_mult_values(array2, rows, cols);
    set_div_values(array2, rows, cols);

    set_mult_values(array3, rows, cols);
    set_div_values(array3, rows, cols);

//    print_table(array1, rows, cols);
    print_table(array2, rows, cols);
    print_table(array3, rows, cols);


    return 0;
}

mult_div_values** create_table_twoD(int m, int n)
{
    mult_div_values **retTable;
    retTable = new mult_div_values*[m];             // allocate memory for m rows
    for (int iRow = 0; iRow < m; iRow++)
        retTable[iRow] = new mult_div_values[n];    // allocate memory for n columns

    return retTable;
}

void add_columns(mult_div_values *retTable[], int m, int n)
{
    for (int iRow = 0; iRow < m; iRow++)
        retTable[iRow] = new mult_div_values[n];    // allocate memory for n columns
}

void set_mult_values(mult_div_values **table, int m, int n)
{
    for(int iRow=0; iRow < m; iRow ++)
    {
        for(int iCol=0; iCol < n; iCol ++)
        {
            table[iRow][iCol].mult = (iRow+1) * (iCol+1);   // necessary to add 1, since
        }                                                   // array indices begin at 0
    }
}

void set_div_values(mult_div_values **table, int m, int n)
{
    for(int iRow=0; iRow < m; iRow ++)
    {
        for(int iCol=0; iCol < n; iCol ++)
        {
            table[iRow][iCol].div = (float(iRow+1)) / (float(iCol+1));
        }
    }
}


void print_table(mult_div_values **table, int m, int n)
{
    cout << endl << "A multiplication table of " << m << " rows by " << n << " columns:" << endl << endl;

    for(int iRow=0; iRow < m; iRow ++)
    {
        for(int iCol=0; iCol < n; iCol ++)
            cout << table[iRow][iCol].mult << '\t';
        cout << endl;
    }

    cout << endl << endl << "A division table of " << m << " rows by " << n << " columns:" << endl << endl;

    for(int iRow=0; iRow < m; iRow ++)
    {
        for(int iCol=0; iCol < n; iCol ++)
            cout << fixed << setprecision(2) << table[iRow][iCol].div << '\t';
        cout << endl;
    }

    cout << endl;
}
