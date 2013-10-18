// =====================================================================================
//  Author:                     Matthew Schreiber
//  Date Created:               07/31/2013 03:47:54 PM
//  Last Modification Date:     08/04/2013 10:13:55 PM
//  Filename:                   pig_latin.cpp
//
//  Overview:
//      This program takes a string entered as a command line argument and translates
//      it into Pig Latin
//
//  Input:
//      The program is invoked in the form './pig_latin "<string" '.  The string must
//      be enclosed in quotation marks.
//
//  Output:
//      (Success) Prints the translated string to the console.  Words beginning with a
//      consonant have the first letter moved to the back of the word, and 'ay' appended
//      after that.  Words beginning with a vowel have 'way' appended.
//      (Failure) Prints message containing proper usage to the console, then exits.
//
//  Sample output:
//
//  ./pig_latin "He took it without so much as a by-your-leave!"
//
//  You entered "he took it without so much as a by-your-leave!"
//  Translated to Pig Latin, that's "ehay ooktay itway ithoutway osay uchmay asway away
//  byay-ouryay-eavelay!"
//
// =====================================================================================

#include<iostream>  // for cout and cin
#include<string>    // for find_first_of() and find_first_not_of()
#include<sstream>   // for stringstream()
#include<cstddef>   // for size_t
#include<cstdlib>   // for exit()
#include<cctype>    // for isalpha()
#include<vector>    // for various vector functions
#include<algorithm> // for transform()

using namespace std;

struct word_parts
{
    string beginPunc;
    string endPunc;

    vector<string> wordEnds;
    vector<string> pigBlocks;
    vector<string> hyphens;
    vector<int> hyphenLocs;

    void clear_vars()
    {
        beginPunc = "";
        endPunc = "";

        wordEnds.clear();
        pigBlocks.clear();
        hyphens.clear();
        hyphenLocs.clear();
    }
};

// Credit for the following two functions is due to Evan Teran, who posted them at
// http://stackoverflow.com/questions/236129/splitting-a-string-in-c
//
// Purpose: Split a string into separate words based upon a chosen delimiter
// Entry:    str is any string object, delimiter is a char such as ',' or ' '
//          (although it can be any char), and wordGroup is a vector of strings
// Exit:    returns a vector full of individual words
vector<string> &split(const string &str, char delim, vector<string> &wordGroup);

// Overloaded function; calls the above function
vector<string> split(const string &str, char delim);

// Purpose: Culls punctuation from the beginning and end of a word,
//          storing it in, respectively, word.beginPunc and
//          word.endPunc
// Entry:   str is any string object, word is any word_parts object
// Exit:    word.beginPunc and word.endPunc now contain strings
//          of punctuation marks, or an empty string if there was no
//          punctuation at the beginning/end of the passed string.
//          Any beginning and ending punctuation is now gone from str.
void store_punc(string &str, word_parts &word);

// Purpose: Parses a string to find the locations of any hyphens;
//          stores these locations in the word.hyphenLoc vector
// Entry:   str is any string object, word is any word_parts object
// Exit:    word.hyphenLoc now contains all hyphen indices in str
void get_hyphen_locs(const string &str, word_parts &word);

// Purpose: Parses a string, identifying and storing the various
//          elements needed to translate the string into Pig Latin
// Entry:   str is any string object (should contain an actual word,
//          but there's no restriction on content), word is any word_parts
//          object
// Exit:    word.wordEnds, word.pigBlocks, and word.hyphens vectors now contain
//          at least one element.  This may be an empty string, based on the
//          content of the string passed.
void store_pig_elements(const string &str, word_parts &word);

int main(int argc, char **argv)
{
    string wordStr;         // for holding individual words culled from the command line argument
    string tempStr;         // for holding translated words to be added to pigPhrase
    string pigPhrase = "";  // the final, translated phrase
    string usage = "Usage: ./pig_latin \"<string>\"";

    word_parts transWord;   // create an instance of word_parts

    // Check to make sure there's only one command line argument (other than the program name)
    // This is one of only two controls helping to ensure proper input, i.e. ./pig_latin "<string>"
    if(argc != 2)
    {
        cout << endl << usage << endl << endl;
        exit(1);
    }

    // assign the second command line argument to start string, then
    // convert all letters to lowercase, then split startString
    // into component words and store them in wordVect
    string startStr = string(argv[1]);
    transform(startStr.begin(), startStr.end(), startStr.begin(), ::tolower);
    vector<string> wordVect = split(startStr, ' ');

    // Check to make sure there are at least SOME letters in the command line
    // argument
    if(startStr.find_first_of("abcdefghijklmnopqrstuvwxyz") == string::npos)
    {
        cout << endl << usage << endl << endl;
        exit(1);
    }

    // loop through each word in wordVect
    for(int wordI = 0; wordI < wordVect.size(); wordI++)
    {
        transWord.clear_vars();     // clear out transWord each loop
        tempStr = "";
        wordStr = wordVect.at(wordI);

        store_punc(wordStr, transWord);
        get_hyphen_locs(wordStr, transWord);
        store_pig_elements(wordStr, transWord);

        // add punctuation and content to tempStr, arranged so that it comes
        // out as valid Pig Latin
        tempStr += transWord.beginPunc;
        for(int vectI = 0; vectI < transWord.wordEnds.size(); vectI++)
        {
            tempStr += transWord.wordEnds.at(vectI) +  transWord.pigBlocks.at(vectI) +  transWord.hyphens.at(vectI);
        }
        tempStr += transWord.endPunc;

        // add a space to the end if not the last word in the original string
        if (wordI == wordVect.size()-1)
            pigPhrase += tempStr;
        else
            pigPhrase += tempStr + " ";
    }

    cout << endl << "You entered \"" << startStr << '\"' << endl;
    cout << "Translated to Pig Latin, that's \"" << pigPhrase << '\"' << endl << endl;

    return 0;
}

