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
Aus allen Klassen mit einem "main" im Namen wird immer nur **ein** Objekt abgeleitet. Außerdem besitzen sie eine ```loop()```-Funktion, da die Klasse in Pseudothreads ausgeführt wird.

1. **main_labCom** [[cpp]](../master/controller/src/main_labCom.cpp) [[h]](../master/controller/src/main_labCom.h):
2. **main_boschCom** [[cpp]](../master/controller/src/main_boschCom.cpp) [[h]](../master/controller/src/main_boschCom.h):
3. **main_StoreD** [[cpp]](../master/controller/src/main_StoreD.cpp) [[h]](../master/controller/src/main_StoreD.h):
4. **main_mfcCtrl** [[cpp]](../master/controller/src/main_mfcCtrl.cpp) [[h]](../master/controller/src/main_mfcCtrl.h):
5. **main_valveCtrl** [[cpp]](../master/controller/src/main_valveCtrl.cpp) [[h]](../master/controller/src/main_valveCtrl.h):

### Nebenklassen:
1. **mfcCtrl** [[cpp]](../master/controller/src/mfcCtrl.cpp) [[h]](../master/controller/src/mfcCtrl.h):
2. **valveCtrl** [[cpp]](../master/controller/src/valveCtrl.cpp) [[h]](../master/controller/src/valveCtrl.h):

## LabView:

## Hardware:
1. **Teensy 3.6** Board [[link]](http://www.pjrc.com/teensy/)
 * Vorderseite [[link]](http://www.pjrc.com/teensy/beta/card9a_rev1_print.pdf)
 * Rückseite [[link]](https://www.pjrc.com/teensy/card9b_rev1.pdf)
2. **20x4 LCD Display (I2C, Buntes Backlight)** [[link]](https://www.adafruit.com/product/499)

[[**Schaltskizze als PDF**]](../master/electronic/Schalplan.pdf)

## Sonstiges:
1. **MarkdownGuide** [[link]](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)

---

ALT (zu überprüfen und zu entfernen)

--- 

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
