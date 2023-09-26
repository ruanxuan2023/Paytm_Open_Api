#ifndef PAYTM_FILE_API_H
#define PAYTM_FILE_API_H

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

typedef enum{
    Paytm_File_Begin = 0, // Beginning of file
    Paytm_File_Current, // Current position of file pointer
    Paytm_File_End // End of file
}Paytm_file_seek_t;

typedef struct 
{
    uint16_t id;
    struct node_t* next;
}node_t;

typedef struct 
{
    node_t* directories;
    node_t* files;
    uint16_t size;
}Paytm_list_item_t;

void Paytm_list_item_free(Paytm_list_item_t *itemslist);
int32 Paytm_dir_listfiles(Paytm_location_t sl, Paytm_list_item_t *itemslist, const char *dirpath, uint8 read_size);

//-1: filepath is null -2: file already exists -3: file create fial fd: create file handle
PFILE Paytm_fcreate(Paytm_location_t sl, const char *filepath, const char *flag);
PFILE Paytm_fopen(Paytm_location_t sl, const char *filename, const char *flag);
int32 Paytm_fread(void *ptr, Paytm_size_t size, Paytm_size_t nmemb, PFILE filehandle);
int32 Paytm_fwrite(void *ptr, Paytm_size_t size, Paytm_size_t nmemb, PFILE filehandle);
int32 Paytm_fseek(PFILE filehandle, int32 offset, Paytm_file_seek_t whence);
int32 Paytm_ftell(PFILE filehandle);
int32 Paytm_fgetpos(PFILE filehandle, uint32 *pos);
int32 Paytm_fsetpos(PFILE filehandle, const uint32 *pos);
int32 Paytm_fsize(PFILE filehandle);
uint8 Paytm_feof(PFILE filehandle);
uint8 Paytm_ferror(PFILE filehandle);
int32 Paytm_fflush(PFILE filehandle);
int32 Paytm_fclose(PFILE filehandle);
int32 Paytm_filesize(Paytm_location_t sl, const char *filename);
int32 Paytm_fremove(Paytm_location_t sl, const char *filename);
void Paytm_frewind(PFILE filehandle);
int32 Paytm_fgetc(PFILE filehandle);
int32 Paytm_fputc(int32 ch, PFILE filehandle);
char *Paytm_fgets(char *str, int n, PFILE filehandle);
int32 Paytm_fputs(const char *str, PFILE filehandle);
int32 Paytm_fexists(Paytm_location_t sl, const char *filename);
int32 Paytm_frename(Paytm_location_t sl, const char *old_filename, const char *new_filename);
int32 Paytm_fprintf(PFILE fh, const char *format, ...);
int32 Paytm_dir_create(Paytm_location_t sl, const char *directory);
int32 Paytm_dir_create_all_subdirectories(Paytm_location_t sl, const char *directory);
int32 Paytm_dir_delete(Paytm_location_t sl, const char *);
int32 Paytm_dir_delete_all_subdirectories(Paytm_location_t sl, const char *directory);
int32 Paytm_dir_exists(Paytm_location_t sl, const char *directory);

#endif