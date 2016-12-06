# Dokumentation
## Akteulle Demonstration:
[Link zu YouTube Video](https://www.youtube.com/watch?v=2sz4_vMzZEc)

## Verwendete Bibliotheken:
1. [**mThread**](http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/):

 Erstellt Pseudothreads auf dem Board, die nacheinander ausgeführt werden. Jeder Thread hat seine eigene ```loop()```.

 Threads werden hinzugefügt mittels ```main_thread_list -> add_thread(CLASSNAME)```, anschließend laufen sie unbegrenzt weiter. **Hinweis:** Es sind scheinbar maximal nur 10 Threads möglich.

2. [**newdel**](https://github.com/jlamothe/newdel):

 Fügt die Keywörter ```new``` und ```delete``` hinzu. Wird für mThread benötigt und musste leicht angepasst werden, damit sie auf neueren Arduino-Boards funktioniert.

3. [**QueueList**](http://playground.arduino.cc/Code/QueueList):

 Fügt verkettete Listen hinzu, welche beliebige Datentypen speichern können. **Hinweis:** es ist nicht möglich Pointer in ihnen zu speichern.

## Übertragungsprotokoll (LabView -> Microcontroller):
Die Übertragung erfolgt Zeilenweise in einem möglichst Übertragungssicheren Format. Als Anfangs und Endzeichen dienen zur Überprüfung der Vollständigkeit jeweils ein öffnender und schließender Tag. Alle Zeitangaben sind in Millisekunden, es gibt nur ganzzahliger Integerwerte. Die IDs beginnen bei 0.

**Zeilenweise Betrachtung der Übertragung**:

1. ```<Anzahl MFC, Anzahl Ventile>```
2. ```<Adresse MFC 0, Adresse MFC 1, ...>```
3. ```<Typ MFC 0, Typ MFC 1, ...>```
4. ```<Ventil-Pin-0, Ventil-Pin-1, ...>```
5. ```<Messintervall>```
6. ```<beginn>``` Ende des Headers, Beginn mit der Eventübertragung
7. ```<MFC oder Ventil, ID, Wert, Zeit>``` Setze Events. Hierbei müssen die Events je MFC/Ventil zeitlich sortiert sein, um eine einfachere Verarbeitung zu gewährleisten. Untereinander dürfen die Events jedoch vertauscht sein. (Zeit von MFC2 darf vor MFC1 sein, auch bei späterer Übertragung. Jedoch darf Zeit von MFC1 nicht vor der Zeit von MFC1 sein)
8. ```<end>``` Ende der Eventübertragung, warte auf Start der Messung
9. ```<start>``` Nicht zwigend notwendig, kann auch händisch per Taster gestartet werden

**Beispiel**:

```
<6,9>
<adresse0,adress1,adresse2,adresse3,adresse4,adresse5>
<typ0,typ1,typ2,typ3,typ4,typ5>
<2,3,4,5,6,7,8,9,10>
<10>
<begin>
<M,0,120,1000>
<M,0,240,1500>
<M,1,170,800>
<M,0,150,1700>
...
<V,0,1,1200>
...
<end>
<start>
```

## Serielle Kommunikation:
Drei Ports des Boards werden verwendet. Port 0 dient zur Kommunikation mit LabView (IN/OUT), Port 1 gibt Debug-Nachrichten aus, sofern der Debug-Schalter am Board aktiviert ist. Zur Kommunikation mit den MFCs dient Port 2.

Es gibt eine extra Klasse namens ```serialCommunication```, welche die Kommunikation über die 3 Ports verwaltet. Zu beginn muss in der ```setup()```-Funktion der Hauptino die Klasse initialisiert werden:
```cpp
srl->setSerial(&Serial, &Serial1, &Serial2); //labview / debug / uart
```
Anschließend kann man an beliebiger Stelle im Programm diese Klasse einfach nutzen:
```cpp
srl->print('D', "Hallo Welt!"); //(Typ: L, D, U / Ausgabetext)
srl->println('D', "Hallo Welt mit Zeilenende!"); //Natuerlich gibt es das ganze auch mit Linebreak
```
Der Typ der Ausgabe entscheidet, welcher Port genutzt wird. Hierbei gibt es drei Typen: L, D und U für LabView, Debug und UART. Die Baudrate wird in der **config.h** eingestellt.

## Display:
Das Display ist via I2C mit dem Board verbunden, die Hintergrundbeleuchtung funktioniert mittels 3 PWM Anschlüssen für je eine Grundfarbe. Daraus können beliebige Hintergrundfarben gemischt werden.

Je nach Meldungstyp ist die Hintergrundfarbe unterschiedlich. Folgende Typen gibt es:

- **Unkritische Fehler**: Error Code Level 1000; Dies sind Fehler, die beispielsweise bei der Kommunikation auftreten, aber keinen Programmabbruch benötigen. Sie werden auf dem Display für 2 Sekunden angezeigt. Farbe: _gelb_
- **Kritische Fehler**: Error Code Level 5000; Diese Fehler führen zu einem Absturz des Programms und werden auf dem Display angezeigt bis der Fehler behoben wurde. Farbe: _rot_

Standardmäßig werden auf dem Display aktuelle Daten zum Programmstatus angezeigt.

## Programmaufbau:
### Hauptdatei:
1. **Controller.ino**:

 Hier werden die Hauptobjekte erstellt und verwaltet. Außerdem werden an dieser Stellte die Threads gestartet und später auch wieder gestoppt. Vor dem Start der Threads werden noch Adressen zwischen den einzelnen Objekten ausgetauscht (beispielsweise die der Queues zum späteren Datenaustausch).

### Hauptklassen:
Aus allen Klassen mit einem "main" im Namen wird immer nur **ein** Objekt abgeleitet. Außerdem besitzen sie eine ```loop()```-Funktion, da die Klasse in Pseudothreads ausgeführt wird.

1. **main_labCom** [[cpp]](../master/controller/src/main_labCom.cpp) [[h]](../master/controller/src/main_labCom.h):

 Quasi Hauptklasse des Programms, Verwaltet IN/OUT mit LabView
2. **main_boschCom** [[cpp]](../master/controller/src/main_boschCom.cpp) [[h]](../master/controller/src/main_boschCom.h):
3. **main_StoreD** [[cpp]](../master/controller/src/main_StoreD.cpp) [[h]](../master/controller/src/main_StoreD.h):
4. **main_mfcCtrl** [[cpp]](../master/controller/src/main_mfcCtrl.cpp) [[h]](../master/controller/src/main_mfcCtrl.h):

 Verwaltet alle MFC Objekte
5. **main_valveCtrl** [[cpp]](../master/controller/src/main_valveCtrl.cpp) [[h]](../master/controller/src/main_valveCtrl.h):

 Verwaltet alle Ventil Objekte

### Nebenklassen:
1. **mfcCtrl** [[cpp]](../master/controller/src/mfcCtrl.cpp) [[h]](../master/controller/src/mfcCtrl.h):
2. **valveCtrl** [[cpp]](../master/controller/src/valveCtrl.cpp) [[h]](../master/controller/src/valveCtrl.h):
3. **serialCommunication** [[cpp]](../master/controller/src/serialCommunication.cpp) [[h]](../master/controller/src/serialCommunication.h):

### Sonstige:
1. **config** [[h]](../master/controller/src/config.h):

 Einstellmöglichkeiten diverster Parameter
2. **eventElement** [[h]](../master/controller/src/eventElement.h):

 Event-Struct, welches von MFCs und Ventilen verwendet wird

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
