#ifndef MAIN_STRINGBUILDER_H
#define MAIN_STRINGBUILDER_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "StoreD.h" //StoreD wird vom StringBuilder verwaltet und aufgerufen

#include "main_valveCtrl.h" //Objekte zum Speichern der Objektpointer
#include "main_mfcCtrl.h"
#include "main_boschCom.h"

#include "config.h"

#include "ownlibs/common.h"
#include "ownlibs/serialCommunication.h"

namespace communication {
    class Main_StringBuilder : public Thread {
    public:
        //Defaultconstructor
        Main_StringBuilder();
        //Destructor
        ~Main_StringBuilder();
        //setze das Intervall, in welchem die Hauptschleife ausgefuert wird.
        //Zeit ist gleich der des Sensors
        void setIntervall(uint16_t intervall);
        //aktiviere Klasse von LabCom aus, setzt die erste Intervallzeit
        void start(uint32_t time);
        //LabCom meldet, dass beide Queues abgearbeitet sind, Meldung wird auf dem Display
        //angezeigt
        void bothQueuesFinished ();
        //speichere Datumstring
        void setDateString(char dateString[]);
        //bekommt mfcTypen von parseInput und gibt sie an StoreD weiter
        void setTypes(char types[][16]);

        //Uebergebe Objektpointer
        void setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl);
        void setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl);
        void setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom);
        void setStoreDObjectPointer(storage::StoreD *storeD);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        bool ready;
        uint32_t lastTime;
        uint32_t startTime;
        uint16_t intervall;
        char dateString[32];

        float mfcValueList[MAX_AMOUNT_MFC];
        float mfcDestinationList[MAX_AMOUNT_MFC];
        uint8_t valveValueList[MAX_AMOUNT_VALVE];
        float* boschValues;

        char newLine[SERIAL_READ_MAX_LINE_SIZE];

        storage::StoreD *storeD; //Hier wird das StoreD-Objekt gespeichert

        //Diese Objekte werden gebraucht, um Daten fuer die Speicherung zu "sammeln"
        control::Main_ValveCtrl *main_valveCtrl;
        control::Main_MfcCtrl *main_mfcCtrl;
        communication::Main_BoschCom *main_boschCom;
    };
}

#endif
