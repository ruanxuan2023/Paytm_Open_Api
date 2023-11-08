#include <stdlib.h>
#include <stdio.h>
#include "typedef.h"
#include "osi_api.h"

#include "paytm_file_api.h"
#include "paytm_debug_uart_api.h"
#define TEST_FILE_SIZE (128*1024)

void fileFormatDemo(void){
    char *test_dir = "TestDir";
    char *test_buf = malloc(TEST_FILE_SIZE);
    uint32 free_size = Paytm_fs_freesize(LOC_EXTER_MEM);
    int ret = 0;

    for(int i = 0; i < TEST_FILE_SIZE; i++){
        test_buf[i] = i%0xff;
    }

    while (1)
    {
        Paytm_TRACE("1.format file systemm free_size: %d", free_size);
        // formart file system
        Paytm_fs_format(LOC_EXTER_MEM);
        free_size = Paytm_fs_freesize(LOC_EXTER_MEM);
        Paytm_TRACE("2.format file systemm done! free_size: %d ", free_size);

        // check Directory exist
        if(Paytm_dir_exists(LOC_EXTER_MEM, test_dir) == 0){
            Paytm_TRACE("Ext Flash directory %s not exist", test_dir);
            // create Directory
            Paytm_dir_create(LOC_EXTER_MEM, test_dir);
            Paytm_TRACE("Ext Flash create directory %s", test_dir);
        }else{
            Paytm_TRACE("Ext Flash directory %s exist", test_dir);
        }
    
        // create 40 files
        for(int i = 0; i < 40; i++){
            char fname[64] = {0};
            snprintf(fname, sizeof(fname), "%s/Test_%d.txt", test_dir, i);
            PFILE fd = Paytm_fcreate(LOC_EXTER_MEM, fname, "wb+");
            Paytm_fwrite(test_buf, TEST_FILE_SIZE, 1, fd);
            Paytm_fclose(fd);
            Paytm_TRACE("Ext Flash write %s done size: %d", fname, ret);
            osiThreadSleep(20);
        }

        // list directory
        Paytm_list_item_t item_list = {0};
        memset(&item_list, 0, sizeof(item_list));
        int cnt = Paytm_dir_listfiles(LOC_EXTER_MEM, &item_list, test_dir, 255);

        Paytm_TRACE("read %d item from external directory %s", cnt, test_dir);
        node_t *file_node = item_list.files;
        while (file_node)
        {
            Paytm_TRACE("File: %s size: %d", file_node->name, file_node->size);
            file_node = file_node->next;
        }
        Paytm_list_item_free(&item_list);

        free_size = Paytm_fs_freesize(LOC_EXTER_MEM);
        Paytm_TRACE("3. file systemm free_size: %d ", free_size);
        // formart file system
        Paytm_fs_format(LOC_EXTER_MEM);
        free_size = Paytm_fs_freesize(LOC_EXTER_MEM);
        Paytm_TRACE("4.format file systemm done! free_size: %d ", free_size);

        // check Directory exist
        if(Paytm_dir_exists(LOC_EXTER_MEM, test_dir) != 0){
            Paytm_TRACE("Ext Flash directory %s exist", test_dir);
        } else {
            Paytm_TRACE("Ext Flash directory %s not exist", test_dir);
        }
        osiThreadSleep(100);
    }
    
    

    return;
}

void fileSystemDemo(void)
{
    char *test_dir = "TestDir";
    char *test_buf = "45451515dfadsafasd";
    char buf[64] = {0};

    // check Directory exist
    if(Paytm_dir_exists(LOC_EXTER_MEM, test_dir) == 0){
        Paytm_TRACE("Ext Flash directory %s not exist", test_dir);
         // create Directory
        Paytm_dir_create(LOC_EXTER_MEM, test_dir);
        Paytm_TRACE("Ext Flash create directory %s", test_dir);
    }else{
        Paytm_TRACE("Ext Flash directory %s exist", test_dir);
    }
   
    // create 10 files
    for(int i = 0; i < 10; i++){
        char fname[64] = {0};
        snprintf(fname, sizeof(fname), "%s/Test_%d.txt", test_dir, i);
        PFILE fd = Paytm_fcreate(LOC_EXTER_MEM, fname, "wb+");
        Paytm_fwrite(test_buf, strlen(test_buf), 1, fd);
        Paytm_fclose(fd);

        memset(buf, 0, sizeof(buf));
        fd = Paytm_fopen(LOC_EXTER_MEM, fname, "rb");
        Paytm_fread(buf, strlen(test_buf), 1, fd);
        Paytm_fclose(fd);
        if(Paytm_memcmp(test_buf, buf, strlen(test_buf)) == 0){
            Paytm_TRACE("Check file %s success", fname);
        }else{
            Paytm_TRACE("Check file %s fail", fname);
            Paytm_TRACE("readbuf %s", buf);
        }
    }

    // list directory
    Paytm_list_item_t item_list = {0};
    memset(&item_list, 0, sizeof(item_list));
    int cnt = Paytm_dir_listfiles(LOC_EXTER_MEM, &item_list, test_dir, 255);

    Paytm_TRACE("read %d item from external directory %s", cnt, test_dir);
    node_t *file_node = item_list.files;
    while (file_node)
    {
        Paytm_TRACE("File: %s size: %d", file_node->name, file_node->size);
        file_node = file_node->next;
    }
    Paytm_list_item_free(&item_list);

    Paytm_TRACE("start delete subdirectories");
    // delete subdirectory
    Paytm_dir_delete_all_subdirectories(LOC_EXTER_MEM, test_dir);

    // check Directory exist
    if(Paytm_dir_exists(LOC_EXTER_MEM, test_dir) != 0){
        Paytm_TRACE("Ext Flash directory %s exist", test_dir);
    } else {
        Paytm_TRACE("Ext Flash directory %s not exist", test_dir);
    }

    return;
}

