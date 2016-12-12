from __future__ import print_function

# import libraries
import serial, platform, glob, sys
from time import *

serialConnection = None #global variable for connection
toWrite = False
i = 0
counter = 0

data = [
    '<1,3>',
    '<adresse0>',
    '<buerkert>',
    '<2,3,4>',
    '<25>',
    '<begin>',
    '<V,0,120,1000>',
    '<M,0,220,2000>',
#    '<M,0,20,7500>',
#    '<M,0,420,10000>',
#    '<M,1,10,150>',
#    '<M,1,125,450>',
#    '<M,2,120,2000>',
#    '<M,2,520,2200>',
#    '<M,2,0,11000>',
    '<end>',
    '<start>'
]

def end_program():
    global serialConnection

    if (serialConnection != None): #close serial connection
        serialConnection.close()
    print("Killed connection.")

try:
    # INIT PROGRAM
    serialConnection = serial.Serial(port="COM5", baudrate=250000, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE)
    serialConnection.flushInput()

    # MAINLOOP
    while (True):
        if (toWrite == True):
            serialConnection.write(data[i] + "\n") #add '\n' (new line) to symbolize line ending
            i+=1
            if (i == len(data)):
                toWrite = False

        # READLINE: SerialData
        if (serialConnection.in_waiting > 0):
            in_data = str(serialConnection.readline())
            print (in_data, end="")

        if (counter == 100):
            toWrite = True

        sleep(0.01)
        counter+=1

except KeyboardInterrupt:
    end_program()
