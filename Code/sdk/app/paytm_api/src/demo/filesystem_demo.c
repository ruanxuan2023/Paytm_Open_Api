#include <stdlib.h>
#include <stdio.h>
#include "typedef.h"
#include "osi_api.h"

#include "paytm_file_api.h"
#include "paytm_debug_uart_api.h"

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