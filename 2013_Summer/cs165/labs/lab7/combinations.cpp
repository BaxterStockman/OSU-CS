// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/23/2013 12:12:32 PM
//  Last Modification Date:
//  Filename:                   combinations.cpp
//
//  Overview:
//      This program is designed to elucidate differences in completion time between
//      a function that calculates the number of ways of choosing 'r' distinct items
//      from a set containing 'n' items using iteration and a function that accomplishes
//      the same task using recursion.
//
//  Input:
//      The user inputs the size of the number used as the base for the factorial
//      computation by passing a command line argument in the form
//      './combinations -n 20 -r 3', where -'n 20' signifies the cardinality of the set
//      and '-r 3' signifies the number of items to be chosen
//
//  Output:
//      The program prints to the console the time in seconds or microseconds it takes for
//      each function to complete.
//
// =====================================================================================

#include<iostream>
#include<sys/time.h>
#include<cstdlib>
#include<string.h>

//using std::cin;
//using std::cout;
//using std::endl;

using namespace std;

long double factor_recurse(long double n, long double r, long double nLessR);

long double factor_iterate(long double n, long double r);

bool check_input(const char *str);

void read_command_args(string &str, string usage, int index, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    long double choiceRecurse;
    long double choiceIterate;

    long double setItems;
    long double choiceItems;
    long double itemDiff;

    string initStr;

    bool nPres = false;
    bool rPres = false;

    string usage = "\nUsage: ./combinations [-n <number>] [-r <number>]\n\t-n Number of items in set\n\t-r Number of items to choose from set\n";

    if(argc > 1)
    {
        for(int index = 1; index < argc; index++)
        {
            initStr = "";
            if(strcmp(argv[index], "-n") == 0)
            {
                read_command_args(initStr, usage, index, argc, argv);
                setItems = atoi(initStr.c_str());
                nPres = true;
            }
            else if(strcmp(argv[index], "-r") == 0)
            {
                read_command_args(initStr, usage, index, argc, argv);
                choiceItems = atoi(initStr.c_str());
                rPres = true;
            }
        }
    }

    if(nPres == false)
    {
        cout << "How many items in the set? ";
        cin >> setItems;
    }

    if(rPres == false)
    {
        cout << "How many items to pick from the set? ";
        cin >> choiceItems;
    }

    itemDiff = setItems - choiceItems;

    typedef struct timeval time;
    time stop, start;

    gettimeofday(&start, NULL);
    choiceRecurse = factor_recurse(setItems, choiceItems, itemDiff);
    gettimeofday(&stop, NULL);

    cout << "The number of ways to choose " << choiceItems << " from " << setItems << " is " << choiceRecurse << endl;

    if(stop.tv_sec > start.tv_sec)
        cout << "It took " << stop.tv_sec-start.tv_sec << " seconds for the recursive function to return its result." << endl;
    else
    {
        cout << "It took " << stop.tv_usec-start.tv_usec
             << " microseconds for the recursive function to return its result." << endl;
    }

    gettimeofday(&start, NULL);
    choiceIterate = factor_iterate(setItems, choiceItems);
    gettimeofday(&stop, NULL);

    cout << "The number of ways to choose " << choiceItems << " from " << setItems << " is " << choiceIterate << endl;

    if(stop.tv_sec > start.tv_sec)
        cout << "It took " << stop.tv_sec-start.tv_sec << " seconds for the iterative function to return its result." << endl;
    else
    {
        cout << "It took " << stop.tv_usec-start.tv_usec
             << " microseconds for the iterative function to return its result." << endl;
    }

    return 0;
}

long double factor_recurse(long double n, long double r, long double nLessR)
{
    if (n > 1 && r > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, r-1, nLessR-1);
    else if(n > 1 && r > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, r-1, 1);
    else if (n > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, 1, nLessR-1);
    else if (n > 1)
        return (n / (r * (nLessR))) * factor_recurse(n-1, 1, 1);
    else if (r > 1 && nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, r-1, nLessR-1);
    else if (r > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, r-1, 1);
    else if (nLessR > 1)
        return (n / (r * (nLessR))) * factor_recurse(1, 1, nLessR-1);
    else
        return 1;
}

long double factor_iterate(long double n, long double r)
{
    long double choices = 1;

    for(long double rIndex = r; rIndex > 0; rIndex--)
    {
        choices /= rIndex;
    }
    for(long double diffIndex = n-r; diffIndex > 0; diffIndex--)
    {
        choices /= diffIndex;
    }
    for(long double nIndex = n; nIndex > 0; nIndex--)
    {
        choices *= nIndex;
    }
    return choices;
}

bool check_input(const char *str)
{
    if (strlen(str) == 0)    // check if string is empty or longer than two characters
        return false;
    while (*str < '0' || *str > '9')
    {
        return false;                           // returns false when character not a digit
        str++;                                  // move to next character
    }
    return true;
}

void read_command_args(string &str, string usage, int index, int argc, char *argv[])
{
    // make sure that what follows flag isn't another flag or out-of-range of argv[]
    if(index+1 < argc && (argv[index+1][0] != '-' || strlen(argv[index+1]) > 3))
    {
        // for all in argv[] after flag, add to str if not another flag or out-of-range
        // of argv[]
        for(int nextArg = index+1; nextArg < argc; nextArg++)
        {
            if(nextArg+1 != argc && (argv[nextArg+1][0] != '-' || strlen(argv[nextArg+1]) > 3))
            {
                str.insert(str.size(), argv[nextArg]);
                str.insert(str.size(), " ");
            }
            else
            {
                str.insert(str.size(), argv[nextArg]);
                break;
            }
        }
    }
    else
    {
        cout << usage << endl;
        exit(1);
    }
}
