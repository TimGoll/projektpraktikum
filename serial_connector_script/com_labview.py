from __future__ import print_function
import serial, platform, glob, sys, threading
from time import *

serialConnection = None #global variable for connection
toWrite = True
i = 0
counter = 0

running = True

port = "COM9" #Mac: /dev/cu.usbmodem1421, Linux: /dev/tty_xxx

data = [
    '<4,16>',
    '<01,21,07,14>',
    '<Buerkert,Buerkert,MKS,MKS>',
    '<0x20>',
    '<0 0, 0 1, 0 2, 0 3, 0 4, 0 5, 0 6, 0 7, 0 8, 0 9, 0 10, 0 11, 0 12, 0 13, 0 14, 0 15>',
    '<250>',
    '<datumTODO>',
    '<begin>',

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
    '<M,2,0,11000>',        #Zeiliche Ordnung nicht wichtig, nur pro Geraet zu beachten
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
    '<V,7,1,27750>',
    '<V,8,1,28000>',
    '<V,9,1,28250>',
    '<V,10,1,28500>',
    '<V,11,1,28750>',
    '<V,12,1,29000>',
    '<V,13,1,29250>',
    '<V,14,1,29500>',
    '<V,15,1,29750>',

    '<V,0,0,29000>',
    '<V,1,0,29250>',
    '<V,2,0,29500>',
    '<V,3,0,29750>',
    '<V,4,0,30000>',
    '<V,5,0,30250>',
    '<V,6,0,30500>',
    '<V,7,0,30750>',
    '<V,8,0,31000>',
    '<V,9,0,31250>',
    '<V,10,0,31500>',
    '<V,11,0,31750>',
    '<V,12,0,32000>',
    '<V,13,0,32250>',
    '<V,14,0,32500>',
    '<V,15,0,32750>',

    '<M,1,750,36450>',
    '<M,2,20,37000>',
    '<M,3,520,40000>',

    '<V,0,1,40000>',
    '<V,1,1,40000>',
    '<V,2,1,40000>',
    '<V,3,1,40000>',
    '<V,4,1,40000>',
    '<V,5,1,40000>',
    '<V,6,1,40000>',
    '<V,7,1,40000>',
    '<V,8,1,40000>',
    '<V,9,1,40000>',
    '<V,10,1,40000>',
    '<V,11,1,40000>',
    '<V,12,1,40000>',
    '<V,13,1,40000>',
    '<V,14,1,40000>',
    '<V,15,1,40000>',

    '<V,0,0,41000>',
    '<V,1,0,41000>',
    '<V,2,0,41000>',
    '<V,3,0,41000>',
    '<V,4,0,41000>',
    '<V,5,0,41000>',
    '<V,6,0,41000>',
    '<V,7,0,41000>',
    '<V,8,0,41000>',
    '<V,9,0,41000>',
    '<V,10,0,41000>',
    '<V,11,0,41000>',
    '<V,12,0,41000>',
    '<V,13,0,41000>',
    '<V,14,0,41000>',
    '<V,15,0,41000>',

    '<end>'
]

ready = False
readline_running = True
class readline (threading.Thread):
    def run (self):
        global ready, readline_running;
        while (readline_running):
            in_data = str(serialConnection.readline())
            if (in_data.find("ready") != -1):
                ready = True
            print (in_data, end="")

read = readline()

def end_program():
    global serialConnection, running, readline_running

    running = False
    readline_running = False #stop readline Thread

    if (serialConnection != None): #close serial connection
        serialConnection.close()
    print("Killed connection.")

try:
    # INIT PROGRAM
    serialConnection = serial.Serial(port=port, baudrate=115200, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE,timeout=0)
    serialConnection.flushInput()

    read.setDaemon(True) #Daemon - thread stops after exiting main-thread
    read.start()

    print("LABVIEW CHANNEL:")

    while(not ready):
        sleep(0.075)
    # MAINLOOP
    sleep(1) #nach ready kurze Pause!
    while (running):
        if (toWrite == True):
            serialConnection.write(data[i] + "\n") #add '\n' (new line) to symbolize line ending
            print("[SENDE: " + data[i] + " ]")
            i+=1
            if (i == len(data)):
                toWrite = False

        sleep(0.05)
        counter+=1

except KeyboardInterrupt:
    end_program()
