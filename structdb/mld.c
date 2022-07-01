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
void print_field(field_info_t fld_info)
{
    printf("Field name:      \t\t%s", fld_info.fname);
    printf("Field size:      \t\t%ul", fld_info.size);
    printf("Field offset:    \t\t%ul", fld_info.offset);
    printf("Field data type: \t\t%s", d_type_to_string(fld_info.dtype));
    printf("Nested struct:   \t\t%s", fld_info.nested_str_name);
}

void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if (!struct_rec) return;

    printf("Structure Name:   \t%s\n\n", struct_rec->struct_name);
    printf("Structure Size:   \t%ul\n", struct_rec->ds_size);
    printf("Number of Fields: \t%ul\n", struct_rec->n_fields);

    int i;
    for (i = 0; i < struct_rec->n_fields; ++i) {
        printf("\t\tFIELD %d\n", i);
        print_field(struct_rec->fields[i]);
        printf("-------------------------");
    }
    printf("\n");
}

void print_structure_db(struct_db_t *struct_db)
{
    struct_db_rec_t *cur_rec;
    int i;

    cur_rec = struct_db;
    for (i = 0; i < struct_db->count; ++i) {
        print_structure_rec(cur_rec);
        cur_rec = cur_rec->next;
    }
}

