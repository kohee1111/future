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
