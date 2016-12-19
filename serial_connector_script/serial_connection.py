from __future__ import print_function
import serial, platform, glob, sys, threading
from time import *

serialConnection = None #global variable for connection
toWrite = True
i = 0
counter = 0

port = "COM4" #Mac: /dev/cu.usbmodem1421, Linux: /dev/tty_xxx

data = [
    '<4,7>',
    '<adresse0,adresse1,adresse2,adresse3>',
    '<buerkert,buerkert,buerkert,buerkert>',
    '<25,28,30,32,34,36,38>',
    '<25>',
    '<begin>',
    '<V,0,0,0>',
    '<V,1,0,0>',
    '<V,2,0,0>',
    '<V,3,0,0>',
    '<V,4,0,0>',
    '<V,5,0,0>',
    '<V,6,0,0>',
    '<M,0,120,1000>',
    '<M,0,220,2000>',
    '<M,0,20,7500>',
    '<M,0,420,10000>',
    '<M,1,10,150>',
    '<M,1,125,450>',
    '<V,0,1,1000>',
    '<V,1,1,1000>',
    '<V,2,1,1000>',
    '<V,3,1,1000>',
    '<V,0,0,3330>',
    '<V,1,0,3340>',
    '<V,2,0,3350>',
    '<V,3,0,3360>',
    '<M,2,120,2000>',
    '<M,2,520,2200>',
    '<M,2,740,3200>',
    '<M,2,250,4900>',
    '<M,2,230,5300>',
    '<M,2,170,7000>',
    '<M,1,225,1450>',
    '<M,1,250,6450>',
    '<M,2,0,11000>',
    '<M,3,120,2100>',
    '<M,3,520,2300>',
    '<M,3,740,3300>',
    '<M,3,250,5000>',
    '<M,3,230,5400>',
    '<M,3,170,7100>',
    '<M,3,225,7450>',
    '<M,3,250,8450>',
    '<M,3,0,11050>',
    '<V,0,0,15000>',
    '<V,4,1,12000>',
    '<V,5,1,18000>',
    '<V,6,1,22500>',

    '<V,0,0,25000>',
    '<V,1,0,25000>',
    '<V,2,0,25000>',
    '<V,3,0,25000>',
    '<V,4,0,25000>',
    '<V,5,0,25000>',
    '<V,6,0,25000>',

    '<V,0,1,26000>',
    '<V,1,1,26250>',
    '<V,2,1,26500>',
    '<V,3,1,26750>',
    '<V,4,1,27000>',
    '<V,5,1,27250>',
    '<V,6,1,27500>',

    '<V,0,0,27000>',
    '<V,1,0,27250>',
    '<V,2,0,27500>',
    '<V,3,0,27750>',
    '<V,4,0,28000>',
    '<V,5,0,28250>',
    '<V,6,0,28500>',

    '<end>',
    '<start>'
]

class readline (threading.Thread):
    def run (self):
        while (True):
            in_data = str(serialConnection.readline())
            print (in_data, end="")

read = readline()

def end_program():
    global serialConnection

    if (serialConnection != None): #close serial connection
        serialConnection.close()
    print("Killed connection.")

try:
    # INIT PROGRAM
    serialConnection = serial.Serial(port=port, baudrate=250000, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE)
    serialConnection.flushInput()

    read.setDaemon(True) #Daemon - thread stops after exiting main-thread
    read.start()

    # MAINLOOP
    while (True):
        if (toWrite == True):
            if (counter == 0):
                sleep(2)

            serialConnection.write(data[i] + "\n") #add '\n' (new line) to symbolize line ending
            i+=1
            if (i == len(data)):
                toWrite = False

        sleep(0.075)
        counter+=1

except KeyboardInterrupt:
    end_program()
