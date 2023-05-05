#include <stdio.h>


/*
    Question 1:
    For a Given C structure as below, write a C 
    program which prints the size and offset of
    each field of this structure.
*/

typedef struct emp_ {
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
} emp_t;

/* Instructor's solutions */

#define OFFSETOF(struct_name, fld_name) \
    (unsigned int)&(((struct_name *)0)->fld_name)

#define FIELD_SIZE(struct_name, fld_name) \
    sizeof(((struct_name *)0)->fld_name)

/*
    Question 2:
    Given the below struture, create an array of
    objects of this structure type.
*/
typedef struct _student {
    char stud_name[32];
    int rollno;
    int age;
} student_t;

/* Instructor's solution */
student_t stud_array[] = {
    {"Abhishek", 123, 168},
    {"Shivani", 111, 151}
};


int main()
{


    void _printFieldSizesAndOffsets(void) {
        printf("field size = %d, field offset = %d\n", 30, 0);
        printf("field size = %lu, field offset = %d\n", sizeof(unsigned int), 30);
        printf("field size = %lu, field offset = %d\n", sizeof(unsigned int), 34);
        printf("field size = %lu, field offset = %d\n", sizeof(void *), 38);
        printf("field size = %lu, field offset = %d\n", sizeof(float), 46);     // pointers are 8 bytes on my platform
    }

    void _betterPrintFieldSizesAndOffsets(void) {
        printf("field size = %lu, field offset = %d\n", FIELD_SIZE(emp_t, emp_name), OFFSETOF(emp_t, emp_name));
        printf("field size = %lu, field offset = %d\n", FIELD_SIZE(emp_t, emp_id), OFFSETOF(emp_t, emp_id));
        printf("field size = %lu, field offset = %d\n", FIELD_SIZE(emp_t, age), OFFSETOF(emp_t, age));
        printf("field size = %lu, field offset = %d\n", FIELD_SIZE(emp_t, mgr), OFFSETOF(emp_t, mgr));
        printf("field size = %lu, field offset = %d\n", FIELD_SIZE(emp_t, salary), OFFSETOF(emp_t, salary));
    }

    // _printFieldSizesAndOffsets();
    _betterPrintFieldSizesAndOffsets();

    student_t student1 = {
        .stud_name = "Student1",
        .rollno = 100,
        .age = 25
    };

    student_t student2 = {
        .stud_name = "Student2",
        .rollno = 101,
        .age = 22
    };

    student_t student3 = {
        .stud_name = "Student3",
        .rollno = 102,
        .age = 19
    };

    student_t *stud_arr[3] = {
        &student1, 
        &student2, 
        &student3
    };

    int i;
    for (i = 0; i < 3; ++i) {
        printf("name = %s, rollno = %d age = %d\n", stud_arr[i]->stud_name, stud_arr[i]->rollno, stud_arr[i]->age);
    }
}