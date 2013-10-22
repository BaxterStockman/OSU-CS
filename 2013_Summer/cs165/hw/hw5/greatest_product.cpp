// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/07/2013 06:30:07 AM
//  Last Modification Date:
//  Filename:                   greatest_product.cpp
//
//  Overview:
//      This program reads a file containing a matrix of values, then calculates
//      the largest multiple of a specified number of adjacent and/or diagonal values.
//
//  Input:
//      This program reads a file containing a matrix of values.  IT WILL NOT WORK WITHOUT
//      THE FILE.
//
//  Output:
//      Prints the largest multiple to the console.
//
// =====================================================================================

#include<iostream>
#include<vector>
#include<string>
#include<fstream>   // for ifstream()
#include<sstream>   // for stringstream()
#include<iterator>  // for istream_iterator()
#include<algorithm> // for count()
#include<iomanip>   // for setw() and left

#define MATRIX_FILE "20_x_20_matrix.txt"
#define MATRIX_ROWS 20
#define MATRIX_COLS 20

using namespace std;

// This class holds a matrix of long int values
class matrix
{
public:

    int rows;
    int cols;

    vector< vector<long int> > elems;

    friend ostream& operator<< (ostream &out, const matrix &box);
    friend istream& operator>> (istream &input, matrix &box);
};

// Purpose: overload the << operator for matrix object
// Entry:   out is an output stream; box is a matrix object
// Exit:    content of box piped to output stream
ostream& operator<< (ostream &out, const matrix &box);

// Purpose: overload the >> operator for matrix object
// Entry:   input is an input stream; box is a matrix object
// Exit:    content of input piped to box
istream& operator>> (istream &input, matrix &box);

// Purpose: creates a matrix object based on the contents of a file
// Entry:   there must exist a file to be read
// Exit:    returns matrix containing values read from file
matrix parse_matrix_file();

// Purpose: calculates the product of numbers starting at (x, y) and ending
//          numMults - 1 positions to the right of (x, y)
// Entry:   box is a matrix object, x is the number of columns from the left at
//          which to start the calculation, y is the number of rows from the top,
//          and numMults is the length of the series of numbers to multiply
// Exit:    returns the product of numMults numbers arranged as described
//          above
long int right_horiz_product(matrix box, int x, int y, int numMults);

// Purpose: calculates the product of numbers starting at (x, y) and ending
//          numMults - 1 positions down from (x, y)
// Entry:   box is a matrix object, x is the number of columns from the left at
//          which to start the calculation, y is the number of rows from the top,
//          and numMults is the length of the series of numbers to multiply
// Exit:    returns the product of numMults numbers arranged as described
//          above
long int down_vert_product(matrix box, int x, int y, int numMults);

// Purpose: calculates the product of numbers starting at (x, y) and ending
//          (x + numMults - 1, y + numMults - 1)
// Entry:   box is a matrix object, x is the number of columns from the left at
//          which to start the calculation, y is the number of rows from the top,
//          and numMults is the length of the series of numbers to multiply
// Exit:    returns the product of numMults numbers arranged as described
//          above
long int right_down_diag_product(matrix box, int x, int y, int numMults);

// Purpose: calculates the product of numbers starting at (x, y) and ending
//          (x - numMults + 1, y + numMults - 1)
// Entry:   box is a matrix object, x is the number of columns from the left at
//          which to start the calculation, y is the number of rows from the top,
//          and numMults is the length of the series of numbers to multiply
// Exit:    returns the product of numMults numbers arranged as described
//          above
long int left_down_diag_product(matrix box, int x, int y, int numMults);

// Purpose: sorts a vector, returning the largest number in it
// Entry:   rawList is a vector containing values of type long int
// Exit:    returns largest value in rawList
long int find_largest_num(vector<long int> &rawList);

int main()
{
    matrix box = parse_matrix_file();   // instantiates a matrix based on content of text file
    vector<long int> productList;       // holds the values of the multiplication operations

    const int prodLength = 4;   // number of values to multiply
    long int product;           // temporary holder for multiplication results
    long int largestNum;        // largest product of prodLength numbers

    // uncomment to print matrix to console
    //cout << box;

    // This for loop checks that the functions won't go out-of-range,
    // then invokes the function and stores the returned value in
    // productList vector
    for(int x = 0; x < box.cols; x++)
    {
        for(int y = 0; y < box.rows; y++)
        {
            if(x < (box.cols-prodLength+1))
            {
                product = right_horiz_product(box, x, y, prodLength);
                productList.push_back(product);
            }
            if(y < (box.rows-prodLength+1))
            {
                product = down_vert_product(box, x, y, prodLength);
                productList.push_back(product);
            }
            if(x < (box.cols-prodLength+1) && y < (box.rows-prodLength+1))
            {
                product = right_down_diag_product(box, x, y, prodLength);
                productList.push_back(product);
            }
            if(x > (prodLength-2) && y < (box.rows-prodLength+1))
            {
                product = left_down_diag_product(box, x, y, prodLength);
                productList.push_back(product);
            }
        }
    }

    largestNum = find_largest_num(productList);

    cout << "The largest product of " << prodLength << " adjacent or diagonal numbers in the entered matrix is: " << largestNum << endl;

    return 0;
}

