#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "my_global.h"
extern "C" {
#include "page0zip.h"
}

int main(int argc, char** argv){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input file> <output file> [<start page>]\n", argv[0]);
        return 1;
    }

    // Handle input file
    int f1;
    if (strcmp(argv[1], "-") == 0) {
        f1 = STDIN_FILENO;  // Read from stdin
    } else {
        f1 = open(argv[1], O_RDONLY);
    }

    // Handle output file
    int f2;
    if (strcmp(argv[2], "-") == 0) {
        f2 = STDOUT_FILENO;  // Write to stdout
    } else {
        f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    int start_page = argc > 3 ? atoi(argv[3]) : 0;

    page_zip_des_t* page_zip;
    page_t* page;
    ibool all = true;
    srv_max_n_threads = 50000;
    os_sync_init();
    sync_init();

    // Error checking for input file
    if(f1 < 0) {
        fprintf(stderr, "Can't open input file %s\n", argv[1]);
        perror("open");
        return 1;
    }

    // Error checking for output file
    if(f2 < 0) {
        fprintf(stderr, "Can't open output file %s\n", argv[2]);
        perror("open");
        return 1;
    }

    struct stat st;
    off_t file_size = 0;
    int estimated_pages = 0;

    // Only perform fstat if input is a regular file
    if (f1 != STDIN_FILENO) {
        if (fstat(f1, &st) == -1) {
            fprintf(stderr, "Can't get file size for %s\n", argv[1]);
            perror("fstat");
            return 1;
        }
        file_size = st.st_size;
        estimated_pages = file_size / (8 * 1024);
    }

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

    // Close file descriptors if they're not stdin/stdout
    if (f1 != STDIN_FILENO) {
        close(f1);
    }
    if (f2 != STDOUT_FILENO) {
        close(f2);
    }

    return 0;
}

