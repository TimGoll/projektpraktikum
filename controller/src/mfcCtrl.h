#ifndef MFCCTRL_H
#define MFCCTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h>

#include "ownlibs/serialCommunication.h"
#include "eventElement.h"
#include "main_display.h"

#include "mfcCom.h"

namespace control {
    // Dies ist die Klasse fuer die MFCs. Jeder MFC bekommt seine eigene Instanz
    // diser Klasse, da dies dann deutlich einfacher zu handhaben ist. Die Objekte
    // werden in main_mfcCtrl erstellt und nach VOLLSTAENDIGER Beendigung der Messung
    // auch wieder geloescht.
    class MfcCtrl {
    public:
        //Defaultconstructor
        MfcCtrl(uint16_t id);
        //Destructor
        ~MfcCtrl();
        //Es gibt zwei verschiedene Typen von MFCs, der Typ muss vorher gesetzt werden
        void setType(char type[]);
        //Jeder MFC hat seine eigene Adresse, die gesetzt werden muss
        void setAddress(char address[]);
        //Stellwerte fuer die MFCs koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(uint16_t value, uint32_t time);
        //sobald diese Funktion ausgefuehrt wird, beginnt das Programm mit der Ansteuerung
        void start(uint32_t startTime);
        //Gebe Adresse des Displayobjektes an diesen MFC, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Gibt den aktuellen Soll-Wert des MFCs zurueck
        uint16_t getCurrentValue();
        //Die compute()-Function wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool compute();
        //schreibt den Typ dieses MFCs in einen String
        void getType(char type[]);
    private:
        uint16_t id; //kontunierliche MFC-Id
        char type[16];
        char address[16];
        bool ready;
        uint32_t startTime;
        int16_t currentValue;

        QueueList <eventElement> eventList;
        eventElement nextEvent;

        io::Main_Display *main_display;
    };
}

#endif
