#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <math.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>

/*
 * This struct for scheduler of data
 */
typedef struct {
    char* data;
    int size;
    int fd_read;
    int fd_write;
    char* p_in;
    char* p_out;
} data_struct;
/*
 * Other define
 */
#define PAGE_SIZE 4096
#define P_READ 0
#define P_WRITE 1

#define SHL_m(ptr, start, end) ({                             \
    char* iter = 0;                                           \
    for (iter = (&(ptr)[(start)]); iter < (end); iter++)    \
        *(iter - start) = *iter;                            \
})
/*
 * Function prototype
 */
int start_server(int number, int descriptor);
/*
 * Main
 */
int main(int argc, char* argv []) {
    const int base = 10;
    char* endptr = NULL;
    /*
     * Number of children
     */
    int number = 0;
    /*
     * File descriptor
     */
    int file_d = 0;
	/*
     * Handler input data
     */
	switch (argc) {
    case 1:
        fprintf(stderr, "No arguments.\n");
        exit(EXIT_FAILURE);
        break;
    case 2:
        fprintf(stderr, "Invalid arguments. Use: \"proxy NUMBER FILENAME\".\n");
        exit(EXIT_FAILURE);
        break;
    case 3:
        number = strtol(argv[1], &endptr, base);
        if (argv[1] + strlen(argv[1]) != endptr) {
            fprintf (stderr, "Uncorrect input data (found symbols).\n");
            exit(EXIT_FAILURE);
        }
        if (number < 1) {
            fprintf (stderr, "Uncorrect interval(only from 1).\n");
            exit(EXIT_FAILURE);
        }
        if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN)) || \
           (number > INT_MAX || number < INT_MIN )) {
            fprintf(stderr, "Too long for me.\n");
            exit(EXIT_FAILURE);				
        }
        file_d = open(argv[2], O_RDONLY);
        if (file_d == -1) {
            fprintf(stderr, "File not exist.\n");
            exit(EXIT_FAILURE);
        }
        break;
    default:
        fprintf (stderr, "Many arguments for me.\n");
        exit(EXIT_FAILURE);
    }

    if (start_server(number, file_d) == -1)
        exit(EXIT_FAILURE);

    return (EXIT_SUCCESS);    
}

/*
 * number - number of proccess
 * descriptor - file descriptor
 */

