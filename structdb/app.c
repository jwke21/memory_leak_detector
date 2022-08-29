#include <memory.h>
#include <stdlib.h>

#include "mld.h"


/* Application structures */

/*
    Employee struct: used as example to highlight
    MLD struct db functionality.
*/
typedef struct emp_
{
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
} emp_t;

/*
    Student struct: used as example to highlight
    MLD struct db functionality.
*/
typedef struct student_{

    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student_ *best_classmate;
} student_t;

int main(int argc, char **argv)
{
    /* Initialize a new structure db */
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));

    /* Create array for employee fields */
    static field_info_t emp_fields[] = {
        FIELD_INFO(emp_t, emp_name, CHAR, 0),
        FIELD_INFO(emp_t, emp_id, UINT32, 0),
        FIELD_INFO(emp_t, age, UINT32, 0),
        FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
        FIELD_INFO(emp_t, salary, FLOAT, 0)
    };
    /* Register employee struct in structure db */
    REGISTER_STRUCT(struct_db, emp_t, emp_fields)

    /* Create array for student fields */
    static field_info_t stud_fiels[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno,    UINT32, 0),
        FIELD_INFO(student_t, age,       UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_classmate, OBJ_PTR, student_t)
    };
    /* Register student struct in structure db */
    REGISTER_STRUCT(struct_db, student_t, stud_fiels)

    print_structure_db(struct_db);
    return 0;
}
