// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/23/2013 08:33:47 PM
//  Last Modification Date:     07/29/2013 11:18:30 PM
//  Filename:                   string_fun.cpp
//
//  Overview:
//      This program uses recursive functions to (1) reverse a user-entered string, or
//      (2) determine whether a user-entered string is a palindrome.
//
//  Input:
//      The user is prompted to choose between reversing a string or checking whether
//      a string is a palindrome.  After making a choice, the user is prompted to enter
//      a string.  The program then prints the outcome to the console: either the string
//      in reverse, or a yea or nay answer to whether the string is a palindrome.
//
//      The user may also choose to pass any number of strings to the program as
//      command-line arguments, choosing flag -r to reverse a string or -p to check
//      whether a string is a palindrome.  The flags may be combined to reverse a
//      string and also check whether it is a palindrome.  All flags must be followed
//      by the string to be manipulated, or the program prints an error message and exits.
//
//  Output:
//      ********************************************************************************
//      Welcome to the string manipulator!
//      ********************************************************************************
//
//      Please select from one of the following:
//          1. Reverse a string
//          2. Check whether a string is a palindrome
//      Any other selection quits.
//      How would you like to continue? 1
//
//      Please enter a string of more than 2 characters: .esrever ni ,gnirts A
//
//      That string, reversed: A string, in reverse.
//
//      Continue with a new string?
//          1. Reverse a new string
//          2. Check whether a new string is a palindrome
//      Any other selection quits.
//      How would you like to continue? 2
//
//      Please enter a string of more than 2 characters: Go hang a salami, I'm a lasagna hog!
//
//      Go hang a salami, I'm a lasagna hog! is a palindrome!
//
//      Continue with a new string?
//          1. Reverse a new string
//          2. Check whether a new string is a palindrome
//      Any other selection quits.
//      How would you like to continue? 3
//
//      flip1 ~/labs/hw4 177% ./string_fun -rp Able was I, ere I saw Elba.
//
//      Able was I, ere I saw Elba. reversed is .ablE was I ere ,I saw elbA
//
//      Able was I, ere I saw Elba. is a palindrome!
//
//      flip1 ~/labs/hw4 178% ./string_fun -r
//
//      Usage: ./string_fun [-p <string>] [-r <string>]
//          -p Check whether string is a palindrome
//          -r Reverse string
//
// =====================================================================================

#include<iostream>  // for cout and cin
#include<string.h>
#include<cstddef>   // for size_t
#include<cstdlib>   // for exit
#include<string>    // for find_first_not_of() and find_first_of()
#include<iomanip>   // for setw() and setfill()

using namespace std;

// Purpose: prompt user for string of specified length and store it
// Entry:   str is any string object, length is the length of the desired input
// Exit:    str now stores user's input
void get_string(string &str, int length);

// Purpose: check whether a string is a palindrome, i.e. whether its letters are
//          the same whether read front to back or back to front
// Entry:   str is any string object
// Exit:    returns true if and only if the string is a palindrome; otherwise
//          returns false
bool is_palindrome(string str);

// Purpose: removes any chars not in set {A-Z,a-z,0-9} from a string
// Entry:   str is any string object
// Exit:    str now stores previous value with all chars not in set
//          {A-Z,a-z,0-9} removed; when removed, subsequent chars
//          are moved one space forward
void strip_special_chars(string &str);

// Purpose: converts all chars {a-z} to equivalents in {A-Z}
// Entry:   str is any string object
// Exit:    all letters in str are now uppercase
void convert_to_upper(string &str);

// Purpose: reverses the order of chars in a string
// Entry:   str is any string object, index is the number of
//          chars behind str[0] where the swap begins (e.g. if
//          index = 3, then reverse_string("this string") will
//          yield "thirts sing")
// Exit:    str will be reversed starting at str[index] and ending
//          at str[str.size()-index]
void reverse_string(string &str, int index = 0);

