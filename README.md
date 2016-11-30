# Dokumentation
## Verwendete Bibliotheken:
1. [**mThread**](http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/): 

 Erstellt Pseudothreads auf dem Board, die nacheinander ausgeführt werden. Jeder Thread hat seine eigene ```loop()```.
 
 Threads werden hinzugefügt mittels ```main_thread_list -> add_thread(CLASSNAME)```, anschließend laufen sie unbegrenzt weiter. **Hinweis:** Es sind scheinbar maximal nur 10 Threads möglich.

2. [**newdel**](https://github.com/jlamothe/newdel):
 
 Fügt die Keywörter ```new``` und ```delete``` hinzu. Wird für mThread benötigt und musste leicht angepasst werden, damit sie auf neueren Arduino-Boards funktioniert.
 
3. [**QueueList**](http://playground.arduino.cc/Code/QueueList):
 
 Fügt verkettete Listen hinzu, welche beliebige Datentypen speichern können. **Hinweis:** es ist nicht möglich Pointer in ihnen zu speichern.

## Programmaufbau:
### Hauptdatei:
1. **Controller.ino**:

 Hier werden die Hauptobjekte erstellt und verwaltet. Außerdem werden an dieser Stellte die Threads gestartet und später auch wieder gestoppt. Vor dem Start der Threads werden noch Adressen zwischen den einzelnen Objekten ausgetauscht (beispielsweise die der Queues zum späteren Datenaustausch).

### Hauptklassen:
1. **main_labCom**:
2. **main_boschCom**:
3. **main_StoreD**:
4. **main_mfcCtrl**:
5. **main_valveStrl**:

### Nebenklassen:
1. **mfcCtrl**:
2. **valveCtrl**:








------------------------------ ALT ------------------------------
 
# Projektpraktikum

## Hardware
1. **Teensy 3.6** Board [[link]](http://www.pjrc.com/teensy/)
  * Vorderseite [[link]](http://www.pjrc.com/teensy/beta/card9a_rev1_print.pdf)
  * Rückseite [[link]](https://www.pjrc.com/teensy/card9b_rev1.pdf)
2. SD Karte
3. Optokoppler
4. Pegelwandler
5. BME 280

## Nützliche Links
1. ThreadSafe-Queues [[link]](https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html)
2. Send Email [[link]](http://playground.arduino.cc/Code/Email)
3. Write to SD (kann bei diesem Board anders sein, Dokumentation lesen!) [[link]](https://www.arduino.cc/en/Tutorial/ReadWrite)
4. LabView-Network [[link]](http://www.ni.com/white-paper/2710/de/)
5. Arduino-Network-Communication [[link]](https://github.com/evothings/evothings-examples/blob/master/examples/arduino-led-onoff-tcp/arduinoethernet/arduinoethernet/arduinoethernet.ino)

Arduino kann kein Multithreading. Wie sieht es mit dem Teensy Board aus? Ansonsten: http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/

Klassiche Struktur in einem Arduino Programm: http://arduino.stackexchange.com/questions/13178/classes-and-objects-how-many-and-which-file-types-do-i-actually-need-to-use-the/13182#13182

Interruptsteuerung: http://playground.arduino.cc/Code/FrequencyTimer2

Queue-List (nicht Threadsafe, aber hier egal): http://playground.arduino.cc/Code/QueueList
