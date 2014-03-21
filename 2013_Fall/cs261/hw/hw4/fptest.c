#include<stdio.h>
#include<stdlib.h>

int compare_fp(const void *lhs, const void *rhs) {
    return 1;
}

int d_compare(const void *lhs, const void *rhs, int(*my_compare)(const void *, const void *)) {
    return my_compare(lhs, rhs);
}

//int compare(void *left, void *right) {
//    d_compare(left, right, &compare_fp(left, right));
//}
//

int main(void) {
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));

    *a = 1;
    *b = 2;

    return d_compare(a, b, compare_fp);
}
