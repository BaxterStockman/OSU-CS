/* CS261- Assignment 1 - student.h*/
/* Name: Matt Schreiber
 * Date: 10-12-2013
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

struct student{
    int id;
    int score;
};

struct student* allocate(int cls){
     /*Allocate memory for ten students*/
    int arr_size = cls * sizeof(struct student);
    struct student *student_array = malloc(arr_size);

     /*return the pointer*/
    return student_array;
}

void generate(struct student* students, int cls){
    srand(time(NULL));

     /*Generate random ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
    int ids[cls];
    int m_score = 100;
    int i, j = 0;

    for(i = 0; i < cls; i++) {
        ids[i] = rand() % cls + 1;
        for(j = 0; j < i; j++) {
            if (ids[i] == ids[j]) {
                ids[i] = rand() % cls + 1;
                j = 0;
                continue;
            }
        }
    }

    for(i = 0; i < cls; i++) {
        students[i].id = ids[i];
        students[i].score = rand() % (m_score + 1);
    }
}

void output(struct student* students, int cls){
     /*Output information about the ten students in the format:
              ID1 Score1
              ID2 score2
              ID3 score3
              ...
              ID10 score10*/
    int i;

    for(i = 0; i < cls; i++) {
        printf("ID: %2d\tScore: %3d\n", students[i].id, students[i].score);
    }
}

void summary(struct student* students, int cls){
     /*Compute and print the minimum, maximum and average scores of the ten students*/
    int i, min, max, avg = 0;
    min = max = students[0].score;

    for(i = 0; i < cls; i++) {
        avg += students[i].score;
        min = min < students[i].score ? min : students[i].score;
        max = max > students[i].score ? max : students[i].score;
    }

    avg /= cls;

    printf("cls average:\t%3d\n", avg);
    printf("Lowest score:\t%3d\n", min);
    printf("Highest score:\t%3d\n", max);
}

void deallocate(struct student* students){
     /*Deallocate memory from students*/
    if(students != NULL)
        free(students);
    else {
        printf("Error: can't free NULL pointer.\n");
        exit(0);
    }
}

