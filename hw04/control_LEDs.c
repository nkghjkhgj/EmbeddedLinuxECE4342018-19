#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <signal.h>    // Defines signal-handling functions (i.e. trap Ctrl-C)
#include "beaglebone_gpio.h"

/****************************************************************
 * Global variables
 ****************************************************************/
int keepgoing = 1;    // Set to 0 when ctrl-c is pressed

/****************************************************************
 * signal_handler
 ****************************************************************/
void signal_handler(int sig);
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "\nCtrl-C pressed, cleaning up and exiting...\n" );
	keepgoing = 0;
}

int main(int argc, char *argv[]) {
    // using GPIO port 1 to control led 3, using gpio port 3 to control led 2
    volatile void *gpio_addr_1;
    volatile void *gpio_addr_3;
    
    volatile unsigned int *gpio_oe_addr_1;
    volatile unsigned int *gpio_setdataout_addr_1;
    volatile unsigned int *gpio_cleardataout_addr_1;
    volatile unsigned int *gpio_oe_addr_3;
    volatile unsigned int *gpio_setdataout_addr_3;
    volatile unsigned int *gpio_cleardataout_addr_3;

    unsigned int reg_1;
    unsigned int reg_3;
    
    // Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

    int fd = open("/dev/mem", O_RDWR);

    gpio_addr_1 = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_START_ADDR);

    gpio_oe_addr_1           = gpio_addr_1 + GPIO_OE;
    gpio_setdataout_addr_1   = gpio_addr_1 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr_1 = gpio_addr_1 + GPIO_CLEARDATAOUT;
    
    gpio_addr_3 = mmap(0, GPIO3_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO3_START_ADDR);

    gpio_oe_addr_3           = gpio_addr_3 + GPIO_OE;
    gpio_setdataout_addr_3   = gpio_addr_3 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr_3 = gpio_addr_3 + GPIO_CLEARDATAOUT;
    printf("GPIO port 1 and port 3 are mapped\n");
    
    
    if(gpio_addr_1 == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    
    if(gpio_addr_3 == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    
    
    // Set USR3 and USR2 to be an output pin
    reg_1 = *gpio_oe_addr_1;
    reg_3 = *gpio_oe_addr_3;
    
    reg_1 &= ~USR3;       // Set USR3 bit to 0
    reg_3 &= ~USR2;       // Set USR2 bit to 0
    
    *gpio_oe_addr_1 = reg_1;
    *gpio_oe_addr_3 = reg_3;
    
    printf("Start controlling USR3 and USR2\n");
    
    while(keepgoing) {
        if(*gpio_oe_addr_1 & GPIO_60){
            printf("button 1 pressed");
            *gpio_setdataout_addr_1 = USR3;
            usleep(250000);
        }
        *gpio_cleardataout_addr_1 = USR3;
            usleep(250000);
        if(*gpio_oe_addr_3 & GPIO_115){
            printf("button 2 pressed");
            *gpio_setdataout_addr_3 = USR2;
            usleep(250000);
        }
        *gpio_cleardataout_addr_3 = USR2;
            usleep(250000);
    }

    munmap((void *)gpio_addr_1, GPIO1_SIZE);
    munmap((void *)gpio_addr_3, GPIO3_SIZE);

    close(fd);
    return 0;
}
