# Dokumentation des Projektpraktikums
Projektpraktikum (Mikrokontrollerpraktikum + Messtechnikpraktikum) Wintersemester 2016/17 am LMT

_Tim Goll, Matthias Baltes, Matthias Jost, Markus Herrmann-Wicklmayr_

## Aktuelles Demonstrationsvideo:
[Link zu YouTube Video](https://www.youtube.com/watch?v=2sz4_vMzZEc)

## Verwendete Bibliotheken:
1. [**mThread**](http://www.kwartzlab.ca/2010/09/arduino-multi-threading-librar/): <br>
 Erstellt Pseudothreads auf dem Board, die nacheinander ausgeführt werden. Jeder Thread hat seine eigene ```loop()```. <br>
 Threads werden hinzugefügt mittels ```main_thread_list -> add_thread(CLASSNAME)```, anschließend laufen sie unbegrenzt weiter. **Hinweis:** Es sind scheinbar maximal nur 10 Threads möglich.

2. [**newdel**](https://github.com/jlamothe/newdel): <br>
 Fügt die Keywörter ```new``` und ```delete``` hinzu. Wird für mThread benötigt und musste leicht angepasst werden, damit sie auf neueren Arduino-Boards funktioniert.

3. [**QueueList**](http://playground.arduino.cc/Code/QueueList): <br>
 Fügt verkettete Listen hinzu, welche beliebige Datentypen speichern können. **Hinweis:** es ist nicht möglich Pointer in ihnen zu speichern.

## Programmablauf:
1. Programm startet nach Öffnen der Seriellen Verbindung (Reset erfolgt automatisch auf dem Arduino; zu schauen, wie dies auf dem Teensy zu erreichen ist, eventuell Reset-Pin vom USB<->UART-Chip abgreifen (sofern vorhanden)?)
2. Programm empfängt Daten (werden überprüft) und startet nach einem Tasterdruck/"start"-Befehl
3. Arbeitet Eventlisten ab, gibt Debug-Informationen aus, sofern über Schalter aktiviert, während der Laufzeit Einstellbar
4. Im Idle-Modus nach Vollständiger Abarbeitung aller Events; es wäre möglich eine LED leuchten zu lassen, wenn das Programm fertig ist

## Übertragungsprotokoll (LabView -> Microcontroller):
Die Übertragung erfolgt Zeilenweise in einem möglichst Übertragungssicheren Format. Als Anfangs und Endzeichen dienen zur Überprüfung der Vollständigkeit jeweils ein öffnender und schließender Tag. Alle Zeitangaben sind in Millisekunden, es gibt nur ganzzahliger Integerwerte. Die IDs beginnen bei 0.

**Zeilenweise Betrachtung der Übertragung**:

1. ```<Anzahl MFC, Anzahl Ventile>```
2. ```<Adresse MFC 0, Adresse MFC 1, ...>```
3. ```<Typ MFC 0, Typ MFC 1, ...>```
4. ```<Ventil-Pin-0, Ventil-Pin-1, ...>```
5. ```<Messintervall>```
6. ```<Datum + Zeit>``` Aktuelles Datum & Uhrzeit. Wird im Header der Messung gespeichert, relativ dazu wird gemessen
7. ```<begin>``` Ende des Headers, Beginn mit der Eventübertragung
8. ```<MFC oder Ventil, ID, Wert, Zeit>``` Setze Events. Hierbei müssen die Events je MFC/Ventil zeitlich sortiert sein, um eine einfachere Verarbeitung zu gewährleisten. Untereinander dürfen die Events jedoch vertauscht sein. (Zeit von MFC2 darf vor MFC1 sein, auch bei späterer Übertragung. Jedoch darf Zeit von MFC1 nicht vor der Zeit von MFC1 sein)
9. ```<end>``` Ende der Eventübertragung, warte auf Start der Messung
10. ```<start>``` Nicht zwigend notwendig, kann auch händisch per Taster gestartet werden

**Beispiel**:

```
<6,9>
<adresse0,adress1,adresse2,adresse3,adresse4,adresse5>
<typ0,typ1,typ2,typ3,typ4,typ5>
<2,3,4,5,6,7,8,9,10>
<10>
<08.02.2017 13:03:29>
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

Über den LabView Port wird nach Erfolgreicher Initialisierung des Boards ein "ready" gesendet. Wurde Ein Befehl korrekt erkannt und erfolgreich verarbeitet wird ein "ok" gesendet, ansonsten kommt ein Errorcode.

## Serielle Hardware
Die Verbindung zwischen dem Teensy und dem PC über Serielle Verbindung wird mittels dem IC **CH340G** realisiert. Dieser funktioniert Plug and Play unter Windows (Windows 10 getestet) und lässt sich auch [mit einfachen Treibern](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver) unter MacOS zum Laufen bringen.

**--- ALT ---**<br>
Die Verbindung zwischen dem Teensy und dem PC über Serielle Verbindung ist daher etwas schwer, da das Board keine eigene Möglichkeit der Kommunikation bietet. Abhilfe schafft jedoch der **Prolific PL2303HX** IC, welcher ein Uart Signal zu einem USB-Signal wandelt und dem PC ein USB-Device simuliert. Dieser Chip ist stanndardmäßig nicht mit Windowsversionen neuer als Windows 8 kompatibel, doch ein [inofizieller Treiber](http://www.ifamilysoftware.com/news37.html) schafft Abhilfe. <br>
Wieso wir diesen Chip dennoch genommen haben? - Ganz einfach, er wird in den meisten käuflich erhältlichen USB<->Uart bauteilen verwendet und somit gibt es auch am meisten Informationen zu diesem.

Die Treiber befinden sich auch in diesem GitHub Repository.<br>
**--- ALT ENDE ---**

## StoreD und die Übertragung an LabView
Beim Speichern auf der SD-Karte wird eine Datei mit einem Dateinamen, der das Datum und die Messungsnummer (falls mehrere Messungen pro Tag) enthält. Dabei wird beim erstellen der txt-Datei ein Header in Klartext erstellt. Dieser enthält wesentliche Informationen wie die Startuhrzeit der Messungen, die Anzahl der MFC's und Ventile, sowie die Typen der MFC's. Nach dem Header folgt eine Leerzeile und dann die Messdaten in codierte Form.

Dabei werden die Daten auf die sogenannte "pseudobyteartige" Weise abgespeichert, was im Grunde eine Umwandlung der Messdaten vom 10er- in das 256er-System entspricht. Dabei enstpricht das ASCII-Zeichen mit der Ordnungszahl n mit n=[0,1,..255] der Zahl n. Die Anzahl der reservierten Bytes pro Messwert (bytesize) können für jeden Messwert individuell festgelegt werden. Nach jedem Datensatz wird in eine neue Zeile gesprungen. Dies hat zum einen eine optimale Speichernutzung zur Folge, zum anderen entfällt die Notwendigkeit eines Seperationszeichen zwischen den Messwerten eines Datensatzes.

Nach der Messung können die Daten am PC m.H. von bytesize wieder wieder zerlegt werden. Danach können die Daten ausgewertet werden.

## Display:
Das Display ist via I2C mit dem Board verbunden, die Hintergrundbeleuchtung funktioniert mittels 3 PWM Anschlüssen für je eine Grundfarbe. Daraus können beliebige Hintergrundfarben gemischt werden. <br>
Je nach Meldungstyp ist die Hintergrundfarbe unterschiedlich. Folgende Typen gibt es:

- **Unkritische Fehler**: Error Code Level 1000; Dies sind Fehler, die beispielsweise bei der Kommunikation auftreten, aber keinen Programmabbruch benötigen. Sie werden auf dem Display für 2 Sekunden angezeigt. Farbe: _gelb_
- **Kritische Fehler**: Error Code Level 5000; Diese Fehler führen zu einem Absturz des Programms und werden auf dem Display angezeigt bis der Fehler behoben wurde. Farbe: _rot_

Standardmäßig werden auf dem Display aktuelle Daten zum Programmstatus angezeigt.

**Beispielanzeige**:
```
STANDARD:                      ERRORBEISPIEL:                 ENDANZEIGE:
+----------------------+       +----------------------+       +----------------------+
|          #M:04 #V:07 |       |      ERROR 1001      |       |          #M:04 #V:07 |
|                      |       |                      |       |                      |
| LAUFZEIT:00:01:34:17 |       |       Falscher       |       |    ABGESCHLOSSEN     |
| V03-0001-00:01:16:22 |       |     Zeilenbeginn     |       |     02:05:24:34      |
+----------------------+       +----------------------+       +----------------------+
Hintergrund: weiß              Hintergrund: organge/rot       Hintergrund: grün
```

- **Standardanzeige:** <br>
 Zeile 1: Anzahl MFCs und Anzahl Ventile <br>
 Zeile 2: xxx <br>
 Zeile 3: Laufzeit der Software, relativ zum Messstart <br>
 Zeile 4: Letztes Event, Typ_ID-Wert-Zeit
- **Erroranzeige:** <br>
 Zeile 1: Error ID <br>
 Zeile 2: xxx <br>
 Zeile 3+4: Kurzbeschreibung des augetretenen Fehlers
- **Endanzeige:** <br>
 Zeigt durchgängig diesen Text an, lässt sich nur zurücksetzen durch Reset des Boards

## Errormeldungen:
Bei allen Fehlermeldungen im 1000er Bereich wird das Programm weiterhin ausgeführt, es wird jedoch eine Wiederholung der entsprechenden Zeile erwartet, daher sind diese Codes LabView-Seitig abzufangen.

### 1000:
**Maximale Input-String-Länge überschritten.** In der _config.h_ wird eine Größe definiert, die pro Zeile gesendet werden darf. Diese Länge darf nicht überschritten werden.

### 1001:
**Falscher Zeilenbeginn.** Die Zeile muss mit einem öffenenden Tag ```<``` begonnen werden, damit sie als gültig akzeptiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1002:
**Falsches Zeilenende.** Die Zeile muss mit einem schließenden Tag ```>``` beendet werden, damit sie als gültig akzeptiert wird. Dabei darf man jedoch nicht vergessen, dass die Stringeingabe mit einem Zeilenumbruch ```\n``` als Vollständig markiert wird. Dies dient zur Vollständigkeitsüberprüfung.

### 1003:
**Lese-Timeout überschritten.** In der _config.h_ wird eine maximale Lesezeit pro String definiert. Wird diese Zeit überschritten, wird das Lesen des Strings an dieser Stelle abgebrochen.

### 5000:
**Zufriff auf nicht definierte MFC/Ventil ID.** Trifft dieser Fall ein, dann wird eine irreversible Errormeldung geworfen, die nur duch einen Programmneustart behoben werden kann. Man sollte in diesem Fall seine Eingaben überprüfen, ob in den Events nur auf vorher definierte MFCs/Ventile zugegriffen wird.

## Programmaufbau:
### Hauptdatei:
1. **Controller.ino**: [[ino]](../master/controller/controller.ino) <br>
 Hier werden die Hauptobjekte erstellt und verwaltet. Außerdem werden an dieser Stellte die Threads gestartet und später auch wieder gestoppt. Vor dem Start der Threads werden noch Adressen zwischen den einzelnen Objekten ausgetauscht (beispielsweise die der Queues zum späteren Datenaustausch).

### Hauptklassen:
Aus allen Klassen mit einem "main" im Namen wird immer nur **ein** Objekt abgeleitet. Außerdem besitzen sie eine ```loop()```-Funktion, da die Klasse in Pseudothreads ausgeführt wird.

1. **main_labCom** [[cpp]](../master/controller/src/main_labCom.cpp) [[h]](../master/controller/src/main_labCom.h): <br>
 Quasi Hauptklasse des Programms, verwaltet IN/OUT mit LabView

2. **main_boschCom** [[cpp]](../master/controller/src/main_boschCom.cpp) [[h]](../master/controller/src/main_boschCom.h):
3. **main_stringBuilder** [[cpp]](../master/controller/src/main_stringBuilder.cpp) [[h]](../master/controller/src/main_stringBuilder.h): <br>
 Diese Klasse sammelt sich per Abfragen alle Daten zusammen und baut im Messtakt daraus Strings, welche an LabCom und StoreD weiter gegeben werden. Diese Klasse erzeugt und verwaltet StoreD.

4. **main_mfcCtrl** [[cpp]](../master/controller/src/main_mfcCtrl.cpp) [[h]](../master/controller/src/main_mfcCtrl.h): <br>
 Verwaltet alle mfcCtrl Objekte

5. **main_valveCtrl** [[cpp]](../master/controller/src/main_valveCtrl.cpp) [[h]](../master/controller/src/main_valveCtrl.h): <br>
 Verwaltet alle valveCtrl Objekte

6. **main_display** [[cpp]](../master/controller/src/main_display.cpp) [[h]](../master/controller/src/main_display.h):

### Nebenklassen:
1. **mfcCtrl** [[cpp]](../master/controller/src/mfcCtrl.cpp) [[h]](../master/controller/src/mfcCtrl.h):
2. **valveCtrl** [[cpp]](../master/controller/src/valveCtrl.cpp) [[h]](../master/controller/src/valveCtrl.h):
3. **StoreD** [[cpp]](../master/controller/src/StoreD.cpp) [[h]](../master/controller/src/StoreD.h): <br>
 Wird in main_stringBuilder erstellt und verwaltet, sorgt dafür, dass Daten auf der SD Karte gespeichert werden.
4. **serialCommunication** [[cpp]](../master/controller/src/ownlibs/serialCommunication.cpp) [[h]](../master/controller/src/ownlibs/serialCommunication.h):

### Sonstige:
1. **common** [[cpp]](../master/controller/src/ownlibs/common.cpp) [[h]](../master/controller/src/ownlibs/common.h): <br>
 Standardfunktionen, die Überall gebraucht werden (```trim()```, ```getTimeString(time, timeString)```, ...)

2. **config** [[h]](../master/controller/src/config.h): <br>
 Einstellmöglichkeiten diverster Parameter.

3. **eventElement** [[h]](../master/controller/src/eventElement.h): <br>
 Event-Struct, welches von MFCs und Ventilen verwendet wird.

3. **errors** [[cpp]](../master/controller/src/errors.cpp) [[h]](../master/controller/src/errors.h): <br>
 CharArray, in welchem die Displayausgaben der einzelnen Errormeldungen gespeichert sind.

## Testskript [[py]](../master/serial_connector_script/serial_connection.py)
Derzeit gibt es ein kleines Testskript zum Test der Steuerungssoftware auf dem Board. Bei der Ausführung ist zu beachten, dass keine andere Serielle Verbindung geöffnet sein darf (Arduino-Debug-Monitor, ...).

Zur Ausführung müssen ```Python 2.7``` und ```pySerial``` installiert sein und bestenfalls die PATH-Variable für die Konsole gesetzt sein. Da das Programm hardgecoded ist, muss der Port in Zeile 10 angepasst werden. Der Port ist beispielsweise in der Arduino IDE sehr einfach einzusehen.

Ist alles vorbereitet wird das Skript mit ```python serial_connection.py``` ausgeführt, insofern man sich im Verzeichnis dieser Datei befindet. In der Datei kann in einem Array die Übertragung definiert werden.

[[Einrichtung von Python (Windows)]] (https://learn.adafruit.com/arduino-lesson-17-email-sending-movement-detector/installing-python-and-pyserial)

## LabView:
@Matthias Baltes: TODO!


## Elektronik:
### Hardware
1. Schaltkizze: [[link]](../master/electronic/Schalplan.pdf)
2. Verwendete Bauteile auf dem Haupboard:
 * **Teensy 3.6** Board [[link]](http://www.pjrc.com/teensy/) <br>
  Vorderseite [[link]](http://www.pjrc.com/teensy/beta/card9a_rev1_print.pdf) <br>
  Rückseite [[link]](https://www.pjrc.com/teensy/card9b_rev1.pdf)
 * **20x4 LCD Display (Buntes Backlight)** [[link]](https://www.adafruit.com/product/499) <br>
  RGB-LCD Pin Header [[link]](https://cdn-shop.adafruit.com/datasheets/WH2004A-CFH-JT%23.pdf) <br>
  I2C-Chip für Display (PCF8574) [[link]](http://www.sunfounder.com/wiki/images/1/18/PCF8574T_datasheet.pdf)
3. Weitere Dinge:
 * **Wandlung "TTL" (3V3) auf RS-232** (Teensy auf RS232 für die MFCs) [[link]](http://www.ti.com/lit/ds/symlink/max232.pdf) <br>
  Dazu ist beispielsweise der MAX232 nötig, da dieser sich selber seine +/- 12V erzeugt <br>
  Außerdem ist ein 3V3 auf 5V Pegelwandler zwischen Teensy und MAX232 zu schalten
 * **Anschließen der Ventile** [[link]](http://www.nxp.com/documents/data_sheet/PCA9539A.pdf) <br>
  Werden über I2C angesteuert (16bit I2C GPIO-Expander dient als Erweiterung) <br>
  Mithilfe dessen ist es möglich mehrere dieser "x16-Ventile"-Boards anzuschließen (mit diesem IC 4, da er 4 verschiedene Adresskombinationen unterstützt)<br>
  Dieser IC läuft auch auf 3V3, lässt sich also OHNE Pegelwandler betreiben<br>
  Hängt mit allen anderen I2C Geräten an Bus 0
 * **Ventilansteuerung** [[link]](http://www.infineon.com/dgdl/Infineon-BTS555-DS-v01_00-en.pdf?fileId=db3a30432ba3fa6f012bd3dfdd0b3b65) <br>
  Die Ventile brauchen 24V, 2A. Um dies zu schalten, nutzen wir diese PROFETs (Haben Kurzschluss und Überspannungsschutz eingebaut, sowie eine Schutzdiode)<br>
  Brauchen eine Inverterschaltung mit Pull-Up Widerstand davor (die PROFETs sind active Low), hier nutzen wir den BC546 (Bipolar)<br>
  Die x16 Platine wird an 24V, 3V3, GND, SDA und SCL angeschlossen und lässt sich mit weiteren parallel schalten.
 * **UART <-> USB** <br>
  Hier ist noch eine bessere Lösung, als dies aktuell der Fall ist, zu finden

### Schematic
#### Ventilsteuerung (links unten)
Jedes Ventil wird einzeln über einen digitalen Pin vom Mikorcontroller angesteuert. Zum schalten werden zur Zeit BS170 Transistoren verwendet.
Die Versorungsspannung der Ventile liegt bei 24V.
PINS 21 bis 36

#### USB Anschlüsse (links oben)
Aktuell sind die USB Anschlüsse noch über Pegelwandler am Board angeschlossen. Später wird noch jeweils ein Chip benltigt um die Kommunikation zu ermöglichen.
USB1: TX1 PIN 0, RX1 PIN1
USB2: TX3 PIN 7, RX3 PIN8

#### LED Anzeigen (rechts unten)
Eine LED um eine laufende Messung anzuzeigen. Schalter auf PIN 38, LED auf PIN 40
Eine LED um eine Debugging Session anzuzeigen. Schalter auf PIN 37, LED auf PIN 39

#### Display (rechts oben)
PIN 		Belegung

1.	GND
2.	5V VCC
3.	Poti(Dimmer)
4.	PCF8547 Setup
5.	PCF8547 Setup
6.	PCF8547 Setup
7.	nicht angeschlossen
8.	nicht angeschlossen
9.	nicht angeschlossen
10.	nicht angeschlossen
11.	PCF8547 Data
12.	PCF8547 Data
13.	PCF8547 Data
14.	PCF8547 Data
15.	5V LED-VCC
16.	PWM Teensy 4
17.	PWM Teensy 3
18.	PWM Teensy 2

#### PCF8547 (mitte rechts vom Teensy)
Vom Teensy kommen über einen SDA und eine SCL PIN Daten, werden in einem Schieberegister abgelegt
und nach erhalt von 8 Bits werden diese über die Ports P0 bis P7 weitergereicht (in diesem Fall an
die Dispaly Ports 4,5,6 und 11,12,13,14. (Einer zu wenig?)

## Sonstiges:
1. **MarkdownGuide** [[link]](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
2. **cloc-1.70.exe** [[link]](https://github.com/AlDanial/cloc)

 Ein kleines Programm zur Ausführung in der Konsole, welches den Softwareumfang darstellt. Wird wie folgt benutzt: ```prompt> cloc-1.70.exe controller/src``` (Auszuführen im Basepath des Projektes, dort liegt auch die exe)

 Ergibt folgendes:
 ```
 prompt> cloc-1.70.exe controller/src
       24 text files.
       24 unique files.
        2 files ignored.

 https://github.com/AlDanial/cloc v 1.70  T=0.50 s (48.0 files/s, 2734.0 lines/s)
 -------------------------------------------------------------------------------
 Language                     files          blank        comment           code
 -------------------------------------------------------------------------------
 C++                             11            185             50            665
 C/C++ Header                    13             72             88            307
 -------------------------------------------------------------------------------
 SUM:                            24            257            138            972
 -------------------------------------------------------------------------------
 ```
