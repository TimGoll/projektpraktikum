from __future__ import print_function
import serial, platform, glob, sys, threading
from time import *

serialConnection = None #global variable for connection
port = "COM8" #Mac: /dev/cu.usbmodem1421, Linux: /dev/tty_xxx

running = True

def end_program():
    global serialConnection, running

    running = False

    if (serialConnection != None): #close serial connection
        serialConnection.close()
    print("Killed connection.")

try:
    # INIT PROGRAM
    serialConnection = serial.Serial(port=port, baudrate=115200, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE, timeout=0)
    serialConnection.flushInput()

    print("MFC CHANNEL:")

    while (running):
        in_data = str(serialConnection.readline())
        print (in_data, end="")
        sleep(0.05)

except KeyboardInterrupt:
    end_program()
