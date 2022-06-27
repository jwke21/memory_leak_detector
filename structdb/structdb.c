#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE     128

/*
    enums to identify data type of fields in
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
    Field info struct to store info about a field in a
    given struct in companion C application.
*/
typedef struct _field_info_
{
    char fname [MAX_FIELD_NAME_SIZE];
    unsigned int size;
    unsigned int offset;
    data_type_t dtype;
    char nested_str_name [MAX_STRUCTURE_NAME_SIZE];
} field_info_t;

/*
   Struct to store info about struct being registered
   by companion C application. Underlying data structure
   is a Linked-List.
   (i.e. structure database record)
*/
typedef struct _struct_db_rec_ struct_db_rec_t;

struct _struct_db_rec_
{
    struct_db_rec_t *next;
    char struct_name [MAX_STRUCTURE_NAME_SIZE]; // key
    unsigned int ds_size;
    unsigned int n_fields;
    field_info_t *fields;
};