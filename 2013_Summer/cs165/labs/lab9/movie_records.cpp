// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               08/06/2013 06:30:11 AM
//  Last Modification Date:
//  Filename:                   movie_records.cpp
//
//  Overview:
//      This program stores various information concerning movies
//
//  Input:
//      The user is prompted to select a number of movies to enter, then enters information
//      about those movies.  The user then can select whether to view the titles all movies,
//      or see only the movies in a specific genre.  The user can then choose whether to see
//      more information about a movie, or rent a movie.  The program then loops if the user
//      chooses to do so, or exits.
//
//      The program also reads a text file containing movie information prior to soliciting
//      user input.
//
//  Output:
//
//      The program outputs movie information to the console based upon the user's choice.
//      The program also stores movie information entered by the user in a text file.
//
//      How many movies would you like to enter?:  1
//
//      Please enter the following information for movie number 1:
//      Title:  Blues Brothers
//      Number of copies:  3
//      Rating:  PG-13
//      Description:  Elroy and Jake Blues are on a mission from God.
//      Genre:  Comedy
//
//      Would you like to see all movies listed by title, or see movies in a particular genre?
//      1.  All movies
//      2.  By genre
//      Enter 1 or 2 (default: all movies):  2
//
//      We have movies in the following genres:
//      1.  Action
//      2.  Sci-fi
//      3.  Comedy
//      4.  ???
//      5.  Animation
//      Enter a number:  2
//
//      All movies in genre Sci-fi:
//      1.  Blade Runner
//
//      Would you like to:
//      1.  See more information about a movie?
//      2.  Rent a movie?
//      Choose a number (default: exit):  1
//
//      Select a movie to view more information about:
//      1.  Blade Runner
//      Please choose a movie by number:  1
//
//      Title:  Blade Runner
//      Number of copies:  2
//      Rating:  R
//      Description:  A sci-fi noir classic starring Harrison Ford
//      Genre:  Sci-fi
//
//      Would you like to:
//      1.  Start over?
//      2.  Exit?
//      Choose a number (default: exit):  2
//
// =====================================================================================

#include<iostream>      // for cout and cin
#include<string>        // for string functions including find_first_not_of()
#include<sstream>       // for stringstream
#include<fstream>       // for ifstream and ofstream
#include<cstdlib>       // for
#include<vector>        // for vector functions including push_back()
#include<algorithm>     // for transform() and sort()

#define NUM_MOVIES 5
#define MOVIE_LIST "movie_list.txt"

using namespace std;

// The following class defines various attributes of movies as would
// be relevant to someone trying to find a movie to watch.
//
// It includes a member function for clearing variables, and also
// overloads the '<<' and '>>' operators
class movie_record
{
public:

    string title;       // title of movie
    int copies;         // number of copies in stock
    string rating;      // MPAA rating
    string description; // A brief blurb about the movie
    string genre;       // genre of movie

    // empties out variables
    void clear_vars()
    {
        title = "";
        copies = 0;
        rating = "";
        description = "";
        genre = "";
    }

    friend ostream& operator<< (ostream &out, const movie_record &movie);
    friend istream& operator>> (istream&, movie_record &movie);
};

// The following two functions were adapted from the tutorial posted by user Bench at
// http://www.dreamincode.net/forums/topic/145699-eof-and-reading-text-files-c/
//
// They also make use of the tutorial posted by user Alex at
// http://www.learncpp.com/cpp-tutorial/93-overloading-the-io-operators/
//
// Purpose: overloads the << operator for use with streams
// Entry:   lefthand argument is an output stream, righthand argument is a
//          variable of class movie_record
// Exit:    contents of movie_record variable piped to stream
ostream& operator<< (ostream &out, const movie_record &movie);

// Purpose: overloads the >> for use with streams
// Entry:   righthand argument is an input stream, lefthand argument is a
//          variable of class movie_record
// Exit:    contents of input stream piped into movie_record variable
istream& operator>> (istream &input, movie_record &movie);

// Purpose: opens and reads into a vector a text file containing information in
//          the format of a movie_record variable
// Entry:   argument is a vector containing variable of type movie_record; vector
//          can be empty
// Exit:    vector has been filled with variable of type movie_record with contents
//          taken from text file
void parse_movie_list_file(vector<movie_record> &records);

// Purpose: reads contents of vector containing variables of type movie_record
//          into a text file
// Entry:   argument is a vector containing variables of type movie_record; vector
//          can be empty.
// Exit:    text file now contains information in the format of movie_record variables
void write_movie_list_file(vector<movie_record> &records);

// Purpose: Allows user to input information about movies
// Entry:   records is a vector containing variables of type movie_record; vector may be
//          empty.  movieNum is the number of movies the user wishes to enter
void set_movies(vector<movie_record> &records, int movieNum);

// Purpose: returns an integer for use with switch statements
// Entry:   lowerBound is the lowest number in the switch statement; upperBound is the highest.
//          prompt contains the prompt soliciting user input.  allowDefault allows the return
//          of a default value if the user does not enter a value.
// Exit:    returns the user's choice of integer, or returns default value in absence of user choice
int get_choice_num(const int lowerBound, const int upperBound, const string prompt, const bool allowDefault);

