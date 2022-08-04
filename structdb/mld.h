/*
    Dependencies: stdlib.h, stdio.h.
*/

#ifndef __MLD__

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
   Casts null struct (i.e. 0) as struct pointer,
   accesses target field, casts as int to get
   offset from 0.
*/
#define OFFSET_OF(struct_name, fld_name) \
        (unsigned int)&(((struct_name *)0)->fld_name)

#define FIELD_SIZE(struct_name, fld_name) \
        sizeof(((struct_name *)0)->fld_name)

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

/***** Printing functions *****/

/*
   Prints all info stored by a given struct record.
*/
void print_structure_rec(struct_db_rec_t *struct_rec);

/*
   Prints all struct records in a given struct 
   record database.
*/
void print_structure_db(struct_db_t *srtuct_db);


/***** Functions/Macros to register and add struct records in db *****/

/*
    Adds a given struct record to the struct database.
    Returns 0 on success, -1 on failure.
*/
int add_struct_to_db(struct_db_t *struct_db, struct_db_rec_t *struct_record);

/*
    Instantiates a field_info_t struct from the given info about the field.
    If there is no nested struct, set its value to 0.
*/
#define FIELD_INFO(struct_name, fld_name, data_type, nested_struct_name) \
        {                                                                \
            #fld_name,                                                 \
            FIELD_SIZE(struct_name, fld_name),                           \
            OFFSET_OF(struct_name, fld_name),                            \
            data_type,                                                   \
            #nested_struct_name                                          \
        }

/*
    Macro containing procedure to register given struct, with the given fields,
    in the given struct db.
*/
#define REGISTER_STRUCT(struct_db, struct_name, fields_array)                   \
        do {                                                                    \
            struct_db_rec_t *record = calloc(1, sizeof(struct_db_rec_t));       \
            strncpy(record->struct_name, struct_name, MAX_STRUCTURE_NAME_SIZE); \
            record->ds_size = sizeof(struct_name);                              \
            record->n_fields = sizeof(fields_array) / sizeof(field_info_t);     \
            if (add_struct_to_db(struct_db, record)) {                          \
                assert(0);                                                      \
            }                                                                   \
        } while (0);

#endif /* __MLD__ */
