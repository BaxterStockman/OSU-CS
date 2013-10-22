#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "dynamicArray.h"

// Use math.h's constants, e.g. M_PI and M_E
// #define _USE_MATH_DEFINES
// except it doesn't work...

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

#ifndef M_E
#define M_E 2.7182818284590452353602875
#endif

#ifndef TYPE
#define TYPE double
#endif

#define ERR_MSG(src, format, ...) sprintf(src, format, ##__VA_ARGS__)
#define DIE_ERR(src, format, ...) {         \
    ERR_MSG(src, format, ##__VA_ARGS__);    \
    die(src);                               \
}

static char *bin_format = "in '%g %s': %s\n%s";
static char *un_format = "in '%s': %s\n%s";
static char *bin_usage = "USAGE:\t./calc <number> <number> <binary operation>";
static char *un_usage = "USAGE:\t./calc <number> <unary operation>";
static char *db_usage = "USAGE:\t./calc <number> <number> <binary operation>\n"   \
                              "\t./calc <number> <unary operation>";
static char *insf_args = "Not enough arguments.";
static char *inval_in = "Invalid input";
static char *hng_ops = "Hanging operand(s)";
static char error [250];

int isfloat(char *str);
int isNumber(char *s, double *num);
int lowercase(char *s);
TYPE add (DynArr *stack);
TYPE subtract(DynArr *stack);
TYPE divide(DynArr *stack);
TYPE multiply(DynArr *stack);
TYPE power(DynArr *stack);
TYPE square(DynArr *stack);
TYPE cube(DynArr *stack);
TYPE absolute_value(DynArr *stack);
TYPE square_root(DynArr *stack);
TYPE exponential(DynArr *stack);
TYPE natural_logarithm(DynArr *stack);
TYPE logarithm(DynArr *stack);
int strtrim(char* s, char c);
char* floatf(double val);
int lenf(double val);
void die(const char *message);
double calculate(int numInputTokens, char **inputString);

int main(int argc , char** argv)
{
    // assume each argument is contained in the argv array
    // argc-1 determines the number of operands + operators
    if (argc == 1)
        DIE_ERR(error, un_format , *argv, insf_args, db_usage);

    // call the calculator function and store in result
    double result = calculate(--argc,++argv);

    printf("%g\n", result);

    return 0;
}

// ===  FUNCTION  ======================================================================
//         Name:    isfloat
//  Description:    param:      s = string
//                  returns:    true if s is in the form of a floating-point number
// =====================================================================================
int isfloat(char *str) {
    char c;
    int seen_pt = 0;    // have we encountered a decimal point?

    // check whether the first character is either a
    // digit or a minus sign
    if((c = *str++) && !(c == '-' || isdigit(c)))
        return 0;

    // iterate through string, exiting loop if we see
    // anything but digits and one (and only one)
    // decimal point
    while((c = *str++) && (isdigit(c) || (c == '.' && !seen_pt)))
        if(c == '.')
            seen_pt = 1;

    // if we've got to the null terminator, the input is a float
    if(c == '\0') return 1;

    // otherwise...
    return 0;
}

/* param: s the string
   param: num a pointer to double
   returns: true (1) if s is a number else 0 or false.
   postcondition: if it is a number, num will hold
   the value of the number
*/
int isNumber(char *s, double *num)
{
    char *end;
    double returnNum;

    if(strcmp(s, "0") == 0)
    {
        *num = 0;
        return 1;
    }
    else
    {
        returnNum = strtod(s, &end);
        /* If there's anything in end, it's bad */
        if((returnNum != 0.0) && (strcmp(end, "") == 0))
        {
            *num = returnNum;
            return 1;
        }
    }
    return 0;  //if got here, it was not a number
}

// ===  FUNCTION  ======================================================================
//         Name:    lowercase
//  Description:    param: s = string
//                  returns: true if s is converted to lowercase, false otherwise
//                  postcondition:  s has been converted to lowercase
// =====================================================================================
int lowercase(char *s) {
    for( ; *s != '\0'; s++)
        *s = tolower(*s);
    if(*s == '\0')
        return 1;
    return 0;
}