// This function overloads the above function name, and calls the function with allowDefault == false.
int get_choice_num(const int lowerBound, const int upperBound, const string prompt);

// Purpose: determines which title of two variables of type movie_record comes first alphabetically
// Entry:   firstRec and secondRec are variable of type movie_record
// Exit:    returns true if first title comes first alphabetically, false otherwise
bool sort_by_title(const movie_record &firstRec, const movie_record &secondRec);

// Purpose: outputs to console the titles of all movies in vector, and returns vector containing
//          these movies
// Entry:   records is an array containing variables of type movie_record
// Exit:    outputs titles of all movies in vector, and returns vector containing
//          these movies
vector<movie_record> get_movies(const vector<movie_record> records);

// Purpose: outputs to the console the titles of all movies in specified genre, and returns
//          vector containing these movies
// Entry:   records is a vector containing variables of type movie_record, and genre is
//          a string containing a movie genre
// Exit:    outputs to the console the titles of all movies in specified genre, and returns
//          a vector containing those movies
vector<movie_record> get_movies(const vector<movie_record> records, const string genre);

// Purpose: displays detailed info on user's chosen movie
// Entry:   records is a vector containing variables of type movie_record; choice
//          contains the index in the vector of the movie whose information will be displayed
// Exit:    prints to the console detailed information on the user's chosen movie
void get_movie_info(const vector<movie_record> records, int choice);

// Purpose: allows the user to rent a movie
// Entry:   records is a vector containing variables of type movie_record; choice is the index
//          in the vector of the movie to be rented
// Exit:    decrements the chosen movie's .copies variable by 1
void rent_movie(vector<movie_record> &records, int choice);

int main()
{

    int movieNum;       // Number of movies to enter
    int movieIndex;     // index of movie in vector records
    int sortChoice;     // holds choice whether to sort by name or by genre
    int genreChoice;    // holds choice of genre
    int rentChoice;     // holds choice of which movie to rent
    int rentOrInfo;     // holds choice whether to rent or get more info
    int contChoice;     // holds choice whether to restart program or exit
    string genre;       // genre of movie
    vector<movie_record> records;       // holds all movie_record variables
    vector<movie_record> userChoices;   // holds return value of get_movies()
    vector<string> genreList;           // holds list of genres of movies in stock

    parse_movie_list_file(records);

    cout << endl;
    movieNum = get_choice_num(0, NUM_MOVIES, "How many movies would you like to enter?:  ");

    set_movies(records, movieNum);

    sort(records.begin(), records.end(), sort_by_title);

    reinit:

    cout << "Would you like to see all movies listed by title, or see movies in a particular genre?" << endl;
    cout << "1.  All movies" << endl;
    cout << "2.  By genre" << endl;
    sortChoice = get_choice_num(1, 2, "Enter 1 or 2 (default: all movies):  ", true);
    cout << endl;

    switch(sortChoice)
    {
        case 1:
        {
            userChoices = get_movies(records);
            break;
        }
        case 2:
        {
            // loop through movies in records vector, adding genre of movie to genreList
            // unless genreList already contains that genre
            for(int index = 0; index < records.size(); index++)
            {
                for(int subIndex = 0; subIndex < genreList.size(); subIndex++)
                    if(records.at(index).genre == genreList.at(subIndex))
                        break;
                genreList.push_back(records.at(index).genre);
            }

            cout << "We have movies in the following genres: " << endl;
            for(int index = 0; index < genreList.size(); index++)
                cout << index+1 << ".  " << genreList.at(index) << endl;

            genreChoice = get_choice_num(1, genreList.size(), "Enter a number:  ");
            cout << endl;
            userChoices = get_movies(records, genreList.at(genreChoice-1));
            break;
        }
        default:
        {
            userChoices = get_movies(records);
        }
    }

    cout << endl;
    cout << "Would you like to:" << endl;
    cout << "1.  See more information about a movie?" << endl;
    cout << "2.  Rent a movie?" << endl;
    rentOrInfo = get_choice_num(1, 2, "Choose a number (default: exit):  ", true);
    cout << endl;

    switch(rentOrInfo)
    {
        case 1:
        {
            cout << "Select a movie to view more information about:";
            for(int index = 0; index < userChoices.size(); index++)
                cout << endl << index+1 << ".  " << userChoices.at(index).title;
            cout << endl;
            movieIndex = get_choice_num(1, userChoices.size(), "Please choose a movie by number:  ");
            get_movie_info(userChoices, movieIndex);
            break;
        }
        case 2:
        {
            cout << "Select a movie to rent:";
            for(int index = 0; index < userChoices.size(); index++)
                cout << endl << index+1 << ".  " << userChoices.at(index).title;
            cout << endl;
            rentChoice = get_choice_num(1, userChoices.size(), "Please choose a movie by number:  ");
            rent_movie(userChoices, rentChoice);
            break;
        }
        default:
        {
            cout << endl;
            return 0;
        }
    }

    cout << "Would you like to:" << endl;
    cout << "1.  Start over?" << endl;
    cout << "2.  Exit?" << endl;
    contChoice = get_choice_num(1, 2, "Choose a number (default: exit):  ", true);

    switch(contChoice)
    {
        case 1:
        {
            goto reinit;
            break;
        }
        case 2:
            break;
        default:
            break;
    }

    write_movie_list_file(records);
    cout << endl;
    return 0;
}

