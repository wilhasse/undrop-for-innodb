#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#define _XOPEN_SOURCE 600
#include <fcntl.h>
#include <time.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#define COMP_PAGE_SIZE 8192 // 16K, adjust if needed
//#define COMP_PAGE_SIZE 16384 // 16K, adjust if needed
#define BUFF_WRITE_SIZE (16777216) // 16M, adjust if needed
//#define BUFF_WRITE_SIZE (33554432) // 32M, adjust if needed
#define BUFF_FILE_SIZE (536870912) // 512, adjust if needed
#define HEADER_SIZE 94 // FIL HEADER + INDEX + FSEG

#ifdef STREAM_PARSER_DEBUG
int debug = 1;
#else
int debug = 0;
#endif

char dst_dir[1024] = "";
int worker = 0;
off_t ib_size = 0; // Initialize ib_size here
#define mutext_pool_size (8)
#ifdef __APPLE__
dispatch_semaphore_t mutex[mutext_pool_size];
#else
sem_t mutex[mutext_pool_size];
#endif

void usage(char*);

#ifdef STREAM_PARSER_DEBUG
int DEBUG_LOG(char* format, ...) {
    if(debug) {
        char msg[1024] = "";
        va_list args;
        va_start(args, format);
        vsprintf(msg, format, args);
        va_end(args);
        return fprintf(stderr, "Worker(%d): %s\n", worker, msg);
    }
    return 0;
}
#else
int DEBUG_LOG(char* format, ...) {
  return 0;
}
#endif

void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

char* h_size(unsigned long long int size, char* buf) {
    unsigned int power = 0;
    double d_size = size;
    while(d_size >= 1024) {
        d_size /= 1024;
        power += 3;
    }
    sprintf(buf, "%3.3f", d_size);
    switch(power) {
        case 3: sprintf(buf, "%s %s", buf, "kiB"); break;
        case 6: sprintf(buf, "%s %s", buf, "MiB"); break;
        case 9: sprintf(buf, "%s %s", buf, "GiB"); break;
        case 12: sprintf(buf, "%s %s", buf, "TiB"); break;
        default: sprintf(buf, "%s exp(+%u)", buf, power); break;
    }
    return buf;
}

void write_page_data_to_disk(FILE *file, unsigned char *page_data, ssize_t *page_data_offset) {
    fwrite(page_data, 1, *page_data_offset, file);
    *page_data_offset = 0; // Reset offset after writing
}

void process_ibfile(int fn, off_t start_offset, off_t length, int worker_id) {
    off_t offset = start_offset;
    unsigned char *buffer = malloc(BUFF_FILE_SIZE); // 16m buffer
    if (!buffer) error("Can't allocate buffer");
    unsigned char *page_data = malloc(BUFF_WRITE_SIZE); // Buffer to hold up to 1000 pages
    if (!page_data) error("Can't allocate page_data buffer");
    ssize_t page_data_offset = 0;
    int total_pages_found = 0;
    int begin_page = -1;

    // Output file
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/page_file_worker_%d.dat", dst_dir, worker_id);
    FILE *file = fopen(filename, "wb");
    if (!file) error("Can't open output file");

    while (offset < start_offset + length) {
        lseek(fn, offset, SEEK_SET);
        ssize_t read_size = read(fn, buffer, BUFF_FILE_SIZE);
        if (read_size <= 0) break;

        DEBUG_LOG("Worker %d: Reading %lu bytes from offset %lx\n", worker_id, read_size, offset);

        ssize_t i = 0;
        while (i < (read_size - HEADER_SIZE -1)) {
            if ((buffer[i + HEADER_SIZE -2] == 0x00 && buffer[i + HEADER_SIZE -1] == 0x00 && buffer[i + HEADER_SIZE] == 0x68 &&
                buffer[i + HEADER_SIZE + 1] == 0x81 && buffer[i + HEADER_SIZE + 14] == 0xff && buffer[i + HEADER_SIZE + 15] == 0xff) ||

               (buffer[i + HEADER_SIZE -2] == 0x00 && buffer[i + HEADER_SIZE -1] == 0x00 && buffer[i + HEADER_SIZE] == 0x68 &&
                buffer[i + HEADER_SIZE + 1] == 0x81 && buffer[i + HEADER_SIZE + 8] == 0xff && buffer[i + HEADER_SIZE + 9] == 0xff)) {

                // Start page
                begin_page = i;

                // Save the entire 8KB page
                if (read_size - i >= COMP_PAGE_SIZE) {
                    total_pages_found++;
                    DEBUG_LOG("Worker %d: Marker found at position page %d offset %zx, saving 8KB page\n", worker_id, total_pages_found, i+offset);
                    memcpy(page_data + page_data_offset, buffer + i, COMP_PAGE_SIZE);
                    page_data_offset += COMP_PAGE_SIZE;
                    i = i + COMP_PAGE_SIZE;
                } else {
                    // Handle the case where the remaining buffer is less than 8KB
                    ssize_t remaining_size = read_size - i;
                    memcpy(page_data + page_data_offset, buffer + i, remaining_size);
                    page_data_offset += remaining_size;

                    // Read the remaining part from disk to complete the 8KB page
                    lseek(fn, offset + i + remaining_size, SEEK_SET);
                    ssize_t additional_read_size = read(fn, buffer, COMP_PAGE_SIZE - remaining_size);
                    if (additional_read_size > 0) {
                        memcpy(page_data + page_data_offset, buffer, additional_read_size);
                        page_data_offset += additional_read_size;
                        total_pages_found++;
                        DEBUG_LOG("Worker %d: Incomplete compressed page completed, saving 8KB page\n", worker_id);
                    } else {
                        DEBUG_LOG("Worker %d: Failed to read remaining part of the compressed page\n", worker_id);
                    }
                    break;
                }
            } else {

              #ifdef STREAM_PARSER_DEBUG
              if ((i - begin_page > COMP_PAGE_SIZE) && (begin_page != -1)) {
               DEBUG_LOG("Worker %d: Page bigger than 8K \n", worker_id);
               begin_page = -1;
              }
              #endif

              // Move to the next byte
              i++;
            }

            // Write to disk if the buffer is full
            if (page_data_offset >= COMP_PAGE_SIZE * 1000) {
                write_page_data_to_disk(file, page_data, &page_data_offset);
            }
        }

        offset += read_size;
        DEBUG_LOG("Worker %d: End offset %lx\n", worker_id, offset);
    }

    // Write any remaining data to disk
    if (page_data_offset > 0) {
        write_page_data_to_disk(file, page_data, &page_data_offset);
    }

    fclose(file);

    // Print total pages found
    printf("Worker %d: Total pages found: %d\n", worker_id, total_pages_found);
}

