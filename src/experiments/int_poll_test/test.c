/* vim: set ts=4 sw=4 et ai : */

/* Use GPIO with edge-triggered interrupt and poll() to wait for a transition on a pair of input pins. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/mman.h>

#define MAX_BITS 90
#define D0_VALUE_PATH "/sys/class/gpio/gpio115/value"
#define D1_VALUE_PATH "/sys/class/gpio/gpio112/value"
#define OUT_VALUE_PATH "/sys/class/gpio/gpio60/value"

struct state {
    int d0_fd;
    int d1_fd;
    int out_fd;
    struct pollfd *pfds;
    int num_pfds;
    char buffer[MAX_BITS+1];
    int cur_bit;
    int out_val;
    int last_d0_val;
    int last_d1_val;
};

int setup_pins(struct state *state);
int setup_realtime();
int wait_for_first_bit(struct state *state);
int wait_for_next_bit(struct state *state);
int append_bit(struct state *state, int d0_val, int d1_val);
int cleanup_pins(struct state *state);
int cleanup_realtime();

int main(int argc, char** argv)
{
    struct state state;
    int retval = 0;

    memset(&state, 0, sizeof(struct state));
    
    if (setup_realtime() != 0)
    {
        exit(1);
    }
    
    if (setup_pins(&state) != 0)
    {
        perror("Failed in setup_pins");
        exit(1);
    }
     
    /* get first bit */
    retval = wait_for_first_bit(&state);
    if (retval < 0)
    {
        cleanup_pins(&state);
        exit(1);
    }

    /* wait for additional bits */
    for (;;) {
        retval = wait_for_next_bit(&state);
        if (retval < 0)
        {
            cleanup_pins(&state);
            exit(1);
        }
        else if (retval == 0)
        {
            break;
        }
        /* else bit was found, so loop */
    }
    
    printf("Loop stopped with %d bits\n", state.cur_bit);
    printf("Loop returned retcode %d\n", retval);

    printf("Read code: %s (%d bits)\n", state.buffer, strlen(state.buffer));

    cleanup_pins(&state);
    cleanup_realtime();

    return 0;
}

int setup_pins(struct state *state)
{
    state->out_fd = open(OUT_VALUE_PATH, O_WRONLY | O_SYNC);
    if (state->out_fd < 0)
    {
        perror("Failed to open out value file");
        return -1;
    }
    state->d0_fd = open(D0_VALUE_PATH, O_RDONLY | O_NONBLOCK);
    if (state->d0_fd < 0)
    {
        perror("Failed to open D0 value file");
        return -1;
    }
    state->d1_fd = open(D1_VALUE_PATH, O_RDONLY | O_NONBLOCK);
    if (state->d1_fd < 0)
    {
        perror("Failed to open D1 value file");
        return -1;
    }
    state->pfds = (struct pollfd *)malloc(2*sizeof(struct pollfd));
    if (!state->pfds)
    {
        perror("Failed to allocated pollfd structs");
        return -1;
    }
    state->num_pfds = 2;
    state->pfds[0].fd = state->d0_fd;
    state->pfds[0].events = POLLPRI;
    state->pfds[1].fd = state->d1_fd;
    state->pfds[1].events = POLLPRI;
    
    return 0;
}

int wait_for_first_bit(struct state *state)
{
    int retval;
    char buf[2];
    int d0_val, d1_val;
    
    /* clear any outstanding interrupt on the pins */
    lseek(state->d0_fd, 0, SEEK_SET);
    lseek(state->d1_fd, 0, SEEK_SET);
    read(state->d0_fd, buf, 1);
    read(state->d1_fd, buf, 1);

    retval = poll(state->pfds, state->num_pfds, -1);
    if (retval < 0)
    {
        perror("Error occurred while polling D0/D1 values");
        return -1;
    }
    lseek(state->d0_fd, 0, SEEK_SET);
    lseek(state->d1_fd, 0, SEEK_SET);
    read(state->d0_fd, buf, 1);
    d0_val = (buf[0] == '1');
    read(state->d1_fd, buf, 1);
    d1_val = (buf[0] == '1');
    lseek(state->out_fd, 0, SEEK_SET);
    state->out_val = (state->out_val==0) ? 1 : 0;
    write(state->out_fd, (state->out_val==0)?"0":"1", 2);
    
    // printf("first bit: %d, %d\n", d0_val, d1_val);
    retval = append_bit(state, d0_val, d1_val);
    return retval;
}

