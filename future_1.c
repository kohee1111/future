//dprintf vs fprintf time comparison by chatgpt 
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define N 100000

int main() {
    FILE *fp1 = fopen("fprintf_output.txt", "w");
    int fd2 = open("dprintf_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(!fp1 || fd2 == -1) {
        perror("File open failed");
        return 1;
    }

    clock_t start, end;
    double time_fprintf, time_dprintf;

    // -------- fprintf timing --------
    start = clock();
    for(int i = 0; i < N; i++) {
        fprintf(fp1, "Line number %d: Hello fprintf!\n", i);
    }
    fclose(fp1); // fclose flushes the buffer
    end = clock();
    time_fprintf = (double)(end - start) / CLOCKS_PER_SEC;

    // -------- dprintf timing --------
    start = clock();
    for(int i = 0; i < N; i++) {
        dprintf(fd2, "Line number %d: Hello dprintf!\n", i);
    }
    close(fd2);
    end = clock();
    time_dprintf = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time taken by fprintf (buffered): %f seconds\n", time_fprintf);
    printf("Time taken by dprintf (unbuffered): %f seconds\n", time_dprintf);

    return 0;
}
///another future example of custom fucntions of open() 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int open_my_file(const char *file_name, int flags, mode_t perm) {
    int fd = open(file_name, flags, perm);
    if (fd < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void my_print(const char *message, ...) {
    va_list ap;
    va_start(ap, message);
    vdprintf(STDOUT_FILENO, message, ap);
    va_end(ap);
}

int main(void) {
    const char *target_file = "log.c";
    int fd = open_my_file(target_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int n = dprintf(fd, "Writing something via dprintf and low level system call!\n");
    if (n < 0) {
        perror("Failed to write in the file");
        close(fd);
        return -1;
    }
    close(fd);
    printf("DONE!\n");
    my_print("Printing message via dprintf()\n");
    return 0;
}
