#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mld.h"


/*
   Converts a given data type enum to string. 
*/
char *d_type_to_string(data_type_t d_type)
{
    switch(d_type)
    {
        case UINT8:      return "UINT8";
        case UINT32:     return "UINT32";
        case INT32:      return "INT32";
        case CHAR:       return "CHAR";
        case OBJ_PTR:    return "OBJ_PTR";
        case FLOAT:      return "FLOAT";
        case DOUBLE:     return "DOUBLE";
        case OBJ_STRUCT: return "STRUCT_PTR";
        default:         return "N/A";
    }
}

/*
   Prints all info about a given field.
*/
void print_field(field_info_t *fld_info)
{
    printf("Field name: %20s | %5s",       fld_info->fname, "");
    printf("Field size: %5d | %5s",        fld_info->size, "");
    printf("Field offset: %10d | %5s",     fld_info->offset, "");
    printf("Field data type: %10s | %5s",  d_type_to_string(fld_info->dtype), "");

    if (strcmp(fld_info->nested_str_name, "0") == 0)
        printf("Nested struct: %10s  |\n", "N/A");
    else
        printf("Nested struct: %10s  |\n",  fld_info->nested_str_name);
}

/*
   Prints all info about a given record.
*/
void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if (!struct_rec) return;

    printf("Structure Name: %20s\n",   struct_rec->struct_name);
    printf("Structure Size: %20d\n",     struct_rec->ds_size);
    printf("Number of Fields: %18d\n\n", struct_rec->n_fields);

    int i;
    field_info_t *field;
    
    for (i = 0; i < struct_rec->n_fields; ++i) {
        field = &struct_rec->fields[i];
        printf("%68s FIELD %d \n", "", i+1);
        print_field(field);
        
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    printf("\n");
}

void print_structure_db(struct_db_t *struct_db)
{
    struct_db_rec_t *cur_rec;
    int i;

    cur_rec = struct_db->head;
    for (i = 0; i < struct_db->count; ++i) {
        printf("\nStructure Number: %5d\n\n", i+1);
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
