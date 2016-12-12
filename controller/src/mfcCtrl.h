#ifndef MFCCTRL_H
#define MFCCTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h>

#include "ownlibs/serialCommunication.h"
#include "eventElement.h"
#include "main_display.h"

namespace control {
    // Dies ist die Klasse fuer die MFCs. Jeder MFC bekommt seine eigene Instanz
    // diser Klasse, da dies dann deutlich einfacher zu handhaben ist. Die Objekte
    // werden in LabCom(??) erstellt und nach VOLLSTAENDIGER Beendigung der Messung
    // auch wieder geloescht.
    class MfcCtrl {
    public:
        //Defaultconstructor
        MfcCtrl(int id);
        //Destructor
        ~MfcCtrl();
        //Es gibt zwei verschiedene Typen von MFCs, der Typ muss vorher gesetzt werden
        void setType(char type[]);
        //Jeder MFC hat seine eigene Adresse, die gesetzt werden muss
        void setAdress(char adress[]);
        //Stellwerte fuer die MFCs koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(int value, unsigned long time);
        //sobald diese Funktion ausgefuehrt wird, beginnt das Programm mit der Ansteuerung
        void start(unsigned long startTime);
        //Gebe Adresse des Displayobjektes an diesen MFC, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Die compute()-Function wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool compute();
    private:
        int id; //kontunierliche MFC-Id
        char type[16];
        char adress[16];
        QueueList <eventElement> eventList;
        bool ready;
        unsigned long startTime;

        eventElement nextEvent;

        io::Main_Display *main_display;
    };
}

#endif
