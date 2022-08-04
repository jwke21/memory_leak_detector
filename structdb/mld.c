#include <stdio.h>
#include <stdlib.h>

#include "mld.h"


/*
   Converts a given data type enum to string. 
*/
char *d_type_to_string(data_type_t d_type)
{
    switch(d_type)
    {
        case UINT8: return "Unsigned Integer, 8-bytes";
        case UINT32: return "Unsigned Integer, 32-bytes";
        case INT32: return "Integer, 32-bytes";
        case CHAR: return "Character";
        case OBJ_PTR: return "Object Pointer";
        case FLOAT: return "Float";
        case DOUBLE: return "Double";
        case OBJ_STRUCT: return "Struct Pointer";
        default: return "N/A";
    }
}

/*
   Prints all info about a given field.
*/
void print_field(field_info_t *fld_info)
{
    printf("Field name: %s \t|\t",       fld_info->fname);
    printf("Field size: %ul \t|\t",      fld_info->size);
    printf("Field offset: %ul \t|\t",    fld_info->offset);
    printf("Field data type: %s \t|\t",  d_type_to_string(fld_info->dtype));
    printf("Nested struct: %s \t|\t\n",  fld_info->nested_str_name);
}

/*
   Prints all info about a given record.
*/
void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if (!struct_rec) return;

    printf("Structure Name:   \t%s\n\n", struct_rec->struct_name);
    printf("Structure Size:   \t%ul\n", struct_rec->ds_size);
    printf("Number of Fields: \t%ul\n", struct_rec->n_fields);

    int i;
    field_info_t *field;
    for (i = 0; i < struct_rec->n_fields; ++i) {
        field = &struct_rec->fields[i];
        printf("\t\tFIELD %d\n", i++);
        print_field(field);
        printf("-------------------------");
    }
    printf("\n");
}

void print_structure_db(struct_db_t *struct_db)
{
    struct_db_rec_t *cur_rec;
    int i;

    cur_rec = struct_db->head;
    for (i = 0; i < struct_db->count; ++i) {
        printf("Struct No: %d\n", i++);
        print_structure_rec(cur_rec);
        cur_rec = cur_rec->next;
    }
}

/*
    Adds a given struct record to the struct database.
    Returns 0 on success, -1 on failure.
*/
int add_struct_to_db(struct_db_t *struct_db, struct_db_rec_t *struct_record)
{
    if (!struct_record || !struct_db) return -1;

    struct_db_rec_t *cur_rec = struct_db->head;

    if (!cur_rec) {
        struct_db->head = struct_record;
    } else {
        while (cur_rec->next) {
            cur_rec = cur_rec->next;
        }
        cur_rec->next = struct_record;
    }

    struct_record->next = NULL;
    struct_db->count++;
    
    return 0;
}