int start_server(int number, int descriptor) {
    int i = 0;
    /*
     * Mode of working
     * 0 - children
     * other - parent
    */
    int mode = 0;
    /*
     * Try to allocate memory for data_struct
     */
    data_struct* proxy_data = (data_struct*) calloc(number, sizeof(data_struct));
    if (proxy_data == NULL) {
        fprintf(stderr, "Can't allocate memory.\n");
        return -1;
    }
    fprintf(stderr, "Memory allocated for data_struct memory.\n");
    /*
     * Check 3^number with INT_MAX
     */
    double data_size_buffer = pow(3, number); 
    if (data_size_buffer > (double)INT_MAX) {
        fprintf(stderr, "I shall not allocate memory for buffers.\n");
        return -1;
    }
    int data_size = (int) data_size_buffer;
    fprintf(stderr, "Size calculated: %d.\n", data_size);
    /*
     * For select(syscall)
     */
    int pipe_max_d = 0;
    /*
     * This pipes for each children
     */
    int child_in  = 0;
    int child_out = 0;
    /*
     * Temporary pipes
     */
    int temp_fd1[2];
    int temp_fd2[2];
    /*
     * Try to allocate memory for buffers
     * and fill fields in data_struct,
     * then FORK
     */
    for (i = 0; i < number; i++) {
        proxy_data[i].data = (char*) calloc(data_size * PAGE_SIZE, sizeof(char));
        if (proxy_data[i].data == NULL) {
            fprintf(stderr, "Can't allocate memory for buffers.\n");
            return -1;
        }
        fprintf(stderr, "Memory allocated for buffer data. Cur_size = %d * PAGE_SIZE.\n", data_size);
        proxy_data[i].size = data_size * PAGE_SIZE;
        proxy_data[i].p_in = proxy_data[i].data;
        proxy_data[i].p_out = proxy_data[i].data;
        data_size /= 3;
        
        pipe(temp_fd1);
        pipe(temp_fd2);

        mode = fork();
        /*
         * Parent mode
         */
        if (mode != 0) {
            /*
             * Close unusual pipes
             */
            close(temp_fd1[P_READ]);
            close(temp_fd2[P_WRITE]);
            /*
             * Set pipe max descriptor
             */
            if (temp_fd1[P_WRITE] - temp_fd2[P_READ] < 0) {
                if (temp_fd2[P_READ] > pipe_max_d)
                    pipe_max_d = temp_fd2[P_READ];
            } else {
                if (temp_fd1[P_WRITE] > pipe_max_d)
                    pipe_max_d = temp_fd1[P_WRITE];
            }
            /*
             * Fill pipe fields
             * last buffer write to stdout
             */
            if (i != 0) {
                proxy_data[i].fd_read = temp_fd2[P_READ];
                proxy_data[i - 1].fd_write = temp_fd1[P_WRITE];
                fcntl(proxy_data[i - 1].fd_write, F_SETFL, O_NONBLOCK);
                fcntl(proxy_data[i].fd_read, F_SETFL, O_NONBLOCK);
            }
            if (i == 0) {
                proxy_data[i].fd_read = temp_fd2[P_READ];
                close(temp_fd1[P_WRITE]);
                fcntl(proxy_data[i].fd_read, F_SETFL, O_NONBLOCK);
            }
            if (i == (number - 1)) {
                proxy_data[i].fd_write = STDOUT_FILENO;

            }
        }
        /*
         * Child mode
         */ 
        else {
            /*
             * Close unusual pipes
             */
            fprintf(stderr, "Start child creation.\n");
            close(temp_fd1[P_WRITE]);
            close(temp_fd2[P_READ]);
            /*
             * Set first pipe as feli descriptor
             */
            if (i == 0) {
                close(temp_fd1[P_READ]);
                child_in = descriptor;
            } else
                child_in = temp_fd1[P_READ];
            
            child_out = temp_fd2[P_WRITE];     
            /*
             * Go out from "for" and start retranslate data
             */
            int j = 0;
            for (j = 0; j < i; j++) {
                close(proxy_data[j].fd_read);
                if ( j != 0 ) close(proxy_data[j - 1].fd_write);
            }
            fprintf(stderr, "End child creation.\n");
            break;
        }
    }
    /*
     * Prepare variable for manage
     */
    fd_set read_set, write_set;
    int success_checks = 1;
    /*
     * Parent mode
     */
    if (mode != 0) {
        /*int i = 0;
        for (i = 0; i < number; i++) {
            int cur_size = 0;
            cur_size = read(proxy_data[i].fd_read, proxy_data[i].data, PAGE_SIZE);
            write(proxy_data[i].fd_write, proxy_data[i].data, cur_size);
        }*/
        while (success_checks) {

            FD_ZERO(&read_set);
            FD_ZERO(&write_set);

            int i = 0;
            fprintf(stderr, "State: ");
            for (i = 0; i < number; i++ ){
                fprintf(stderr, "%d ", (int)(proxy_data[i].p_out - proxy_data[i].p_in));
            }
            fprintf(stderr, "\n");
            success_checks = 0;
            /*
             * Check pipes on exist and collect number of living pipes
             */
            for (i = 0; i < number; i++ ) {
                if (proxy_data[i].fd_read != 0 && \
                    (int)(proxy_data[i].p_out - proxy_data[i].p_in) != proxy_data[i].size) {
                    FD_SET(proxy_data[i].fd_read, &read_set);
                    success_checks++;
                }
                if (proxy_data[i].fd_write != 0 && \
                    (int)(proxy_data[i].p_out - proxy_data[i].p_in) != 0) {
                    FD_SET(proxy_data[i].fd_write, &write_set);
                    success_checks++;
                }
            }
            /*
             * If all pipes deleted then break "while"
             */
            if (success_checks == 0)
                break;
            if (select(pipe_max_d + 1, &read_set, &write_set, NULL, NULL) < 1)
                break;
                
            /*
             * Try to read from some pipe
             */
            for (i = 0; i < number; i++) {
                if (FD_ISSET(proxy_data[i].fd_read, &read_set) != 0) {
                    int cur_length = read(proxy_data[i].fd_read, proxy_data[i].p_out, \
                                          proxy_data[i].size - (int)(proxy_data[i].p_out - proxy_data[i].p_in));
                    //fprintf(stderr, "Current size-state(read): %d \n", cur_length);
                    if (cur_length > 0)
                        proxy_data[i].p_out += cur_length;
                    else {
                        close(proxy_data[i].fd_read);
                        proxy_data[i].fd_read = 0;
                    }
                }
            }
            /*
             * Try to write to some pipe
             */
            for (i = 0; i < number; i++) {
                if (FD_ISSET(proxy_data[i].fd_write, &write_set) != 0) {
                    int cur_length = write(proxy_data[i].fd_write, proxy_data[i].data, \
                                           proxy_data[i].p_out - proxy_data[i].p_in);
                    //fprintf(stderr, "Current size-state(write): %d \n", cur_length);
                    if (cur_length > 0) {
                        SHL_m(proxy_data[i].data, cur_length, proxy_data[i].p_out);
                        proxy_data[i].p_out -= cur_length;
                    }
                    if (cur_length < 1 && proxy_data[i].fd_read == 0 && \
                        proxy_data[i].p_out == proxy_data[i].data ) {
                        close(proxy_data[i].fd_write);
                        proxy_data[i].fd_write = 0;

                    }               
                }
            }
            /*
             * Close write pipe or no?
             */
            for (i = 0; i < number; i++) {
                if (proxy_data[i].fd_write != 0 && proxy_data[i].fd_read == 0 && \
                    proxy_data[i].p_out == proxy_data[i].data) {
                    close(proxy_data[i].fd_write);
                    proxy_data[i].fd_write = 0;
                } 
            } 
        }
    }
    /*
     * Child mode
     */
    else {
        char packet[PAGE_SIZE];
        int cur_size = 1;
        while (cur_size > 0) {
            cur_size = read(child_in, packet, PAGE_SIZE);
            write(child_out, packet, cur_size);
        }
    }
    sleep(5);
    return 0;   
}

