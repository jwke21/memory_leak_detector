/*
    Dependencies: assert.h, stdlib.h, stdio.h.
*/

#ifndef __MLD__
#include <assert.h>

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE     128

/*
    Enums to identify data type of fields in
    in companion C app's structure.
*/
typedef enum 
{
    UINT8,      // unsigned int 8-bits
    UINT32,     // unsigned int 32-bits
    INT32,      // int 32-bits
    CHAR,       // character
    OBJ_PTR,    // pointer to another obj
    FLOAT,      // float
    DOUBLE,     // double
    OBJ_STRUCT  // pointer to another struct
} data_type_t;

/*
    Enums to identify booleans.
*/
typedef enum
{
    MLD_FALSE,
    MLD_TRUE
} mld_boolean_t;

/* 
   Casts null struct (i.e. 0) as struct pointer,
   accesses target field, casts as int to get
   offset from 0.
*/
#define OFFSET_OF(struct_name, fld_name) \
        (unsigned int)&(((struct_name *)0)->fld_name)

#define FIELD_SIZE(struct_name, fld_name) \
        sizeof(((struct_name *)0)->fld_name)


typedef struct _struct_db_rec_ struct_db_rec_t;

/*
    Struct to store info about a field in a
    given struct in companion C application.
*/
typedef struct _field_info_
{
    char fname [MAX_FIELD_NAME_SIZE];   /* Name of the field */
    unsigned int size;                  /* Size of the field */
    unsigned int offset;                /* Offset of the field */
    data_type_t dtype;                  /* Data type of the field */
    /* following field is only meaninful if dtype = OBJ_PTR or OBJ_STRCUT */
    char nested_str_name [MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

/*
   Struct to store info about struct being registered
   by companion C application. Underlying data structure
   is a Linked-List.
   (i.e. structure database record)
*/
typedef struct _struct_db_rec_
{
    struct_db_rec_t *next;  /* Pointer to the next struct in the db */
    char struct_name [MAX_STRUCTURE_NAME_SIZE]; // key
    unsigned int ds_size;   /* Size of the struct */
    unsigned int n_fields;  /* Num of fields in the struct */
    field_info_t *fields;   /* Pointer to the array of field structs */
} struct_db_rec_t;

/* Head of structure db linked list */
typedef struct _struct_db_
{
    struct_db_rec_t *head;
    unsigned int count;
} struct_db_t;

/***** Structure db definition ends *****/

/***** Printing functions for struct db *****/

/*
   Prints all info stored by a given struct record.
*/
void print_structure_rec(struct_db_rec_t *struct_rec);

/*
   Prints all struct records in a given struct 
   record database.
*/
void print_structure_db(struct_db_t *struct_db);


/***** Functions/Macros to register and add struct records in db *****/

/*
    Adds a given struct record to the struct database.
    Returns 0 on success, -1 on failure.
*/
int add_struct_to_db(struct_db_t *struct_db, struct_db_rec_t *struct_record);

/*
    Instantiates a field_info_t struct from the given info about the field.
    If there is no nested struct, set its value to 0 (i.e. NULL).
*/
#define FIELD_INFO(struct_name, fld_name, data_type, nested_struct_name) \
    {                                                                    \
        #fld_name,                                                       \
        FIELD_SIZE(struct_name, fld_name),                               \
        OFFSET_OF(struct_name, fld_name),                                \
        data_type,                                                       \
        #nested_struct_name                                              \
    }

/*
    Macro containing procedure to register given struct, with the given fields,
    in the given struct db.
*/
#define REGISTER_STRUCT(struct_db, added_struct_name, fields_array)                 \
    do {                                                                            \
        struct_db_rec_t *record = calloc(1, sizeof(struct_db_rec_t));               \
        strncpy(record->struct_name, #added_struct_name, MAX_STRUCTURE_NAME_SIZE);  \
        record->ds_size = sizeof(added_struct_name);                                \
        record->n_fields = sizeof(fields_array)/sizeof(field_info_t);               \
        record->fields = fields_array;                                              \
        if (add_struct_to_db(struct_db, record)) {                                  \
            assert(0);                                                              \
        }                                                                           \
    } while (0);







/***** Object db structs *****/

/*
    Structure to store info about an object malloc'd (or calloc'd)
    by companion C application.
*/
typedef struct _object_db_rec_ object_db_rec_t;

struct _object_db_rec_
{
    object_db_rec_t *next;
    void *obj_ptr;               // Pointer to C app object associated with record.
    unsigned int units;          // Number of objects of this type that the record calloc'd.
    struct_db_rec_t *struct_rec; // struct_db record associated with object.
    mld_boolean_t visited;       // For graph traversal.
    mld_boolean_t is_root;       // Whether the object is a root object.
};


/*
    Object database struct (linked list).
*/
typedef struct _object_db_
{
    struct_db_t *struct_db;
    object_db_rec_t *head;
    unsigned int count;
} object_db_t;


/***** Printing functions for struct db *****/

/*
   Prints all info stored by a given object record.
*/
void print_obj_db_rec(object_db_rec_t *obj_rec, int index);

/*
   Prints all object records in a given object record db.
*/
void print_obj_db(object_db_t *obj_db);

/*
    Prints the values of all the given object record's fields.
*/
void mld_dump_object_rec_detail(object_db_rec_t *obj_rec);

/***** Memory allocation functions *****/

/*
    Custom calloc to be used with MLD library in order for companion C app
    to allocate memory for an object in a way that enables MLD library to
    track whether it's been free'd.

    RETURNS
        Pointer to allocated object, or NULL if the allocation failed.

    example usage:
        emp_t *emp = mld_calloc(obj_db_ptr, "emp_t", 1);
    is equivalent with:
        emp_t *emp = calloc(1, sizeof(emp_t));
*/
void *mld_calloc(object_db_t *object_db, char *struct_name, int units);

/*
    Frees the given object from the object db.
*/
void mld_free(object_db_t *object_db, void *object);


/***** Root object functions *****/

/*
    Registers an object already in the object db (i.e. already allocated with mld_calloc) as root.
*/
void register_root_object(object_db_t *object_db, void *object, char *struct_name, unsigned int units);

/*
    Allocates an object, adds it to the object db and sets it to root.
*/
void add_object_as_global_root(object_db_t *object_db, void *object);


/***** Memory leak detection functions *****/

/*
    Checks for leaked objects in the given object database.
*/
void run_mld_check(object_db_t *object_db);

/*
    Prints the objects in the given object database that are determined
    to be leaked. Only works after having called run_mld_check on the
    given object_db. 
*/
void print_leaked_objects(object_db_t *object_db);


#endif /* __MLD__ */