// ===  FUNCTION  ======================================================================
//         Name:    add
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    sum of the top two values on the stack
//                  post:       top two values of stack popped from stack
// =====================================================================================
TYPE add (DynArr *stack)
{
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return lhs + rhs;
}

// ===  FUNCTION  ======================================================================
//         Name:    subtract
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    difference between the top two values on the stack
//                  post:       top two values of stack popped from stack
// =====================================================================================
TYPE subtract(DynArr *stack)
{
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return lhs - rhs;
}

// ===  FUNCTION  ======================================================================
//         Name:    divide
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    quotient of the top two values on the stack
//                  post:       top two values of stack popped from stack
// =====================================================================================
TYPE divide(DynArr *stack)
{
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return lhs / rhs;
}

// ===  FUNCTION  ======================================================================
//         Name:    multiply
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    product of the top two values on the stack
//                  post:       top two values of stack popped from stack
// =====================================================================================
TYPE multiply(DynArr *stack)
{
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return lhs * rhs;
}

// ===  FUNCTION  ======================================================================
//         Name:    power
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    second-from-top value on stack raised to the power of
//                              the top value on the stack
//                  post:       top two values of stack popped from stack
// =====================================================================================
TYPE power(DynArr *stack)
{
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);

    return pow(lhs, rhs);
}

// ===  FUNCTION  ======================================================================
//         Name:    square
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    square of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE square(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return pow(lhs, 2);
}

// ===  FUNCTION  ======================================================================
//         Name:    cube
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    cube of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE cube(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return pow(lhs, 3);
}

// ===  FUNCTION  ======================================================================
//         Name:    absolute_value
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    absolute value of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE absolute_value(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return lhs >= 0 ? lhs : -1 * lhs;
}

// ===  FUNCTION  ======================================================================
//         Name:    square_root
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    square root of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE square_root(DynArr *stack) {
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return sqrt(lhs);
}

// ===  FUNCTION  ======================================================================
//         Name:    exponential
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    constant e raised to the power of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE exponential(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return exp(lhs);
}

// ===  FUNCTION  ======================================================================
//         Name:    natural_logarithm
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    natural logarigthm  of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE natural_logarithm(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return log(lhs);
}

// ===  FUNCTION  ======================================================================
//         Name:    logarithm
//  Description:    param:      stack = pointer to DynArr struct
//                  returns:    log base 10 of top value on stack
//                  post:       top value of stack popped from stack
// =====================================================================================
TYPE logarithm(DynArr *stack)
{
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    return log10(lhs);
}

// ===  FUNCTION  ======================================================================
//         Name:    strtrim
//  Description:    params:     s = string
//                              c = char to be removed from end of string
//                  returns:    number of trailing chars cut from string
//                  post:       all occurrences of c from end of string to first char
//                              that is not c are removed
// =====================================================================================
int strtrim(char* s, char c) {
    int cut = 0;
    char *p;
    for( ; (p = strrchr(s, c)); cut++)
        *p = '\0';
    return cut;
}

char* floatf(double val) {
    static char s[50];
    char *pt;

    // Store formatted result in result_str
    sprintf(s, "%f", val);

    // remove trailing zeroes from the part of
    // result_str starting at the decimal point
    strtrim(strchr(s, '.'), '0');

    // if there's nothing left after the decimal point,
    // remove it
    if((pt = (strrchr(s, '.'))) && *(pt+1) == '\0')
        *pt = '\0';
    return s;
}

// ===  FUNCTION  ======================================================================
//         Name:    lenf
//  Description:    params:     val = double-precision float
//                  returns:    the number of digits in val
// =====================================================================================
int lenf(double val) {
    int whole = (int)val;
    double rmd = val - whole;
    int len = 0;
    while((whole /= 10))
        len++;
    while((rmd /= (0.1)))
        len++;
    return len;
}

