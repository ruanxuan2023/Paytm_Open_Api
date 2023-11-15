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
    char name[64];
    uint32_t size;
    struct node_t* next;
}node_t;

typedef struct 
{
    node_t* directories;
    node_t* files;
    uint16_t size;
}Paytm_list_item_t;

void Paytm_list_item_free(Paytm_list_item_t *itemslist);

/**
 * @description: 
 * @return {*}  The read count of files in folder. If empty, return 0
 */
int32 Paytm_dir_listfiles(Paytm_location_t sl, Paytm_list_item_t *itemslist, const char *dirpath, uint8 read_size);


/**
 * @description: 
 * @return {*} true: format success, false: fomat fail
 */
int32 Paytm_fs_format(Paytm_location_t sl);

uint32 Paytm_fs_freesize(Paytm_location_t sl);

/**
 * @description: 
 * @return {*} -1: filepath is null -2: file already exists -3: file create fial fd: create file handle
 */
PFILE Paytm_fcreate(Paytm_location_t sl, const char *filepath, const char *flag);

/**
 * @description: 
 * @return {*} <= 0: means open fail or no this file, > 0: fopen success 
 */
PFILE Paytm_fopen(Paytm_location_t sl, const char *filename, const char *flag);

/**
 * @description: 
 * @param {void} *ptr           pointer to buffer to place data read
 * @param {Paytm_size_t} size   size of element referenced by buffer pointer
 * @param {Paytm_size_t} nmemb  number of elements to be read
 * @param {PFILE} filehandle    stream index of file
 * @return {*}                  return number of elements that successfully read
 */
int32 Paytm_fread(void *ptr, Paytm_size_t size, Paytm_size_t nmemb, PFILE filehandle);

/**
 * @description: 
 * @param {void} *ptr           pointer to buffer to place data written
 * @param {Paytm_size_t} size   size of element referenced by buffer pointer
 * @param {Paytm_size_t} nmemb  number of elements to be writted
 * @param {PFILE} filehandle    stream index of file
 * @return {*}                  number of elements succesfully write
 */
int32 Paytm_fwrite(void *ptr, Paytm_size_t size, Paytm_size_t nmemb, PFILE filehandle);

/**
 * @description: 
 * @param {PFILE} filehandle            stream index of file
 * @param {int32} offset                the offset to obtain to the position indicator (in bytes)
 * @param {Paytm_file_seek_t} whence    the position to add offset(SEEK_SET, SEEK_CUR, SEEK_END)
 * @return {*}                          0:filename is valid     -1:fail
 */
int32 Paytm_fseek(PFILE filehandle, int32 offset, Paytm_file_seek_t whence);


int32 Paytm_ftell(PFILE filehandle);
int32 Paytm_fgetpos(PFILE filehandle, uint32 *pos);
int32 Paytm_fsetpos(PFILE filehandle, const uint32 *pos);
int32 Paytm_fsize(PFILE filehandle);
uint8 Paytm_feof(PFILE filehandle);
uint8 Paytm_ferror(PFILE filehandle);
int32 Paytm_fflush(PFILE filehandle);
int32 Paytm_fclose(PFILE filehandle);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *filename
 * @return {*}                      return size of file, 0 means fail
 */
int32 Paytm_filesize(Paytm_location_t sl, const char *filename);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *filename
 * @return {*}                      0:filename is valid, -1: fail
 */
int32 Paytm_fremove(Paytm_location_t sl, const char *filename);
void Paytm_frewind(PFILE filehandle);
int32 Paytm_fgetc(PFILE filehandle);
int32 Paytm_fputc(int32 ch, PFILE filehandle);
char *Paytm_fgets(char *str, int n, PFILE filehandle);
int32 Paytm_fputs(const char *str, PFILE filehandle);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *filename
 * @return {*}                      0:exists    others:not exists
 */
int32 Paytm_fexists(Paytm_location_t sl, const char *filename);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *old_filename
 * @param {char} *new_filename
 * @return {*}                       0:old filename is valid, -1:fail
 */
int32 Paytm_frename(Paytm_location_t sl, const char *old_filename, const char *new_filename);
int32 Paytm_fprintf(PFILE fh, const char *format, ...);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *directory
 * @return {*}                        0:success, -1:fail
 */
int32 Paytm_dir_create(Paytm_location_t sl, const char *directory);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *directory
 * @return {*}                          0:success, -1:fail                          
 */
int32 Paytm_dir_create_all_subdirectories(Paytm_location_t sl, const char *directory);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *
 * @return {*}                          0:filename is valid, -1:fail
 */
int32 Paytm_dir_delete(Paytm_location_t sl, const char *);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *directory
 * @return {*}                          0:filename is valid, others:fail
 */
int32 Paytm_dir_delete_all_subdirectories(Paytm_location_t sl, const char *directory);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *directory
 * @return {*}                          0:exists    others:not exists
 */
int32 Paytm_dir_exists(Paytm_location_t sl, const char *directory);

/**
 * @description: 
 * @param {Paytm_location_t} sl
 * @param {char} *zipfile
 * @param {char} *directory
 * @return {*}                          0:exists    others:not exists
 */
int32 Paytm_Unzip(Paytm_location_t sl, const char *zipfile, const char *directory);
#endif