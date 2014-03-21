/* You can modify the structure to store whatever you'd like in your BST */

int d_compare(const void * left, const void *right, int (*my_compare)(const void *, const void *));
void d_print(const void *val, void (*my_print)(const void *));

struct data {
    int number;
    char *name;
};

struct pizza {
    char *name;
    int size;
    char **toppings;
    int num_toppings;
    int price;
};

struct car {
    char *make;
    char *model;
    int year;
};

int d_compare(const void * left, const void *right, int (*my_compare)(const void *, const void *));
void d_print(const void *val, void (*my_print)(const void *));