// ===  FUNCTION  ======================================================================
//         Name:    die
//  Description:    Prints error message, then exits
//                  params:     message = string
//                  returns:    nothing
//                  post:       program exits
// =====================================================================================
void die(const char *message) {
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

// ===  FUNCTION  ======================================================================
//         Name:    calculate
//  Description:    Implementation of Reverse Polish Notation calculator
//                  params:     numInputTokens = number of arguments to calculate
//                              inputString = point to array containing arguments
//                  returns:    Computed value of expression resulting from concatenation
//                              of values in inputString
// =====================================================================================
double calculate(int numInputTokens, char **inputString)
{
    int i;
    double result = 0.0;
    char c;
    char *s;
    DynArr *stack;

    //set up the stack
    stack = createDynArr(20);

    // start at 1 to skip the name of the calculator calc
    for(i=0;i < numInputTokens;i++)
    {
        s = inputString[i];
        c = s[0];

        if(isalpha(c))
            lowercase(s);

        // Hint: General algorithm:
        // (1) Check if the string s is in the list of operators.
        //   (1a) If it is, perform corresponding operations.
        //   (1b) Otherwise, check if s is a number.
        //     (1b - I) If s is not a number, produce an error.
        //     (1b - II) If s is a number, push it onto the stack

        if(c == '+') {
            if(sizeDynArr(stack) < 2)
                DIE_ERR(error, bin_format, topDynArr(stack), s, insf_args, bin_usage);
            pushDynArr(stack, add(stack));

        }
        else if(strcmp(s, "-") == 0) {
            if(sizeDynArr(stack) < 2)
                DIE_ERR(error, bin_format, topDynArr(stack), s, insf_args, bin_usage);
            pushDynArr(stack, subtract(stack));
        }
        else if(c == '/') {
            if(sizeDynArr(stack) < 2)
                DIE_ERR(error, bin_format, topDynArr(stack), s, insf_args, bin_usage);
            pushDynArr(stack, divide(stack));
        }
        else if(c == '*' || c == 'x') {
            if(sizeDynArr(stack) < 2)
                DIE_ERR(error, bin_format, topDynArr(stack), s, insf_args, bin_usage);
            pushDynArr(stack, multiply(stack));
        }
        else if(c == '^') {
            if(strcmp(s, "^2") == 0) {
                if(sizeDynArr(stack) < 1)
                    DIE_ERR(error, un_format, s, insf_args, un_usage);
                pushDynArr(stack, square(stack));
            }
            else if(strcmp(s, "^3") == 0) {
                if(sizeDynArr(stack) < 1)
                    DIE_ERR(error, un_format, s, insf_args, un_usage);
                pushDynArr(stack, cube(stack));
            }
            else {
                if(sizeDynArr(stack) < 2)
                    DIE_ERR(error, un_format, s, insf_args, un_usage);
                pushDynArr(stack, power(stack));
            }
        }
        else if(strcmp(s, "abs") == 0) {
            if(sizeDynArr(stack) < 1)
                DIE_ERR(error, un_format, s, insf_args, un_usage);
            pushDynArr(stack, absolute_value(stack));
        }
        else if(strcmp(s, "sqrt") == 0) {
            if(sizeDynArr(stack) < 1)
                DIE_ERR(error, un_format, s, insf_args, un_usage);
            pushDynArr(stack, square_root(stack));
        }
        else if(strcmp(s, "exp") == 0) {
            if(sizeDynArr(stack) < 1)
                DIE_ERR(error, un_format, s, insf_args, un_usage);
            pushDynArr(stack, exponential(stack));
        }
        else if(strcmp(s, "ln") == 0) {
            if(sizeDynArr(stack) < 1)
                DIE_ERR(error, un_format, s, insf_args, un_usage);
            pushDynArr(stack, natural_logarithm(stack));
        }
        else if(strcmp(s, "log") == 0) {
            if(sizeDynArr(stack) < 1)
                DIE_ERR(error, un_format, s, insf_args, un_usage);
            pushDynArr(stack, logarithm(stack));
        }
        else if(strcmp(s, "pi") == 0)
            pushDynArr(stack, M_PI);
        else if(strcmp(s, "e") == 0)
            pushDynArr(stack, M_E);
        else if(isfloat(s))
            pushDynArr(stack, atof(s));
        else
            DIE_ERR(error, un_format, s, inval_in, un_usage);
    }   //end for

    if(sizeDynArr(stack) > 1)
        DIE_ERR(error, un_format, s, hng_ops, db_usage);

    result = topDynArr(stack);
    return result;
}