ostream& operator<< (ostream &out, const movie_record &movie)
{
    // outputs each movie characteristic on new line
    out << movie.title << endl << movie.copies << endl << movie.rating << endl << movie.description << endl << movie.genre << endl;
    return out;
}

istream& operator>> (istream &input, movie_record &movie)
{
    using std::getline;

    // each line of input entered into specified variable
    getline(input, movie.title, '\n');
    input >> movie.copies;
    input.ignore(10000, '\n');
    getline(input, movie.rating, '\n');
    getline(input, movie.description, '\n');
    getline(input, movie.genre, '\n');
    return input;
}

void parse_movie_list_file(vector<movie_record> &records)
{
    movie_record input;

    ifstream movie_list(MOVIE_LIST);

    // raise error if file not open
    if(!movie_list.is_open())
        cout << endl << "Could not open file!" << endl << endl;

    // before end of stream, adds info in stream to vector
    while(movie_list >> input)
        records.push_back(input);
}

void write_movie_list_file(vector<movie_record> &records)
{
    ofstream movie_list("movie_list.txt");

    // raise error if file not open
    if(!movie_list.is_open())
        cout << endl << "Could not open file!" << endl << endl;

    // for each movie_record in vector, output to movie_list
    // text file
    for(int index = 0; index < records.size(); index++)
        movie_list << records.at(index);
}

void set_movies(vector<movie_record> &records, int movieNum)
{
    string copies;
    int copyNum;
    movie_record movie;

    for(int index = 0; index < movieNum; index++)
    {
        // prompt for and read all movie attributes
        cout << endl << "Please enter the following information for movie number " << index+1 << ":" << endl;
        cout << "Title:  ";
        getline(cin, movie.title);
        cout << "Number of copies:  ";
        getline(cin, copies);
        stringstream ss(copies);
        ss >> copyNum;
        movie.copies = copyNum;
        cout << "Rating:  ";
        getline(cin, movie.rating);
        cout << "Description:  ";
        getline(cin, movie.description);
        cout << "Genre:  ";
        getline(cin, movie.genre);
        cout << endl;

        // store movie in vector, then clear movie for next loop around
        records.push_back(movie);
        movie.clear_vars();
    }
}

bool sort_by_title(const movie_record &firstRec, const movie_record &secondRec)
{
    // compares strings to see which comes first alphabetically.
    // returns true if the left come first, false if the right comes first
    return firstRec.title < secondRec.title;
}

int get_choice_num(const int lowerBound, const int upperBound, const string prompt, const bool allowDefault)
{
    string choice;
    int choiceNum;

    cout << prompt;
    while(getline(cin, choice))
    {
        stringstream ss(choice);
        ss >> choiceNum;
        if(choiceNum >= lowerBound && choiceNum <= upperBound)
            break;
        if(choice.length() == 0 && allowDefault == true)
        {
            choiceNum = -1; // arbitrary value unlikely to be used in switch statement
            break;
        }
        cout << "That is not a valid selection." << endl;
        cout << prompt;
    }
    return choiceNum;
}

int get_choice_num(const int lowerBound, const int upperBound, const string prompt)
{
    return get_choice_num(lowerBound, upperBound, prompt, false);
}

vector<movie_record> get_movies(const vector<movie_record> records)
{
    cout << "All movies, by title: " << endl;
    // loop through all movie_records and output to console
    for(int index = 0; index < records.size(); index++)
        cout << index+1 << ". " << records.at(index).title << endl;
    return records;
}

vector<movie_record> get_movies(const vector<movie_record> records, string genre)
{
    int counter = 1;        // holds number of movies in genre
    vector<movie_record> inGenre;

    cout << "All movies in genre " << genre << ":" << endl;
    for(int index = 0; index < records.size(); index++)
    {
        if(genre == records.at(index).genre)
        {
            inGenre.push_back(records.at(index));
            cout << counter << ".  " << records.at(index).title << endl;
            counter++;
        }
    }
    return inGenre;
}

void get_movie_info(vector<movie_record> records, int choice)
{
    choice--;
    cout << endl << "Title:  " << records.at(choice).title << endl;
    cout << "Number of copies:  " << records.at(choice).copies << endl;
    cout << "Rating:  " << records.at(choice).rating << endl;
    cout << "Description:  " << records.at(choice).description << endl;
    cout << "Genre:  " << records.at(choice).genre << endl << endl;
}

void rent_movie(vector<movie_record> &records, int choice)
{
    choice--;
    records.at(choice).copies--;
}
