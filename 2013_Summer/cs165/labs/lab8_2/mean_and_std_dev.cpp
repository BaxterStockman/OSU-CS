// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/08/2013 11:58:00 AM
//  Last Modification Date:
//  Filename:                   mean_and_std_dev.cpp
//
//  Overview:   This program takes a (more or less) arbitrary number of decimal values
//              entered by the user, stores them in a vector, dynamically-allocated array,
//              and automatically-allocated array, and then calculates the mean and standard
//              deviation of each of the sequences.
//
//  Input:      The user enters decimal values at the command line.
//
//
//  Output:     The program prints to the console the mean and standard deviation of each of
//              the three sequences, together with introductory/clarifying statements.
//
//
//  Sample output:
//
//  Getting values for vector ---
//  Please enter a decimal value (enter nothing to continue): 10
//  Please enter a decimal value (enter nothing to continue): 20
//  Please enter a decimal value (enter nothing to continue): 30
//  Please enter a decimal value (enter nothing to continue): 40
//  Please enter a decimal value (enter nothing to continue): 50
//  Please enter a decimal value (enter nothing to continue):
//
//  Getting values for dynamic array ---
//  Please enter a decimal value (enter nothing to continue): 10
//  Please enter a decimal value (enter nothing to continue): 20
//  Please enter a decimal value (enter nothing to continue): 30
//  Please enter a decimal value (enter nothing to continue): 40
//  Please enter a decimal value (enter nothing to continue): 50
//  Please enter a decimal value (enter nothing to continue):
//
//  Getting values for static array ---
//  Please enter a decimal value (enter nothing to continue): 10
//  Please enter a decimal value (enter nothing to continue): 20
//  Please enter a decimal value (enter nothing to continue): 30
//  Please enter a decimal value (enter nothing to continue): 40
//  Please enter a decimal value (enter nothing to continue): 50
//  Please enter a decimal value (enter nothing to continue):
//
//  Calculating mean for vector ---
//  The mean of that series is:  30
//
//  Calculating mean for dynamic array ---
//  The mean of that series is:  30
//
//  Calculating mean for static array ---
//  The mean of that series is:  30
//
//  Calculating standard deviation for vector ---
//  The standard deviation of that series is:  14.1421
//
//  Calculating standard deviation  for dynamic array ---
//  The standard deviation of that series is:  14.1421
//
//  Calculating standard deviation for static array ---
//  The standard deviation of that series is:  14.1421
//
// =====================================================================================

#include<iostream>  // for cin and cout
#include<vector>    // for vector object and associated methods like size()
#include<string>    // for string object and associated methods like length()
#include<sstream>   // for stringstream()
#include<cmath>     // for pow()

// 10MB is the stack size of the flip server, according to ulimit -s
#define STACK_SIZE_LIMIT 10485760

using namespace std;

// A double takes up 8 bytes.  The following line defines the max size
// of a double array to be half of the specified STACK_SIZE_LIMIT,
// which I hope is a sane value
const int double_arr_limit = (STACK_SIZE_LIMIT / 8) / 2;

// Purpose: Fills a vector with double values
// Entry:   series is a vector holding double values; prompt is a string
//          soliciting user input
// Exit:    series has been filled with double values
void get_vector_values(vector<double> &series, string prompt);

// Purpose: Fills a dynamically-allocated array with double values
// Entry:   series is a pointer to double values; seriesLen is an int holding
//          the length of series; prompt is a string soliciting user input
// Exit:    series has been filled with double values; seriesLen has been
//          increased to reflect the number of values held in series
void get_dyn_array_values(double *&series, int &seriesLen, string prompt);

// Purpose: Fills a statically-allocated array with double values
// Entry:   series is an array of double values; seriesLen is an int holding
//          the length of series; prompt is a string soliciting user input
// Exit:    series has been filled with double values; seriesLen has been
//          increased to reflect the number of values held in series
void get_static_array_values(double series[], int &seriesLen, string prompt);

// Purpose: Calculates the mean average of the doubles held in a vector
// Entry:   series is a vector holding double values
// Exit:    Returns the mean average of the values in series
double mean_vector(vector<double> &series);

// Purpose: Calculates the mean average of the doubles held in an array
// Entry:   series is a pointer to an array holding double values; seriesLen
//          is the number of values in series
// Exit:    Returns the mean average of the values in series
double mean_dyn_array(double *series, const int seriesLen);

// Purpose: Calculates the mean average of the doubles held in an array
// Entry:   series is an array holding double values; seriesLen is the number
//          of values in series
// Exit:    Returns the mean average of the values in series
double mean_static_array(double series[], const int seriesLen);

// Purpose: Calculates the standard deviation of the doubles held in a vector
// Entry:   series is a vector holding double values; mean is a double variable
//          holding the mean average of the elements of series
// Exit:    Returns the mean average of the values in series
double std_dev_vector(vector<double> &series, const double mean);