#define TEST_FILE_NAME  "compare.txt"
void fileHeapLeakDemo(void* p)
{
    PFILE fd = 0;
    uint8_t buf[4];

    Paytm_fcreate(LOC_INTER_MEM, TEST_FILE_NAME, NULL);

    fd = Paytm_fopen(LOC_INTER_MEM, TEST_FILE_NAME, NULL);

    Paytm_fwrite("123", 3, 1, fd);

    while (1)
    {
        Paytm_fread(buf, 3, 1, fd);
        Paytm_delayMilliSeconds(10 * 1000);
        Paytm_TRACE("Free Heap Info: %d", Paytm_GetFreeHeapSize());
    }

    Paytm_fclose(fd);
}

void fileUnzip(void)
{
    int rc = 0;
    if(Paytm_fexists(LOC_EXTER_MEM, "paytm_res_en.czip") != 0)
    {
        Paytm_TRACE("File paytm_res_en.czip not exists");
        return;
    }

    PFILE fd = 0;
    fd = Paytm_fopen(LOC_EXTER_MEM, "paytm_res_en.czip", "rb+");
    if(fd < 0)
    {
        Paytm_TRACE("File paytm_res_en.czip open fail");
        return;
    }

    Paytm_TRACE("paytm_res_en.czip size %d", Paytm_filesize(LOC_EXTER_MEM, "paytm_res_en.czip"));
    
    Paytm_fclose(fd);

    rc = Paytm_Unzip(LOC_EXTER_MEM, "paytm_res_en.czip", "");
    if(rc)
    {
        Paytm_TRACE("Unzip file paytm_res_en.czip fail, rc = %d", rc);
        return;
    }

    Paytm_list_item_t item_list = {0};
    memset(&item_list, 0, sizeof(item_list));
    int cnt = Paytm_dir_listfiles(LOC_EXTER_MEM, &item_list, "resources/sounds/en", 255);

    Paytm_TRACE("read %d item from external directory resources", cnt);
    node_t *file_node = item_list.files;
    while (file_node)
    {
        Paytm_TRACE("File: %s size: %d", file_node->name, file_node->size);
        file_node = file_node->next;
    }
    Paytm_list_item_free(&item_list);
}

void fileLite(void)
{
    int rc = 0;
    char data[4] = {'o', 'p', 'd', 'e'};

    if(Paytm_fexists(LOC_INTER_MEM, "devinfo.cfg") != 0)
    {
        Paytm_TRACE("File devinfo.cfg not exists");
        return;
    }

    PFILE fd = 0;
    fd = Paytm_fopen(LOC_INTER_MEM, "devinfo.cfg", "wb+");
    if(fd < 0)
    {
        Paytm_TRACE("File devinfo.cfg open fail");
        return;
    }

    Paytm_TRACE("devinfo.cfg size %d, fd = %d", Paytm_filesize(LOC_INTER_MEM, "devinfo.cfg"), fd);

    rc = Paytm_fwrite(data, 1, 4, fd);
    if(rc <= 0)
    {
        Paytm_TRACE("Paytm_fwrite devinfo.cfg fail");
    }

    Paytm_TRACE("Paytm_fwrite devinfo.cfg %d bytes", rc);

    Paytm_fseek(fd, 0, 0);

    rc = Paytm_fread(data, 1, 4, fd);
    if(rc <= 0)
    {
        Paytm_TRACE("Paytm_fread devinfo.cfg fail");
    }

    Paytm_TRACE("Read %s", data);
    Paytm_fclose(fd);
}