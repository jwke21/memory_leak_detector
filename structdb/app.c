#include "mld.h"
#include <memory.h>
#include <stdlib.h>

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


int main(int argc, char **argv)
{
    /* Initialize a new structure db */
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));

    /* Create structure record for structure emp_t */
    
}