// Purpose: Calculates the standard deviation of the doubles held in a dynamically-
//          allocated array
// Entry:   series is a pointer to an array holding double values; mean is a double
//          variable holding the mean average of the elements of series
// Exit:    Returns the mean average of the values in series
double std_dev_dyn_array(double *series, const int seriesLen, const double mean);

// Purpose: Calculates the standard deviation of the doubles held in a dynamically-
//          allocated array
// Entry:   series is an array holding double values; mean is a double variable
//          holding the mean average of the elements of series
// Exit:    Returns the mean average of the values in series
double std_dev_static_array(double series[], const int seriesLen, const double mean);

// Purpose: Prints to the console variables of type double preceded by a specified
//          message
// Entry:   value is a variable of type double; intro is a string object containing
//          a message to be printed to the console
// Exit:    prints intro and value to the console
void print_double(double value, const string intro);

int main()
{
    vector<double> vectSeries;                  // vector for holding double values
    double *dynArrSeries;                       // pointer to point to array of double values
    double staticArrSeries[double_arr_limit];   // array of double values

    int dynArrLen = 0;      // length of dynamically-allocated array, initialized at 0
    int staticArrLen = 0;   // length of statically-allocated array, initialized at 0

    double vectMean;        // holds the mean of the elements in vectSeries
    double dynArrMean;      // holds the mean of the elements in dynArrSeries
    double staticArrMean;   // holds the mean of the elements in staticArrSeries

    double vectStdDev;      // holds the standard deviation of the elements in vectSeries
    double dynArrStdDev;    // holds the standard deviation of the elements in dynArrSeries
    double staticArrStdDev; // holds the standard deviation of the elements in staticArrSeries

    string prompt = "Please enter a decimal value (enter nothing to continue): ";
    string meanStr = "The mean of that series is: ";
    string stdDevStr = "The standard deviation of that series is: ";

    // Fill the various data structures with user-entered values
    cout << endl << "Getting values for vector ---" << endl;
    get_vector_values(vectSeries, prompt);
    cout << endl << "Getting values for dynamic array ---" << endl;
    get_dyn_array_values(dynArrSeries, dynArrLen, prompt);
    cout << endl << "Getting values for static array ---"  << endl;
    get_static_array_values(staticArrSeries, staticArrLen, prompt);

    // Get and print to the console the means of the various data structures
    cout << endl << "Calculating mean for vector ---" << endl;
    vectMean = mean_vector(vectSeries);
    print_double(vectMean, meanStr);
    cout << endl << "Calculating mean for dynamic array ---" << endl;
    dynArrMean = mean_dyn_array(dynArrSeries, dynArrLen);
    print_double(dynArrMean, meanStr);
    cout << endl << "Calculating mean for static array ---" << endl;
    staticArrMean = mean_static_array(staticArrSeries, staticArrLen);
    print_double(staticArrMean, meanStr);

    // Get and print to the console the standard deviations of the various data structures
    cout << endl << "Calculating standard deviation for vector ---" << endl;
    vectStdDev = std_dev_vector(vectSeries, vectMean);
    print_double(vectStdDev, stdDevStr);
    cout << endl << "Calculating standard deviation  for dynamic array ---" << endl;
    dynArrStdDev = std_dev_dyn_array(dynArrSeries, dynArrLen, dynArrMean);
    print_double(dynArrStdDev, stdDevStr);
    cout << endl << "Calculating standard deviation for static array ---" << endl;
    staticArrStdDev = std_dev_static_array(staticArrSeries, staticArrLen, staticArrMean);
    print_double(staticArrStdDev, stdDevStr);
    cout << endl;

    return 0;
}

void get_vector_values(vector<double> &series, string prompt)
{
    string str;
    double element;

    cout << prompt;
    while(getline(cin, str))
    {
        // Verify that the user-entered string contains only numerals and decimal points,
        // and that it is non-empty
        // If so, create a stringstream object that can be inputted into a double variable,
        // then store this variable
        if(str.find_first_not_of("1234567890.") == string::npos && str.length() != 0)
        {
            stringstream ss(str);
            ss >> element;
            series.push_back(element);
        }
        // Exit loop if the user enters an empty string
        else if(str.length() == 0)
        {
            break;
        }
        // All other input gets an error message
        else
        {
            cout << "That input is invalid." << endl;
        }
        cout << prompt;
    }
}

