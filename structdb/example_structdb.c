#include <stdio.h>
#include "structdb.c"

#define MAX_NAME_SIZE 30
/*
    Employee struct: used as example to highlight
    MLD struct db functionality.
*/
typedef struct emp_
{
    char emp_name[MAX_NAME_SIZE];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
} emp_t;


/*
    Array containing info about fields of emp_ struct, each
    element will be a pointer to a field_info_t struct containing
    the info about the associated emp_t struct field.

    example:
        field_info_t example_field = {
            char fname [MAX_FIELD_NAME_SIZE=128],
            unsigned int size,
            unsigned int offset,
            data_type_t dtype,
            char nested_str_name [MAX_STRUCTURE_NAME_SIZE=128]
        };
*/
field_info_t emp_fields[5];

field_info_t emp_fields[0] = {
    "emp_name",
    sizeof(char) * MAX_NAME_SIZE,
    0,
    CHAR,
    NULL
};

field_info_t emp_fields[1] = {
    "emp_id",
    sizeof(unsigned int),
    30,            // (i.e. 0 + fields[0].size)
    UINT8,
    NULL
};

field_info_t emp_fields[2] = {
    "age",
    sizeof(unsigned int),
    34,             // (i.e. 0 + fields[0].size + fields[1].size)
    UINT8,
    NULL
};

field_info_t emp_fields[3] = {
    "mgr",
    sizeof(void *),   // (i.e. just the size of a pointer, sizeof pointer will depend on platform)
    38,               // (i.e. 0 + fields[0].size + fields[1].size + fields[2].size)
    OBJ_PTR,
    "emp_t",          // (i.e. it contains a pointer to a emp_t nested struct type)
};

field_info_t emp_fields[4] = {
    "salary",
    sizeof(float),
    46,
    FLOAT,
    NULL
};

/*
    Struct_db_rec object associated with emp_t will
    have the following fields:
        struct_db_rec_t *next;
        char struct_name [MAX_STRUCTURE_NAME_SIZE];
        unsigned int ds_size;
        unsigned int n_fields;
        field_info_t *fields;
*/
struct_db_rec_t emp_struct = {
    NULL,
    "emp_t",
    sizeof(emp_t),
    5,
    emp_fields
};

