#ifndef mfcCtrl_h
#define mfcCtrl_h

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <QueueList.h>

namespace control {
    typedef struct eventElementStruct {
        int value;
        unsigned int time;
    } eventElement;


    // Dies ist die Klasse fuer die MFCs. Jeder MFC bekommt seine eigene Instanz
    // diser Klasse, da dies dann deutlich einfacher zu handhaben ist. Die Objekte
    // werden in LabCom(??) erstellt und nach VOLLSTAENDIGER Beendigung der Messung
    // auch wieder geloescht.
    class MfcCtrl : public Thread {
    public:
        //Defaultconstructor
        MfcCtrl();
        //Destructor
        ~MfcCtrl();
        //Es gibt zwei verschiedene Typen von MFCs, der Typ muss vorher gesetzt werden
        void setType(char type[]);
        //Jeder MFC hat seine eigene Adresse, die gesetzt werden muss
        void setAdress(char adress[]);
        //Stellwerte fuer die MFCs koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(int value, unsigned int time);
        //sobald diese Funktion ausgefuehrt wird, beginnt das Programm mit der Ansteuerung
        void start();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int id; //kontunierliche MFC-Id
        char type[];
        char adress[];
        QueueList <eventElement*> eventList;
        bool ready;

        eventElement *nextEvent;
    };
}

#endif
