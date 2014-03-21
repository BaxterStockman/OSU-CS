#include "linkedList.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*Function to get number of milliseconds elapsed since program started execution*/
double getMilliseconds() {
   return 1000.0 * clock() / CLOCKS_PER_SEC;
}

int main(int argc, char* argv[]){
    if( argc != 2 ) return 0;

    struct linkedList* b = createLinkedList();
    int n = atoi(argv[1]);/*number of elements to add*/
    int i;
    for( i = 0 ; i < n; i++)
        addList(b, (TYPE)i);/*Add elements*/

    double t1 = getMilliseconds();/*Time before contains()*/

    for(i=0; i<n; i++)
        containsList(b, i);

    double t2 = getMilliseconds();/*Time after contains()*/
    printf("Items:%-6d Time (ms):%-6g\n", n, t2-t1);
    return 0;
}
