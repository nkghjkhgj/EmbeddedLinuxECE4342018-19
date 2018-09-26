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
    // using GPIO port 1 to control led 1 at gpio49 at 23, using gpio port 3 to control led 2 at gpio115 at 25
    volatile void *gpio_addr_1;
    volatile void *gpio_addr_0;
    
    volatile unsigned int *gpio_oe_addr_1;
    volatile unsigned int *gpio_datain_1;
    volatile unsigned int *gpio_setdataout_addr_1;
    volatile unsigned int *gpio_cleardataout_addr_1;
    volatile unsigned int *gpio_oe_addr_0;
    volatile unsigned int *gpio_datain_0;
    volatile unsigned int *gpio_setdataout_addr_0;
    volatile unsigned int *gpio_cleardataout_addr_0;

    unsigned int reg_1;
    unsigned int reg_0;
    
    // Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

    int fd = open("/dev/mem", O_RDWR);

    gpio_addr_1 = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_START_ADDR);

    gpio_oe_addr_1           = gpio_addr_1 + GPIO_OE;
    gpio_datain_1            = gpio_addr_1 + GPIO_DATAIN;
    gpio_setdataout_addr_1   = gpio_addr_1 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr_1 = gpio_addr_1 + GPIO_CLEARDATAOUT;
    
    gpio_addr_0 = mmap(1, GPIO0_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_START_ADDR);

    gpio_oe_addr_0           = gpio_addr_0 + GPIO_OE;
    gpio_datain_0            = gpio_addr_0 + GPIO_DATAIN;
    gpio_setdataout_addr_0   = gpio_addr_0 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr_0 = gpio_addr_0 + GPIO_CLEARDATAOUT;
    
    printf("GPIO port 1 and port 0 are mapped\n");
    
    
    if(gpio_addr_1 == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    
    if(gpio_addr_0 == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }

    // Set USR3 and USR1 to be an output pin
    reg_1 = *gpio_oe_addr_1;
    reg_0 = *gpio_oe_addr_0;
    
    reg_0 &= ~GPIO_30;       // Set USR3 bit to 0
    reg_1 &= ~GPIO_60;       // Set USR1 bit to 0
    
    *gpio_oe_addr_1 = reg_1;
    *gpio_oe_addr_0 = reg_0;

    while(keepgoing) {
	if*(gpio_datain_1 & GPIO_60){        
        *gpio_setdataout_addr_1 = USR2;
        }
        *gpio_cleardataout_addr_1 = USR2;
	}
	if*(gpio_datain_0 & GPIO_30){        
        *gpio_setdataout_addr_0 = USR3;
        }
        *gpio_cleardataout_addr_0 = USR3;
	}	

    close(fd);
    return 0;
}