vector<string> &split(const string &str, char delim, vector<string> &wordGroup)
{
    stringstream ss(str);   // copy str into buffer
    string ownWord;

    // copy words from stream into ownWord, delimiting separate words
    // at each instance of delim
    while (getline(ss, ownWord, delim))
    {
        wordGroup.push_back(ownWord);
    }

    return wordGroup;
}


vector<string> split(const string &str, char delim)
{
    vector<string> wordGroup;

    split(str, delim, wordGroup);

    return wordGroup;
}

void store_punc(string &str, word_parts &word)
{
    // get the indices of the first and last letters
    size_t firstLtr = str.find_first_of("abcdefghijklmnopqrstuvwxyz");
    size_t lastLtr = str.find_last_of("abcdefghijklmnopqrstuvwxyz");

    // if there's punctuation at the beginning, store it and cut it
    if(firstLtr != 0)
    {
        word.beginPunc = str.substr(0, firstLtr);
        str.erase(0, firstLtr-1);
    }

    // likewise for punctuation at the end
    if(lastLtr < (str.length()-1))
    {
        word.endPunc = str.substr(lastLtr+1, str.length()-lastLtr);
        str.erase(lastLtr+1, str.length()-lastLtr);
    }
}

void get_hyphen_locs(const string &str, word_parts &word)
{
    // check whether character at index is a hyphen; if so,
    // store the character's index
    for(int index = 0; index < str.length()-1; index++)
    {
        if(str.at(index) == '-')
            word.hyphenLocs.push_back(index);
    }

    // store a hyphen location at the end to make things easier
    // when looping through the word_parts elements to reconstruct
    // the word (helps to prevent out-of-bounds problems)
    word.hyphenLocs.push_back(str.length());
}

void store_pig_elements(const string &str, word_parts &word)
{
    int prevIndex = 0;  // stores index of beginning of string, or index just
                        // after a hyphen instance
    size_t firstVowel;  // location of first value

    int hyphenIndex;    // index of next hyphen
    int endIndex;       // index of last letter before hyphen

    string tempStr;     // stores substring from beginning to hyphen, or
                        // between hyphens, or from last hyphen to end-of-word
    string pigBlock;    // stores first consonants + "ay", or just "way"
    string wordEnd;     // stores the last part of the word, after which pigBlock
                        // will be appended

    for(int index = 0; index < word.hyphenLocs.size(); index++)
    {
        hyphenIndex = word.hyphenLocs.at(index);
        endIndex = hyphenIndex-1;

        tempStr = str.substr(0, hyphenIndex);
        pigBlock.erase(0, pigBlock.length());
        wordEnd.erase(0, wordEnd.length());

        firstVowel = tempStr.find_first_of("aeiou", prevIndex);

        // store empty string at end of hyphen vector if end-of-word
        // has been reached, otherwise store a hyphen
        if(word.hyphenLocs.at(index) == str.length())
            word.hyphens.push_back("");
        else
            word.hyphens.push_back(str.substr(hyphenIndex, 1));

        // if there are vowels, enter conditional, otherwise, enter else
        if(firstVowel != string::npos)
        {
            // when the first letter of tempStr is a vowel, store "way" in
            // pigBlocks, and store the whole tempStr in wordEnds
            if(firstVowel == prevIndex)
            {
                pigBlock = "way";
                wordEnd = tempStr.substr(firstVowel, hyphenIndex-firstVowel);
                word.pigBlocks.push_back(pigBlock);
                word.wordEnds.push_back(wordEnd);
            }
            // if there's a qu, store it + "ay" in pigBlocks and store the rest
            // in wordEnds
            else if(str.at(firstVowel) == 'u' && str.at(firstVowel-1) == 'q')
            {
                pigBlock = tempStr.substr(prevIndex, 1+firstVowel-prevIndex) + "ay";
                wordEnd = tempStr.substr(firstVowel+1, hyphenIndex-(firstVowel+1));
                word.pigBlocks.push_back(pigBlock);
                word.wordEnds.push_back(wordEnd);
            }
            // store opening consonant block + "ay" in pigBlocks and store the rest
            // in wordEnds
            else
            {
                pigBlock = tempStr.substr(prevIndex, firstVowel-prevIndex) + "ay";
                wordEnd = tempStr.substr(firstVowel, hyphenIndex-firstVowel);
                word.pigBlocks.push_back(pigBlock);
                word.wordEnds.push_back(wordEnd);
            }
        }
        // if there are no vowels, store the whole tempStr in pigBlocks with "ay"
        // appended, and store an empty string in wordEnds
        else
        {
            pigBlock = tempStr.substr(prevIndex, firstVowel-prevIndex) + "ay";
            word.pigBlocks.push_back(pigBlock);
            word.wordEnds.push_back("");
        }

        prevIndex = hyphenIndex+1;
    }
}