int wait_for_next_bit(struct state *state)
{
    /* fast poll, burning CPU, so that we don't miss anything */
    int new_d0_val, new_d1_val;
    int retval;
    char buf[2];

    /* check constantly for new data, breaking if timeout of 5ms */
    struct timespec last_time, cur_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);
    for (;;)
    {
        clock_gettime(CLOCK_MONOTONIC, &cur_time);
        long int delta_ns;
        delta_ns = (cur_time.tv_sec - last_time.tv_sec)*1000000000;
        delta_ns += (cur_time.tv_nsec - last_time.tv_nsec);
        if (delta_ns > 5000000)
        {
            /* timeout */
            return 0;
        }
        lseek(state->d0_fd, 0, SEEK_SET);
        lseek(state->d1_fd, 0, SEEK_SET);
        read(state->d0_fd, buf, 1);
        new_d0_val = (buf[0] == '1');
        read(state->d1_fd, buf, 1);
        new_d1_val = (buf[0] == '1');

        lseek(state->out_fd, 0, SEEK_SET);
        state->out_val = (state->out_val==0) ? 1 : 0;
        write(state->out_fd, (state->out_val==0)?"0":"1", 2);
        
        /* has state changed? */
        if ((new_d0_val != state->last_d0_val) || (new_d1_val != state->last_d1_val))
        {
            /*
            lseek(state->out_fd, 0, SEEK_SET);
            state->out_val = (state->out_val==0) ? 1 : 0;
            write(state->out_fd, (state->out_val==0)?"0":"1", 2);
            */
            retval = append_bit(state, new_d0_val, new_d1_val);
            if (retval < 0)
            {
                return retval;
            }
            else
            {
                return 1;
            }
        }
    }
}

int append_bit(struct state *state, int d0_val, int d1_val)
{
    // printf("d0: %d, d1: %d\n", d0_val, d1_val);
    if (d0_val && d1_val)
    {
        fprintf(stderr, "Both D0 and D1 are high; reader may be disconnected.\n");
        return -1;
    }
    if (state->cur_bit >= MAX_BITS)
    {
        fprintf(stderr, "Too many bits read (%d)", MAX_BITS);
        return -1;
    }
    if (d0_val)
    {
        state->buffer[state->cur_bit] = '0';
        state->cur_bit++;
    }
    else if (d1_val)
    {
        state->buffer[state->cur_bit] = '1';
        state->cur_bit++;
    }
    state->last_d0_val = d0_val;
    state->last_d1_val = d1_val;
    return 0;
}

int cleanup_pins(struct state *state)
{
    if (state->pfds > 0)
    {
        free(state->pfds);
        state->pfds = NULL;
    }
    if (state->d0_fd > 0)
    {
        close(state->d0_fd);
        state->d0_fd = 0;
    }
    if (state->d1_fd > 0)
    {
        close(state->d1_fd);
        state->d1_fd = 0;
    }
    lseek(state->out_fd, 0, SEEK_SET);
    write(state->out_fd, "0", 2);
    close(state->out_fd);
}

int setup_realtime()
{
    int min_prio, max_prio, my_prio;
    int policy = SCHED_FIFO;
    struct rlimit my_rlimit_rttime = {100000, 100000}; /* max 100ms of realtime without yielding */
    if (setrlimit(RLIMIT_RTTIME, &my_rlimit_rttime)!=0)
    {
        perror("Failed to set rlimit");
        return -1;
    }
    my_prio = 10;
    struct sched_param my_sched_param = {my_prio};
    if (sched_setscheduler(0, policy, &my_sched_param)!=0)
    {
        perror("Failed to set scheduler");
        return -1;
    }
    if (mlockall(MCL_CURRENT|MCL_FUTURE)!=0)
    {
        perror("Failed to lock all memory pages");
        return -1;
    }
    return 0;
}

int cleanup_realtime()
{
    return 0;
}