void get_dyn_array_values(double *&series, int &seriesLen, string prompt)
{
    series = new double[0];
    string str;
    double element;
    double *temp;               // temporary pointer for holding sequence while expanding array
    int expandTo = seriesLen;   // holds new limit for array when array runs out of space

    cout << prompt;
    while(getline(cin, str))
    {
        // Verify that the user-entered string contains only numerals and decimal points,
        // and that it is non-empty
        // If so, create a stringstream object that can be inputted into a double variable,
        // then store this variable
        if(str.find_first_not_of("1234567890.") == string::npos && str.length() != 0)
        {
            stringstream ss(str);
            ss >> element;

            // When the length of the sequence of values equals or exceeds the array's
            // capacity, double the array's size if the array already has any positions,
            // or make an array of length 1 if not
            if(seriesLen >= expandTo)
            {
                if(expandTo > 0)
                    expandTo *= 2;
                else
                    expandTo = 1;
                temp = new double[expandTo];
                for(int index = 0; index < seriesLen; index++)
                    temp[index] = series[index];
                temp[seriesLen] = element;
                delete [] series;
                series = temp;
            }
            else
            {
                series[seriesLen] = element;
            }

            seriesLen++;
        }
        // Exit loop if the user enters an empty string
        else if(str.length() == 0)
        {
            break;
        }
        // All other input gets an error message
        else
        {
            cout << "That input is invalid." << endl;
        }

        cout << prompt;
    }
}

void get_static_array_values(double series[], int &seriesLen, string prompt)
{
    string str;
    double element;

    cout << prompt;
    while(getline(cin, str))
    {
        // Let the user know that the array can't hold any more values, then exit loop
        if(seriesLen == double_arr_limit)
        {
            cout << "The length of the series is " << seriesLen << endl;
            cout << "The maximum length of this array is " << double_arr_limit << endl;
            cout << "Breaking because we've hit the limit." << endl;
            break;
        }

        // Verify that the user-entered string contains only numerals and decimal points,
        // and that it is non-empty
        // If so, create a stringstream object that can be inputted into a double variable,
        // then store this variable
        if(str.find_first_not_of("1234567890.") == string::npos && str.length() != 0)
        {
            stringstream ss(str);
            ss >> element;
            series[seriesLen] = element;
            seriesLen++;
        }
        // Exit loop if the user enters an empty string
        else if(str.length() == 0)
        {
            break;
        }
        // All other input gets an error message
        else
        {
            cout << "That input is invalid." << endl;
        }

        cout << prompt;
    }
}

double mean_vector(vector<double> &series)
{
    const int seriesLen = series.size();
    double sum;
    double mean;

    // Loop aggregates the sum of the elements in series
    for(int index = 0; index < seriesLen; index++)
        sum +=series.at(index);

    mean = (sum / seriesLen);
    return mean;
}

double mean_dyn_array(double *series, const int seriesLen)
{
    double sum;
    double mean;

    // Loop aggregates the sum of the elements in series
    for(int index = 0; index < seriesLen; index++)
        sum +=series[index];

    mean = (sum / seriesLen);
    return mean;
}

double mean_static_array(double series[], const int seriesLen)
{
    double sum;
    double mean;

    // Loop aggregates the sum of the elements in series
    for(int index = 0; index < seriesLen; index++)
        sum +=series[index];

    mean = (sum / seriesLen);
    return mean;
}

double std_dev_vector(vector<double> &series, const double mean)
{
    double squareSum = 0;
    double variance;
    double stdDev;

    // Loop aggregates the sum of the squares of each element in series
    for(int index = 0; index < series.size(); index++)
    {
        squareSum += pow((mean - series.at(index)), 2);
    }

    // The variance of a sequence is the sum of the squares of the elements
    // of the sequence divided by the number of elements
    variance = squareSum / series.size();
    // The standard deviation of a sequence is the square root of the variance
    stdDev = sqrt(variance);

    return stdDev;
}

double std_dev_dyn_array(double *series, const int seriesLen, const double mean)
{
    double squareSum = 0;
    double variance;
    double stdDev;

    // Loop aggregates the sum of the squares of each element in series
    for(int index = 0; index < seriesLen; index++)
    {
        squareSum += pow((mean - series[index]), 2);
    }

    // The variance of a sequence is the sum of the squares of the elements
    // of the sequence divided by the number of elements
    variance = squareSum / seriesLen;
    // The standard deviation of a sequence is the square root of the variance
    stdDev = sqrt(variance);

    return stdDev;
}

double std_dev_static_array(double series[], const int seriesLen, const double mean)
{
    double squareSum = 0;
    double variance;
    double stdDev;

    // Loop aggregates the sum of the squares of each element in series
    for(int index = 0; index < seriesLen; index++)
    {
        squareSum += pow((mean - series[index]), 2);
    }

    // The variance of a sequence is the sum of the squares of the elements
    // of the sequence divided by the number of elements
    variance = squareSum / seriesLen;
    // The standard deviation of a sequence is the square root of the variance
    stdDev = sqrt(variance);

    return stdDev;
}

void print_double(const double value, string intro)
{
    cout << intro << " " << value << endl;
}
