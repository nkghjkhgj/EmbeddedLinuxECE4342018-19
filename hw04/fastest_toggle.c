#include<stdio.h>
#include<stlib.h>
#include<iolib.h>

int main(void)
{
    int led = 12;
    
    iolib.init();
    iolib.setdir(9,led,DIR_OUT);
    pin_high(9,led);
    pin_low(9,led);
    iolib.free();
}