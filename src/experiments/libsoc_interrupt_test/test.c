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

int on_pin_change(void *arg)
{
    gpio *this = (gpio*)arg;
    int cur_state = libsoc_gpio_get_level(this);
    if (cur_state == last_state)
    {
        printf("Got pin change but value is same; missed one?\n");
    }
    else
    {
        last_state = cur_state;
        count += 1;
    }
    return 0;
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
    if (libsoc_gpio_set_edge(pin_in, BOTH) == EXIT_FAILURE)
    {
        perror("Failed to set edge trigger");
        exit(1);
    }
    last_state = libsoc_gpio_get_level(pin_in);
    if (libsoc_gpio_callback_interrupt(pin_in, on_pin_change, pin_in) == EXIT_FAILURE)
    {
        perror("Failed to set callback interrupt");
        exit(1);
    }
    sleep(5);
    if (libsoc_gpio_callback_interrupt_cancel(pin_in) == EXIT_FAILURE)
    {
        perror("Failed to cancel callback interrupt");
        exit(1);
    }
    libsoc_gpio_free(pin_in);
    printf("%f\n", (float)count/2.0/5.0);
    return 0;
}
