#include <stdio.h>
#include "dynamicArray.h"
#include "toDoList.h"
#include "type.h"

int main(void) {
    struct DynArr *test = createDynArr(2);
    Task *t1 = createTask(1, "this");
    Task *t2 = createTask(2, "that");

    addDynArr(test, t1);
    addDynArr(test, t2);
    printList(test);

    swapDynArr(test, 0, 1);
    printList(test);

    deleteDynArr(test);
    return 0;

}