int open_ibfile(char *fname) {
    struct stat st;
    int fn;
    char buf[255];

    fprintf(stderr, "Opening file: %s\n", fname);
    fprintf(stderr, "File information:\n\n");

    if (stat(fname, &st) != 0) {
        printf("Errno = %d, Error = %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
#ifdef __APPLE__
    fprintf(stderr, "ID of device containing file: %12d\n", st.st_dev);
    fprintf(stderr, "inode number:                 %12llu\n", st.st_ino);
#else
    fprintf(stderr, "ID of device containing file: %12ju\n", st.st_dev);
    fprintf(stderr, "inode number:                 %12ju\n", st.st_ino);
#endif
    fprintf(stderr, "protection:                   %12o ", st.st_mode);
    switch (st.st_mode & S_IFMT) {
        case S_IFBLK:  fprintf(stderr, "(block device)\n");            break;
        case S_IFCHR:  fprintf(stderr, "(character device)\n");        break;
        case S_IFDIR:  fprintf(stderr, "(directory)\n");               break;
        case S_IFIFO:  fprintf(stderr, "(FIFO/pipe)\n");               break;
        case S_IFLNK:  fprintf(stderr, "(symlink)\n");                 break;
        case S_IFREG:  fprintf(stderr, "(regular file)\n");            break;
        case S_IFSOCK: fprintf(stderr, "(socket)\n");                  break;
        default:       fprintf(stderr, "(unknown file type?)\n");      break;
    }
#ifdef __APPLE__
    fprintf(stderr, "number of hard links:         %12u\n", st.st_nlink);
#else
    fprintf(stderr, "number of hard links:         %12zu\n", st.st_nlink);
#endif
    fprintf(stderr, "user ID of owner:             %12u\n", st.st_uid);
    fprintf(stderr, "group ID of owner:            %12u\n", st.st_gid);
#ifdef __APPLE__
    fprintf(stderr, "device ID (if special file):  %12d\n", st.st_rdev);
    fprintf(stderr, "blocksize for filesystem I/O: %12d\n", st.st_blksize);
    fprintf(stderr, "number of blocks allocated:   %12lld\n", st.st_blocks);
#else
    fprintf(stderr, "device ID (if special file):  %12ju\n", st.st_rdev);
    fprintf(stderr, "blocksize for filesystem I/O: %12lu\n", st.st_blksize);
    fprintf(stderr, "number of blocks allocated:   %12ju\n", st.st_blocks);
#endif
    fprintf(stderr, "time of last access:          %12lu %s", st.st_atime, ctime(&(st.st_atime)));
    fprintf(stderr, "time of last modification:    %12lu %s", st.st_mtime, ctime(&(st.st_mtime)));
    fprintf(stderr, "time of last status change:   %12lu %s", st.st_ctime, ctime(&(st.st_ctime)));
    h_size(st.st_size, buf);
    fprintf(stderr, "total size, in bytes:         %12jd (%s)\n\n", (intmax_t)st.st_size, buf);

    fn = open(fname, O_RDONLY);
#ifdef posix_fadvise
    posix_fadvise(fn, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif
    if (fn == -1) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }
    if (ib_size == 0) {
        if (st.st_size != 0) {
            ib_size = st.st_size;
        }
    }
    if (ib_size == 0) {
        fprintf(stderr, "Can't determine size of %s. Specify it manually with -t option\n", fname);
        exit(EXIT_FAILURE);
    }
#ifdef __APPLE__
    fprintf(stderr, "Size to process:              %12lld (%s)\n", ib_size, h_size(ib_size, buf));
#else
    fprintf(stderr, "Size to process:              %12lu (%s)\n", ib_size, h_size(ib_size, buf));
#endif

    return fn;
}

void usage(char* cmd) {
    fprintf(stderr,
        "Usage: %s -f <innodb_datafile> [-d <destination_directory>] [-V|-g]\n"
        "  Where:\n"
        "    -h         - Print this help\n"
        "    -V or -g   - Print debug information\n"
        "    -d dir     - Destination directory for extracted pages\n",
        cmd);
}

int main(int argc, char **argv) {
    int fn = 0, ch;
    char ibfile[1024] = "";

    while ((ch = getopt(argc, argv, "gVhf:d:")) != -1) {
        switch (ch) {
            case 'f':
                strncpy(ibfile, optarg, sizeof(ibfile));
                break;
            case 'd':
                strncpy(dst_dir, optarg, sizeof(dst_dir));
                break;
            case 'V':
            case 'g':
                debug = 1;
                break;
            default:
            case '?':
            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
    if (strlen(ibfile) == 0) {
        fprintf(stderr, "You must specify file with -f option\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strlen(dst_dir) == 0) {
        snprintf(dst_dir, sizeof(dst_dir), "pages-%s", basename(ibfile));
    }

    if (-1 == mkdir(dst_dir, 0755)) {
        //fprintf(stderr, "Could not create directory %s\n", dst_dir);
        //perror("mkdir()");
        //exit(EXIT_FAILURE);
    }

    int i = 0;
    for (i = 0; i < mutext_pool_size; i++) {
#ifdef __APPLE__
        mutex[i] = dispatch_semaphore_create(1);
#else
        sem_init(mutex + i, 1, 1);
#endif
    }

    fn = open_ibfile(ibfile);
    if (fn == 0) {
        fprintf(stderr, "Can not open file %s\n", ibfile);
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int ncpu = sysconf(_SC_NPROCESSORS_CONF);
    ncpu = 1;
    DEBUG_LOG("Number of CPUs %d\n", ncpu);
    int n;
    pid_t *pids = malloc(sizeof(pid_t) * ncpu);
    if (!pids) {
        char tmp[20];
        fprintf(stderr, "Can't allocate memory (%s) for pid cache\n", h_size(sizeof(pid_t) * ncpu, tmp));
        error("PID cache allocation failed");
    }

    time_t a, b;
    time(&a);
    for (n = 0; n < ncpu; n++) {
        pid_t pid = fork();
        if (pid == 0) {
            worker = n;
            DEBUG_LOG("I'm child(%d): %u.", n, getpid());
            DEBUG_LOG("Processing from %lu bytes starting from %lu", ib_size / ncpu, n * ib_size / ncpu);
            process_ibfile(fn, n * ib_size / ncpu, ib_size / ncpu,worker);
            DEBUG_LOG("Finished process_ibfile... ");
            exit(EXIT_SUCCESS);
        } else {
            pids[n] = pid;
        }
    }

    for (int n = 0; n < ncpu; n++) {
        int status;
        waitpid(pids[n], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            fprintf(stderr, "Worker %d failed with exit status %d\n", n, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Worker %d was terminated by signal %d\n", n, WTERMSIG(status));
        }
    }

    for (i = 0; i < mutext_pool_size; i++) {
#ifdef __APPLE__
        dispatch_release(mutex[i]);
#else
        sem_destroy(mutex + i);
#endif
    }
    time(&b);
    printf("All workers finished in %lu sec\n", b - a);
    close(fn);
    exit(EXIT_SUCCESS);
}
