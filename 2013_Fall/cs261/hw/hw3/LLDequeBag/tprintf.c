#include<stdio.h>
#include<stdlib.h>

#define TYPE double

#if TYPE == double || TYPE == float
#define TFMT "%g"
#elif TYPE == int || TYPE == short
#define TFMT "%f"
#endif

int main() {
    TYPE t = 20;
    printf("The value of variable t of TYPE is: " TFMT "\n", t);
}
