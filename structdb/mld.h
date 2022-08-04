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
   Struct to store info about struct being registered
   by companion C application. Underlying data structure
   is a Linked-List.
   (i.e. structure database record)
*/
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

struct _struct_db_rec_
{
    struct_db_rec_t *next;  /* Pointer to the next struct in the db */
    char struct_name [MAX_STRUCTURE_NAME_SIZE]; // key
    unsigned int ds_size;   /* Size of the struct */
    unsigned int n_fields;  /* Num of fields in the struct */
    field_info_t *fields;   /* Pointer to the array of field structs */
};

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


/***** Functions to manage records in db *****/

/*
    Adds a given struct record to the struct database.
    Returns 0 on success, -1 on failure.
*/
int add_struct_to_db(struct_db_t *struct_db, struct_db_rec_t *struct_record);

#endif /* __MLD__ */
