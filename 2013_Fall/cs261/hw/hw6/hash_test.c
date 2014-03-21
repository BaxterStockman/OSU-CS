#include <stdio.h>
#include <stdlib.h>
#include "hashMap.h"

int main(void) {
    struct hashMap *testMap = createMap(8);

    char *of = "one fish";
    char *tf = "two fish";
    char *rf = "red fish";
    char *bf = "blue fish";
    char *mu = "A string I just made up";

    insertMap(testMap, of, 1);
        if(containsKey(testMap, of));
            printf("Hash contains %s.\n", of);
    printMap(testMap);
    insertMap(testMap, tf, 2);
        if(containsKey(testMap, tf));
            printf("Hash contains %s.\n", tf);
    printMap(testMap);
    insertMap(testMap, rf, 3);
        if(containsKey(testMap, rf));
            printf("Hash contains %s.\n", rf);
    printMap(testMap);
    insertMap(testMap, bf, 4);
        if(containsKey(testMap, bf));
            printf("Hash contains %s.\n", bf);
    printMap(testMap);
    if(containsKey(testMap, mu) == 1)
        printf("Hash contains %s, even though it shouldn't.\n", mu);
    printMap(testMap);

    printf("%d\n", stringHash1("has"));
    printf("%d\n", stringHash1("ash"));
    printf("%d\n", stringHash2("has"));
    printf("%d\n", stringHash2("ash"));

    printf("Table load is %g\n", tableLoad(testMap));
    printf("There are %d empty buckets\n", emptyBuckets(testMap));

    deleteMap(testMap);
    return 0;
}
