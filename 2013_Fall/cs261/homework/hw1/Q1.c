/* CS261- Assignment 1 - Q.1*/
/* Name: Matt Schreiber
 * Date: 10-12-2-13
 * Solution description:    Creates an array of 10 students,
 *                          assigns IDs and scores to them
 *                          randomly, and outputs them to the
 *                          console.  Also calculates and outputs
 *                          the average score, lowest score, and
 *                          highest score.
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define NUM_STUDENTS 10

typedef struct student{
    int id;
    int score;
} student;

struct student* allocate() {
     /*Allocate memory for ten students*/
    int arr_size = NUM_STUDENTS * sizeof(student);
    student *student_array = malloc(arr_size);

     /*return the pointer*/
    return student_array;
}

void generate(student* students){
    srand(time(NULL));

     /*Generate random ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
    int class = NUM_STUDENTS;
    int ids[class];
    int m_score = 100;
    int i, j = 0;

    for(i = 0; i < class; i++) {
        ids[i] = rand() % class + 1;
        while(j < i) {
            if (ids[i] == ids[j]) {
                ids[i] = rand() % class + 1;
                j = 0;
                continue;
            }
            j++;
        }
        j = 0;
    }

    for(i = 0; i < class; i++) {
        students[i].id = ids[i];
        students[i].score = rand() % (m_score + 1);
    }
}

void output(student* students){
     /*Output information about the ten students in the format:
              ID1 Score1
              ID2 score2
              ID3 score3
              ...
              ID10 score10*/
    int class = NUM_STUDENTS;
    int i;

    for(i = 0; i < class; i++) {
        printf("ID: %2d\tScore: %3d\n", students[i].id, students[i].score);
    }
}

void summary(student* students){
     /*Compute and print the minimum, maximum and average scores of the ten students*/
    int class = NUM_STUDENTS;
    int i, min, max, avg = 0;
    min = max = students[0].score;

    for(i = 0; i < class; i++) {
        avg += students[i].score;
        // Assign to min the lower of min and students[i].score
        min = min < students[i].score ? min : students[i].score;
        // Assign to max the lower of max and students[i].score
        max = max > students[i].score ? max : students[i].score;
    }

    avg /= class;

    printf("Class average:\t%3d\n", avg);
    printf("Lowest score:\t%3d\n", min);
    printf("Highest score:\t%3d\n", max);
}

void deallocate(student* students){
     /*Deallocate memory from students*/
    if(students) free(students);
    else {
        printf("Error: can't free memory.\n");
        exit(0);
    }
    students = NULL;
}

int main() {
    student* stud = NULL;

    /*call allocate*/
    stud = allocate();

    /*call generate*/
    generate(stud);

    /*call output*/
    putchar('\n');
    output(stud);
    putchar('\n');

    /*call summary*/
    summary(stud);
    putchar('\n');

    /*call deallocate*/
    deallocate(stud);
    return 0;
}
