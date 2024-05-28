#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>  // Include for close() function

#include "my_global.h"
extern "C" {
#include "page0zip.h"
}

int main(int argc, char** argv){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input file> <output file> [<start page>]\n", argv[0]);
        return 1;
    }

    int f1 = open(argv[1], O_RDONLY);
    int f2 = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    int start_page = argc > 3 ? atoi(argv[3]) : 0;

    page_zip_des_t* page_zip;
    page_t* page;
    ibool all = true;
    srv_max_n_threads = 50000;
    os_sync_init();
    sync_init();

    if(-1 == f1) {
        fprintf(stderr, "Can't open file %s\n", argv[1]);
        return 1;
    }
    if(-1 == f2) {
        fprintf(stderr, "Can't open file %s\n", argv[2]);
        return 1;
    }

    struct stat st;
    if (fstat(f1, &st) == -1) {
        fprintf(stderr, "Can't get file size for %s\n", argv[1]);
        return 1;
    }
    off_t file_size = st.st_size;
    int estimated_pages = file_size / (8 * 1024);

    fprintf(stderr," *********************** Decompressing pages .... *************** \n");
    fprintf(stderr,"File size: %ld bytes\n", file_size);
    fprintf(stderr,"Estimated number of pages: %d\n", estimated_pages);
    fprintf(stderr,"Starting from page: %d\n", start_page);

    page = static_cast<page_t*>(ut_align((page_t*) malloc(2 * UNIV_PAGE_SIZE), UNIV_PAGE_SIZE));
    if(page == NULL){
        fprintf(stderr, "Can't allocate memory for page\n");
        return 1;
    }
    page_zip = (page_zip_des_t*)malloc(sizeof(page_zip_des_t));
    page_zip->data = static_cast<page_zip_t*>(ut_align((page_zip_t*)malloc(2 * UNIV_PAGE_SIZE), UNIV_PAGE_SIZE));
    if(page_zip == NULL){
        fprintf(stderr, "Can't allocate memory for page_zip\n");
        return 1;
    }
    if(page_zip->data == NULL){
        fprintf(stderr, "Can't allocate memory for page_zip->data\n");
        return 1;
    }

    int read_bytes = 0;
    unsigned page_type;
    unsigned page_level;
    int total_pages = 0;
    int processed_pages = 0;
    off_t offset = start_page * (8 * 1024);
    lseek(f1, offset, SEEK_SET);

    while(1){
        memset(page_zip->data, 0, 8 * 1024);
        memset(page, 0, UNIV_PAGE_SIZE);
        read_bytes = read(f1, page_zip->data, 8 * 1024);
        if(read_bytes == -1) return 1;
        if(read_bytes == 0) break; // End of file

        total_pages++;

        page_zip->m_start = 0;
        page_zip->m_end = 0;
        page_zip->m_nonempty = 0;
        page_zip->n_blobs = 0;
        page_zip->ssize = 4;
        page_type = mach_read_from_2(page_zip->data + FIL_PAGE_TYPE);
        page_level = mach_read_from_2(page + PAGE_HEADER + PAGE_LEVEL);

        if(page_type == FIL_PAGE_INDEX){
            if(FALSE == page_zip_decompress(page_zip, page, all)){
                fprintf(stderr, "Decompression failed for page %d\n", total_pages + start_page);
                break;
            } else {
                fprintf(stderr, "Decompression OK for page %d\n", total_pages + start_page);
                write(f2, page, UNIV_PAGE_SIZE);
                processed_pages++;
            }
        }
    }

    fprintf(stderr,"Total pages read: %d\n", total_pages + start_page);
    fprintf(stderr,"Successfully processed pages: %d\n", processed_pages);

    free(page);
    free(page_zip->data);
    free(page_zip);
    close(f1);
    close(f2);

    return 0;
}

