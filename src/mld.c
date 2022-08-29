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
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        field = &struct_rec->fields[i];
        printf("%68s FIELD %d \n", "", i+1);
        print_field(field);
        
    }
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
    printf("\n");
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

/*
    Finds the given struct in the given struct_db.
    Returns the matching struct_db_rec_t pointer if found, NULL if not found.
*/
struct_db_rec_t *struct_db_look_up(struct_db_t *struct_db, char *struct_name)
{
    if (!struct_db || !struct_name) return NULL;

    struct_db_rec_t *cur_rec = struct_db->head;

    while (cur_rec) {
        if (strcmp(cur_rec->struct_name, struct_name) == 0) {
            return cur_rec;
        }
        cur_rec = cur_rec->next;
    }

    return NULL;
}

void print_obj_db_rec(object_db_rec_t *obj_rec, int index)
{
    if (!obj_rec) return;

    printf("------------------------------------------------------------------------\n");
    printf("| %-3d | memory_loc = %-10p | units = %4d | name = %-10s |\n",
           index, obj_rec->obj_ptr, obj_rec->units, obj_rec->struct_rec->struct_name);
}

void print_obj_db(object_db_t *obj_db)
{
    if (!obj_db) return;
    
    object_db_rec_t *cur_rec;
    int i;

    printf("Total registered objects: %-3d\n\n", obj_db->count);

    cur_rec = obj_db->head;
    i = 1;
    while(cur_rec) {
        print_obj_db_rec(cur_rec, i);
        cur_rec = cur_rec->next;
        i++;
    }

    printf("------------------------------------------------------------------------\n\n");
}

/*
    Determines whether the given object pointer is in the object_db.
    Returns the objects record if in db, otherwise returns NULL.
*/
object_db_rec_t *find_obj_rec(object_db_t *object_db, void *obj)
{
    object_db_rec_t *cur_rec = object_db->head;

    while (cur_rec) {
        if (cur_rec->obj_ptr == obj) {
            return cur_rec;
        }
        cur_rec = cur_rec->next;
    }

    return NULL;
}


/*
    Adds a given object record to the given object database.
    Returns 0 on success, -1 on failure.
*/
int add_obj_to_db(object_db_t *object_db, void *object, int units, struct_db_rec_t *struct_rec)
{
    if (!object_db || !object || !struct_rec) return -1;

    // Check that the object is not already in db.
    object_db_rec_t *match = find_obj_rec(object_db, object);
    assert(!match);

    object_db_rec_t *obj_rec = calloc(units, sizeof(object_db_rec_t));

    obj_rec->next = object_db->head;
    obj_rec->obj_ptr = object;
    obj_rec->struct_rec = struct_rec;
    obj_rec->units = units;

    object_db->head = obj_rec;
    object_db->count++;

    return 0;
}


void *mld_calloc(object_db_t *object_db, char *struct_name, int units)
{
    if (!object_db || !struct_name) return NULL;

    // Get associated struct record from struct db.
    struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
    // Check that associated struct record exists.
    assert(struct_rec);
    // Allocate space for the object.
    void *allocated_obj = calloc(units, struct_rec->ds_size);
    // Add object to object db.
    if (add_obj_to_db(object_db, allocated_obj, units, struct_rec)) {
        assert(0);
    }
    return allocated_obj;
}

void mld_dump_object_rec_detail(object_db_rec_t *obj_rec)
{
    if (!obj_rec) return;

    int i, j;
    int n_fields = obj_rec->struct_rec->n_fields;
    int units = obj_rec->units;
    char *struct_name;
    char *field_name;
    char *cur_object;
    unsigned int offset;


    for (i=0; i < units; i++) {
        cur_object = obj_rec->obj_ptr + (i * obj_rec->struct_rec->ds_size);
        
        for (i=0; i < n_fields; i++) {
            struct_name = obj_rec->struct_rec->struct_name;
            field_name = obj_rec->struct_rec->fields[i].fname;
            offset = obj_rec->struct_rec->fields[i].offset;

            // printf format: [struct_name]->[field]->[field_value]
            switch (obj_rec->struct_rec->fields[i].dtype) {
                case UINT8:
                case UINT32:
                    printf("%s->%s==%u\n", struct_name, field_name, *(unsigned int *)(obj_rec->obj_ptr + offset));
                    break;
                case INT32:
                    printf("%s->%s==%d\n", struct_name, field_name, *(int *)(obj_rec->obj_ptr + offset));
                    break;
                case CHAR:
                    printf("%s->%s==%s\n", struct_name, field_name, (char *)(obj_rec->obj_ptr + offset));
                    break;
                case OBJ_PTR:
                    printf("%s->%s==%p\n", struct_name, field_name, (void *)(obj_rec->obj_ptr + offset));
                    break;
                case FLOAT:
                    printf("%s->%s==%f\n", struct_name, field_name, *(float *)(obj_rec->obj_ptr + offset));
                    break;
                case DOUBLE:
                    printf("%s->%s==%f\n", struct_name, field_name, *(double *)(obj_rec->obj_ptr + offset));
                    break;
                default:
                    break;
            }
        }
    }
}

void remove_obj_rec_from_db(object_db_t *object_db, object_db_rec_t *obj_rec)
{
    assert(object_db);
    assert(obj_rec);

    object_db_rec_t *cur_rec = object_db->head;
    // Remove record from object db
    if (cur_rec == obj_rec) {
        object_db->head = obj_rec->next;
    } else {
        while (cur_rec->next != obj_rec) {
            cur_rec = cur_rec->next;
        }
        cur_rec->next = obj_rec->next;
    }
    object_db->count--;
    free(obj_rec);
}

void mld_free(object_db_t *object_db, void *object)
{
    if (!object_db || !object) return;

    object_db_rec_t *obj_rec;

    obj_rec = find_obj_rec(object_db, object);

    assert(obj_rec);

    // Free object
    free(obj_rec->obj_ptr);

    remove_obj_rec_from_db(object_db, obj_rec);
}
