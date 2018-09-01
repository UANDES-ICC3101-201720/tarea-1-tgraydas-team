
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include "types.h"
#include "const.h"
#include "util.h"

int finish;
int arr[10];
bool fnd = false;
int found;
int job_waiting = 0;
int max;
int min;
int xx;
int n = sizeof(arr);
int max_threads;
int c = 0;
int position;
int size;

// TODO: implement

typedef struct
{
    unsigned int *arr;
    int l;
    int r;
    int x;
} data;

void serial_binsearch(unsigned int arr[], int l, int r, int x)
{
    int low, high, mid;

    low = l;
    high = r - 1;

    while (low < high)
    {
        mid = (low + high) / 2;

        if (x < mid)
            high = mid - 1;
        else if (x > mid)
            low = mid + 1;
        else
        {
            break;
        }
    }
}
void *binsearch(void *args)
{
    data *info = args;
    int low, high, mid, x;
    x = info->x;
    low = info->l;
    high = info->r - 1;
    job_waiting++;
    finish = 0;
    while (low < high && !finish)
    {
        mid = (low + high) / 2;
        if (x < mid)
            high = mid - 1;
        else if (x > mid)
            low = mid + 1;
        else
        {
            finish = 1;
            pthread_exit(0);
        }
    }
    return 0;
}
// TODO: implement
void parallel_binsearch(unsigned int arr[], int l, int r, int x)
{
    int max_threads = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t m_tid[max_threads];
    int c_parts = r / max_threads;
    int mult = 1;
    for (int i = 0; i < max_threads; i++)
    {
        data *info = malloc(sizeof(data));
        info->arr = arr;
        info->l = l;
        info->x = position;
        info->r = (c_parts * mult) - 1;
        if (pthread_create(&m_tid[i], NULL, (void *)binsearch, info))
        {
            free(info);
        }
        l = c_parts * mult;
        mult++;
    }
}

int main(int argc, char **argv)
{
    char experiments[2] = "";
    int option = 0;
    while ((option = getopt(argc, argv, "T:E:P:")) != -1)
    {
        switch (option)
        {
        case 'T':

            strcpy(experiments, optarg);
            continue;
        case 'E':
            size = atoi(optarg);
            continue;
        case 'P':

            position = atoi(optarg);
            continue;
        default:
            printf("Parameters wrong");
            exit(1);
        }
    }
    /* TODO: start datagen here as a child process. */
    pid_t i = fork();
    if (i == 0)
    {
        if (execv("./datagen", argv) < 0)
        {
            printf("Doesnt works");
            exit(1);
        }
    }
    else if (i > 0)
    {
    }
    else
    {
        perror("fork failed");
        exit(3);
    }
    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * ocupar read del socket para el arreglo del binsearch
     */

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */

    struct sockaddr_un addr;

    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, DSOCKET_PATH, sizeof(addr.sun_path));
    int re = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (re == -1)
    {
        perror("error al conectar con el socket");
    }
    char arr[20] = "BEGIN S";
    strcat(arr, experiments);
    if (write(fd, arr, sizeof(arr)) == -1)
    {
        printf("Error al iniciar el datagen\n");
    }
    long unsigned int readvalues = 0;
    size_t numvalues = pow(10, atoi(experiments));
    size_t readbytes = 0;
    UINT *readbuf = malloc(sizeof(UINT) * numvalues);
    while (readvalues < numvalues)
    {
        readbytes = read(fd, readbuf + readvalues, sizeof(UINT) * 1000);
        readvalues += readbytes / 4;
    }

    if (write(fd, DATAGEN_END_CMD, sizeof(DATAGEN_END_CMD)) == -1)
    {
        printf("Error al terminar datagen\n");
    }
    struct timespec start, finish;
    double elapsed = 0;
    double parallel_e = 0;
    for (int i = 0; i < size; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        serial_binsearch(readbuf, 0, numvalues, position);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        clock_gettime(CLOCK_MONOTONIC, &start);
        parallel_binsearch(readbuf, 0, numvalues, position);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        parallel_e = (finish.tv_sec - start.tv_sec);
        parallel_e += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("%i, %s, %lf, %lf\n", i, experiments, elapsed, parallel_e); // E, T, SERIAL_TIME, PARALEL_TIME
    }

    free(readbuf);
    exit(0);
}