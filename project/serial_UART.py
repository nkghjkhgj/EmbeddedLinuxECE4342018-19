#!/usr/bin/env python
import Adafruit_BBIO.UART as UART
import serial
import time
import binascii



# ser1 = serial.Serial(port = "/dev/ttyS1", baudrate = 9600)
s = serial.Serial("/dev/ttyS1")
while true:
    # s.write(open("target.txt","rb").read())
    s.write("Hello World")

