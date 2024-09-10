/* 
Tyler Carpenter
COP3502C-24Summer C001

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef malloc
#undef calloc
#undef free

#include "leak_detector_c.h"

typedef struct student {
  char *lname; // this will require DMA to store a string
  int assignment;
  int finalExam;
  int total;
  int *quizzes; // this will require DMA to store quizzes
} student;

student **readCourses(int *C, int *N, int *M);
void printCourseList(student **courseList, int C, int N, int M);
void printHighestTotal(student **courseList, int C, int N, int M);
void release_memory(student **courseList, int C, int N, int M);

int main() {
  atexit(report_mem_leak); // for memory leak detector.
  student **courseList;

  int i, j, C, N, M;

  // Take number of courses C, number of students N and number of quizzes M
  scanf("%d %d %d", &C, &N, &M);

  // Create course list, get input for scores, quizzes, and final exam for all
  // students
  courseList = readCourses(&C, &N, &M);

  // Print course list
  //printCourseList(courseList, C, N, M);

  // Print student with highest total score
  printHighestTotal(courseList, C, N, M);

  // xfree(ptr);
  release_memory(courseList, C, N, M);

  return 0;
}

student **readCourses(int *C, int *N, int *M) {
  // Allocate memory for C number of courses
  student **courseList =
      (student **)xmalloc(*C * sizeof(student *), __FILE__, __LINE__);
  // Allocate memory for each student of N number of students
  for (int i = 0; i < *C; i++) {
    courseList[i] =
        (student *)xmalloc(*N * sizeof(student), __FILE__, __LINE__);
    // Check memory leak
    if (courseList[i] == NULL) {
      for (int j = 0; j < i; j++) {
        xfree(courseList[j]);
      }
      xfree(courseList);
      return NULL;
    }
    // Allocate memory for M number of quizzes and student name
    for (int j = 0; j < *N; j++) {
      courseList[i][j].quizzes =
          (int *)xmalloc(*M * sizeof(int), __FILE__, __LINE__);
      // Check memory leak
      if (courseList[i][j].quizzes == NULL) {
        for (int k = 0; k < j; k++) {
          xfree(courseList[i][k].quizzes);
        }
        xfree(courseList[i]);
        for (int j = 0; j < i; j++) {
          xfree(courseList[j]);
        }
        xfree(courseList);
        return NULL;
      }
      courseList[i][j].lname =
          (char *)xmalloc(50 * sizeof(char), __FILE__, __LINE__);
      // Check memory leak
      if (courseList[i][j].lname == NULL) {
        for (int k = 0; k < *M; k++) {
          xfree(courseList[i][j].quizzes[k]);
        }
        xfree(courseList[i][j].quizzes);
        xfree(courseList[i]);
        for (int j = 0; j < i; j++) {
          xfree(courseList[j]);
        }
        xfree(courseList);
        return NULL;
      }
    }
  }
  // Store information (Name, quizzes, assignment, total, final exam for each
  // student)
  int total = 0;
  for (int i = 0; i < *C; i++) {
    for (int j = 0; j < *N; j++) {
      total = 0;
      // last name, assignment score, scores for M number of quizzes, and final
      // exam score
      scanf("%s %d", courseList[i][j].lname,
            &(courseList[i][j].assignment)); // Lastname and assignment
      for (int k = 0; k < *M; k++) {
        scanf("%d", &(courseList[i][j].quizzes[k])); // Quizzes
        total += courseList[i][j].quizzes[k];        // Quizzes to total
      }
      scanf("%d", &(courseList[i][j].finalExam)); // Final exam
      // calculate and store total
      total = total + courseList[i][j].assignment + courseList[i][j].finalExam; // add assignment and final exam
      courseList[i][j].total = total;
    }
  }

  return courseList;
}

void printCourseList(student **courseList, int C, int N, int M)
// Prints the full course list
// This function was to help me visualize the data
{
  printf("Course List: \n");
  for (int i = 0; i < C; i++) {
    // Print course numbers
    printf("Course %d: \n", i + 1);
    for (int j = 0; j < N; j++) {
      // Print student information
      printf("%s ", courseList[i][j].lname);      // Last name
      printf("%d ", courseList[i][j].assignment); // Assignment
      for (int k = 0; k < M; k++) {
        printf("%d ", courseList[i][j].quizzes[k]); // Quizzes
      }
      printf("%d \n", courseList[i][j].finalExam); // Final exam")
    }
  }
}

void printHighestTotal(student **courseList, int C, int N, int M)
// This function finds and prints the student with the highest total score
{
  int maxTotal = 0;
  int maxTotalIndex = 0;
  for (int i = 0; i < C; i++) {
    for (int j = 0; j < N; j++) {
      if (courseList[i][j].total > maxTotal) {
        maxTotal = courseList[i][j].total;
        maxTotalIndex = i * N + j;
      }
    }
  }
  int studentIndex = maxTotalIndex / N;
  int courseIndex = maxTotalIndex % N;
  printf("Name: %s\n", courseList[studentIndex][courseIndex].lname);
  printf("Assignment: %d\n", courseList[studentIndex][courseIndex].assignment);
  printf("Quizzes: ");
  for (int k = 0; k < M; k++) {
    printf("%d ", courseList[studentIndex][courseIndex].quizzes[k]);
  }
  printf("\nFinal exam: %d\n", courseList[studentIndex][courseIndex].finalExam);
  printf("Total: %d\n", maxTotal);
  printf("Course number: %d\n", studentIndex + 1);
}

void release_memory(student **courseList, int C, int N, int M)
// Releases memory for course list
{
  for (int i = 0; i < C; i++) {
    for (int j = 0; j < N; j++) {
      xfree(courseList[i][j].quizzes);
      xfree(courseList[i][j].lname);
    }
    xfree(courseList[i]);
  }
  xfree(courseList);
}