ostream& operator<< (ostream &out, const matrix &box)
{
    // formats output so that matrix is printed with numbers in
    // columns aligned to the left
    for(int yInd = 0; yInd < box.rows; yInd++)
    {
        for(int xInd = 0; xInd < box.cols; xInd++)
            out << setw(4) << box.elems[xInd][yInd];
        out << endl;
    }
    return out;
}

istream& operator>> (istream &input, matrix &box)
{
    string rowStr;      // store contents of getline()
    int rowLen;         // number of discrete elements in rowStr
    long int element;   // holds value of discrete element in rowStr

    getline(input, rowStr);

    // The following line is due to user Dietmar Kuhl, who posted it at
    // http://stackoverflow.com/questions/12875503/counting-the-number-of-words-in-a-string-c
    rowLen = distance(istream_iterator<string>(istringstream(rowStr) >> ws), istream_iterator<string>());

    // stream rowStr for capture of discrete elements
    stringstream ss(rowStr);

    for(int xInd = 0; xInd < rowLen; xInd++)
    {
        ss >> element;  // capture discrete element from stream
        box.elems.at(xInd).push_back(element);  // add element to vector
    }

    return input;
}

matrix parse_matrix_file()
{
    matrix box;
    box.rows = MATRIX_ROWS;
    box.cols = MATRIX_COLS;
    vector<long int> emptyVect;

    ifstream input(MATRIX_FILE);    // open input file

    // Check that file is open; raise error if not
    if(!input.is_open())
        cout << endl << "Could not open file!" << endl << endl;

    // create a vector for each column in box
    for(int xInd = 0; xInd < box.cols; xInd++)
        box.elems.push_back(emptyVect);

    // fill vectors with values from file
    for(int index = 0; index < box.rows; index++)
        input >> box;

    return box;
}

// calculates the product of all vertical series of four in matrix
long int right_horiz_product(matrix box, int x, int y, int numMults)
{
    // recurse until numMults == 0, re-running the function on the number one position to
    // the right from the previous number.
    if(numMults > 0)
    {
        return box.elems[x][y] * right_horiz_product(box, x+1, y, numMults-1);
    }
    else
        return 1;
}

// calculates the product of all horizontal series of four in matrix
long int down_vert_product(matrix box, int x, int y, int numMults)
{
    // recurse until numMults == 0, re-running the function on the number one position
    // down from the previous number
    if(numMults > 0)
    {
        return box.elems[x][y] * down_vert_product(box, x, y+1, numMults-1);
    }
    else
        return 1;
}

// calculates the product of all right-slanted diagonal series of four in matrix
long int right_down_diag_product(matrix box, int x, int y, int numMults)
{
    // recurse until numMults == 0, re-running the function on the number one position to
    // the right and one position down from the previous number.
    if(numMults > 0)
    {
        return box.elems[x][y] * right_down_diag_product(box, x+1, y+1, numMults-1);
    }
    else
        return 1;
}

// calculates the product of all left-slanted diagonal series of four in matrix
long int left_down_diag_product(matrix box, int x, int y, int numMults)
{
    // recurse until numMults == 0, re-running the function on the number one position to
    // the left and one position down from the previous number.
    if(numMults > 0)
    {
        return box.elems[x][y] * left_down_diag_product(box, x-1, y+1, numMults-1);
    }
    else
        return 1;
}

long int find_largest_num(vector<long int> &rawList)
{
    int temp;
    int vectSize = rawList.size();

    // if the vector has only one element, it is the largest value
    // from the initial vector
    if(vectSize == 1)
    {
        return rawList.at(0);
    }
    // recurse on vector with last value removed
    else if(rawList.at(vectSize-2) > rawList.at(vectSize-1))
    {
        rawList.pop_back();
        find_largest_num(rawList);
    }
    // recurse on vector with second-to-last value removed
    else
    {
        temp = rawList.at(vectSize-1);
        rawList.pop_back();
        rawList.pop_back();
        rawList.push_back(temp);
        find_largest_num(rawList);
    }
}



