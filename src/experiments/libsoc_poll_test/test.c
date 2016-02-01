/* vim: set ts=4 sw=4 et ai : */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <libsoc_gpio.h>

#define GPIO_PIN 60

int count = 0;
int last_state = 0;

int timeval_cmp(struct timeval *a, struct timeval *b)
{
    /* Compare two struct timevals, returning -1 is a < b, +1 if a > b, or 0 if a == b. */
    if (a->tv_sec < b->tv_sec)
    {
        return -1;
    }
    else if (a->tv_sec > b->tv_sec)
    {
        return +1;
    }
    else
    {
        if (a->tv_usec < b->tv_usec)
        {
            return -1;
        }
        else if (a->tv_usec > b->tv_usec)
        {
            return +1;
        }
        else
        {
            return 0;
        }
    }
}

int main(int argc, char** argv)
{
    gpio *pin_in;

    pin_in = libsoc_gpio_request(GPIO_PIN, LS_SHARED);
    if (!pin_in)
    {
        perror("Failed to get GPIO");
        exit(1);
    }
    last_state = libsoc_gpio_get_level(pin_in);
    
    /* Loop for 5 seconds while continually polling */
    struct timeval deadline, current_time;
    int current_state;

    gettimeofday(&deadline);
    deadline.tv_sec += 5;
    
    for (;;)
    {
        gettimeofday(&current_time);
        if (timeval_cmp(&current_time, &deadline) >= 0)
        {
            break;
        }
        current_state = libsoc_gpio_get_level(pin_in);
        if (current_state != last_state)
        {
            count += 1;
            last_state = current_state;
        }
    }

    libsoc_gpio_free(pin_in);
    printf("%f\n", (float)count/2.0/5.0);
    return 0;
}
