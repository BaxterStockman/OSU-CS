#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "bst.h"
#include "structs.h"

#define PIZZA_PI 3.14159

enum my_types {provided, pizza, car};
#define D_TYPE provided

#if DTYPE == provided
#define COMP_M compare_provided
#define PRINT_M print_provided
#elif DTYPE == pizza
#define COMP_M compare_pizza
#define PRINT_M print_pizza
#elif DTYPE == car
#define COMP_M compare_car
#define PRINT_M print_car
#endif

int compare_provided(const void *lhs, const void *rhs) {
    struct data *left = (struct data*)lhs;
    struct data *right = (struct data*)rhs;
    if(left->number > right->number) return 1;
    else if(left->number == right->number) return 0;
    else return -1;
}

int compare_pizza(const void *lhs, const void *rhs) {
    struct pizza *left = (struct pizza*)lhs;
    struct pizza *right = (struct pizza*)rhs;
    double left_val = pow(((double)left->size / 2), 2) * PIZZA_PI * left->num_toppings / left->price;
    double right_val = pow(((double)right->size / 2), 2) * PIZZA_PI * right->num_toppings / right->price;
    if (left_val > right_val) return 1;
    else if (left_val == right_val) return 0;
    else return -1;
}

int compare_car(const void *lhs, const void *rhs) {
    struct car *left = (struct car*)lhs;
    struct car *right = (struct car*)rhs;

    int make_cmp = strncmp(left->make, right->make, strlen(left->make));
    if (make_cmp > 0) return 1;
    if (make_cmp < 0) return -1;

    int model_cmp = strncmp(left->model, right->model, strlen(left->model));
    if (model_cmp > 0) return 1;
    if (model_cmp < 0) return -1;

    if(left->year > right->year) return 1;
    else if(left->year == right->year) return 0;
    else return -1;
}

void print_provided(const void *val) {
    struct data *inst = (struct data*)val;
    printf("%s: %d\n", inst->name, inst->number);
}

void print_pizza(const void *val) {
    int i;
    struct pizza *inst = (struct pizza*)val;
    printf("%s: %d\" $%d\nToppings:", inst->name, inst->size, inst->price);
    for(i = 0; i < inst->num_toppings; i++)
        printf(" %s", inst->toppings[i]);
    putchar('\n');
}

void print_car(const void *val) {
    struct car *inst = (struct car*)val;
    printf("Make: %s; Model: %s; Year: %d\n", inst->make, inst->model, inst->year);
}

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater then, less then, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */

 /*Define this function, type casting the value of void * to the desired type.
  The current definition of TYPE in bst.h is void*, which means that left and
  right are void pointers. To compare left and right, you should first cast
  left and right to the corresponding pointer type (struct data *), and then
  compare the values pointed by the casted pointers.

  DO NOT compare the addresses pointed by left and right, i.e. "if (left < right)",
  which is really wrong.
 */

int d_compare(const void * left, const void *right, int (*my_compare)(const void *, const void *)) {
    return my_compare(left, right);
}

void d_print(const void *val, void (*my_print)(const void *)) {
    my_print(val);
}

int compare(TYPE left, TYPE right) {
    return d_compare(left, right, COMP_M);
}

/*Define this function, type casting the value of void * to the desired type*/
void print_type(TYPE curval) {
    d_print(curval, PRINT_M);
}

