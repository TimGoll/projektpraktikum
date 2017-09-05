#ifndef MAIN_VALVECTRL_H
#define MAIN_VALVECTRL_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "config.h"
#include "valveCtrl.h"
#include "main_display.h"
#include "ownlibs/pca9555.h"
#include "ownlibs/common.h"

namespace control {
    // an die Ventile werden absolutwerte uerbtragen. Diese basieren auf der Zeit, die gespeichert
    // wird, wenn diese Klasse erstellt wird
    class Main_ValveCtrl : public Thread {
    public:
        //Defaultconstructor
        Main_ValveCtrl();
        //Destructor
        ~Main_ValveCtrl();
        //Funktion, die von LabCom aufgerufen wird. Sie erstellt Valve-Objekte
        void createValve(uint16_t amount);
        //Adressen werden von LabCom übergeben, Kommunikationsobjekte werden erstellt
        void setPcbAddresses(uint16_t amount, char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Wird von LabCom aufgerufen und bekommt ein Array mit allen Pins.
        //Adressen werden weiter an alle Valve-Objekte gegeben
        void setPins(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]);
        //Stellwerte fuer die MFCs koennen als Pseudoevents gesetzt werden. Die Ereignisse
        //werden in einer Queue gespeichert und bei gegebenen Zeitpunkt ausgefuehrt
        void setEvent(uint16_t valveID, uint16_t value, uint32_t time);
        //setzt die 'ready'-Variable der Valves auf true. Außerdem wird der Nullpunkt der Steuerung gesetzt
        void start(uint32_t startTime);
        //Gebe Adresse des Displayobjektes an die einzelnen Ventile, um zu kommunizieren
        void setMainDisplayObjectPointer(io::Main_Display *main_display);
        //Diese Funktion wird vom StringBuilder aufgerufen und sammelt die Daten der Ventile
        //um sie in den Ausgabestring zu schreiben
        //Die Funktion fragt die einzelnen Ventile nach ihren Werten ab
        void getValveValueList(uint8_t valveValueList[]);
        uint16_t getAmount_valve();
        //wird von Main_LabCom kontinuierlich abgefragt, ob die queue abgeschlossen ist.
        bool getQueueFinished();
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        int16_t amount_valve;
        bool valve_continue_next_loop[MAX_AMOUNT_VALVE];
        int16_t amount_of_finished_valves;
        bool queueFinished;

        control::ValveCtrl *valve_list[MAX_AMOUNT_VALVE]; //Hier werden die Adressen der Valve-Objekte gespeichert

        Pca9555 *comPointer[MAX_AMOUNT_VALVE_PCB]; //hier werden die Objekte fuer die Kommunikation mit den Ventilplatinen gespeichert.
        io::Main_Display *main_display;
    };
}

#endif