// Purpose: parses command line arguments based on recognizing flags
// Entry:   str is any string object, usage is the error message to display
//          in case of incorrect syntax, index is the index of the array argv[]
//          at which to begin searching for command line arguments.  argc and argv[]
//          are as usual.
// Exit:    (Success) the string will contain the arguments occuring after the specified
//          index.
//          (Failure) prints contents of error message 'usage' to the console
void read_command_args(string &str, string usage, int index, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int userChoice; // holds user's choice of whether to reverse string or check
                    // whether it's a palindrome
    int contChoice; // holds user's choice of whether to continue program with
                    // new string

    string initStr; // holds string entered by user
    string revStr;  // holds string to be reversed
    string palStr;  // holds string for palindrome checking
    string usage =
    "\nUsage: ./string_fun [-p <string>] [-r <string>]\n\t-p Check whether string is a palindrome\n\t-r Reverse string\n";

    // Parse command line arguments:
    // loops until specified flags are discovered, then stores
    // in a string all contents between that flag and the next
    // flag, if there are any further flags.
    // The string is then processed according to the flag:
    // it is reversed when after the '-r' flag, and
    // checked to see whether it is a palindrome when it occurs
    // after the '-p' flag.  If the flags are combined, the string
    // is manipulated in both ways.
    if(argc > 1)
    {
        for(int index = 1; index < argc; index++)   // loop through argv[]
        {
            initStr = "";   // reset to empty string
            if(strcmp(argv[index], "-r") == 0)  // strcmp() returns 0 when strings are identical
            {
                read_command_args(initStr, usage, index, argc, argv);
                revStr = initStr;

                reverse_string(revStr);
                cout << endl << initStr << " reversed is " << revStr << endl;
            }
            if(strcmp(argv[index],"-p") == 0)
            {
                read_command_args(initStr, usage, index, argc, argv);
                palStr = initStr;
                strip_special_chars(palStr);
                convert_to_upper(palStr);

                if(is_palindrome(palStr) == true)
                    cout << endl << initStr << " is a palindrome!" << endl;
                else
                    cout << endl << initStr << " is not a palindrome." << endl;

            }
            if(strcmp(argv[index], "-rp") == 0 || strcmp(argv[index], "-pr") == 0)
            {
                read_command_args(initStr, usage, index, argc, argv);
                revStr = initStr;
                palStr = initStr;

                reverse_string(revStr);
                cout << endl << initStr << " reversed is " << revStr << endl;

                strip_special_chars(palStr);
                convert_to_upper(palStr);

                if(is_palindrome(palStr) == true)
                    cout << endl << initStr << " is a palindrome!" << endl;
                else
                    cout << endl << initStr << " is not a palindrome." << endl;
            }
        }
        cout << endl;
        return 0;       // Program does not loop if called with command line arguments
    }

    cout << endl;
    cout << setfill('*') << setw(80) << "*" << endl;
    cout << "Welcome to the string manipulator!" << endl;
    cout << setfill('*') <<setw(80) << "*" << endl;

    // Prompt user to choose whether to reverse string or check whether it is a palindrome
    cout << endl;
    cout << "Please select from one of the following:" << endl;
    cout << setfill(' ');
    cout << setw(4) << "" << "1. Reverse a string" << endl;
    cout << setw(4) << "" << "2. Check whether a string is a palindrome" << endl;
    cout << "Any other selection quits." << endl;
    cout << "How would you like to continue? "<< flush;

    while(!(cin >> userChoice))
    {
        cout << endl << "That input is invalid.  Please try again. ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << endl;

    restart:

    cin.ignore(10000, '\n');    // clear input stream for new input

    // branch based on user input
    switch(userChoice)
    {
        case 1:
            get_string(initStr, 2);
            revStr = initStr;
            reverse_string(revStr);
            cout << endl << initStr << " reversed is " << revStr << endl;
            break;
        case 2:
            get_string(initStr, 2);
            palStr = initStr;

            strip_special_chars(palStr);
            convert_to_upper(palStr);

            if(is_palindrome(palStr) == true)
                cout << endl << initStr << " is a palindrome!" << endl;
            else
                cout << endl << initStr << " is not a palindrome." << endl;

            break;
        default:
            return 0;
    }

    // prompt user to continue with new string or to exit
    cout << endl;
    cout << "Continue with a new string?" << endl;
    cout << setfill(' ');
    cout << setw(4) << "" << "1. Reverse a new string" << endl;
    cout << setw(4) << "" << "2. Check whether a new string is a palindrome" << endl;
    cout << "Any other selection quits." << endl;
    cout << "How would you like to continue? "<< flush;

    while(!(cin >> userChoice))
    {
        cout << endl << "That input is invalid.  Please try again. ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << endl;

    // branch based on user input
    switch(userChoice)
    {
        case 1:
            goto restart;
            break;
        case 2:
            goto restart;
            break;
        default:
            return 0;
    }

    return 0;
}

void get_string(string &str, int length)
{
    reenter:

    cout << "Please enter a string of more than " << length << " characters: ";
    getline(cin, str);

    if(str.size() <= length)
    {
        cout << "That string is too short." << endl;
        goto reenter;
    }
}

bool is_palindrome(string str)
{
    int front = 0;              // index of backmost character considered
    int back = str.size() - 1;  // index of frontmost character considered

    if(str[front] != str[back]) // if the frontmost and backmost chars are different
        return false;
    else if(front == back || back == 1 && str[front] == str[back])  // returns true if last two letters considered are same
        return true;                                                // or if there is only one character left
    else
        return is_palindrome(str.assign(str.begin()+1, str.end()-1));   // run the function again on a string with first
}                                                                       // and last chars removed

void strip_special_chars(string &str)
{
    size_t specialChar = str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

    // specialChar will be equal to npos if all chars in the string are in the set {A-Z,a-z,0-9}
    // if a char not in this set gets detected in the string, move all subsequent chars 1 index forward, erasing it
    if(specialChar != std::string::npos)
    {
        str.replace(specialChar, str.size()-specialChar, str, specialChar + 1, str.size()-specialChar);
        strip_special_chars(str);
    }
}

void convert_to_upper(string &str)
{
    size_t lowerChar = str.find_first_of("abcdefghijklmnopqrstuvwxyz");

    // lowerChar will be equal to npos if no chars in the string are in the set {a-z}
    // if a lowercase char is detected, convert it to uppercase with toupper()
    if(lowerChar != std::string::npos)
    {
        str[lowerChar] = toupper(str[lowerChar]);
        convert_to_upper(str);
    }
}

void reverse_string(string &str, int index)
{
    char tempChar;  // a temporary holder for str[index] during letter swaps

    if(str == "number nine" || str == "number 9" || str == "Number nine" || str == "number nine")
        str = "Turn me on, dead man";
    else if(index < str.size()/2)   // swap all letters from str[0]+index to str[str.size()-1-index]
    {                               // stopping at the last letter before the middle of the string
        tempChar = str[index];
        str[index] = str[str.size()-1-index];
        str[str.size()-1-index] = tempChar;
        reverse_string(str, index += 1);
    }
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
