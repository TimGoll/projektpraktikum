#ifndef MAIN_DISPLAY_H
#define MAIN_DISPLAY_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>
#include <Wire.h>

#include "ownlibs/lcd_I2C.h"
#include "ownlibs/serialCommunication.h"
#include "ownlibs/common.h"
#include "config.h"
#include "errors.h"

namespace io {
    class Main_Display : public Thread {
    public:
        //Defaultconstructor
        Main_Display();
        //Destructor
        ~Main_Display();
        //verarbeite Error Nummer und gebe Displayausgabe
        void throwError(uint16_t errorNumber);
        //zeige auf dem Display an, dass Uebertragung gestartet werden kann
        void boardIsReady();
        //Zeige an, dass Header-Uebertragung gestartet wurde
        void header_started(uint16_t amount_MFC, uint16_t amount_valve);
        //Zeige an, dass Header vollstanedig + Event-Uebertragung gestartet
        void event_started();
        //Zeige an, dass Event-Uebertragung dertig und Mess-Start erwartet wird
        void event_finished();
        //Messung gestartet, beginne Live-Ausgabe
        void start(uint32_t startTime);
        //setze letzt ausgefuehrtes Event zur Displayausgabe
        void setLastEvent(char type, uint16_t id, uint16_t value, uint32_t time);
        //LabCom meldet, dass beide Queues abgearbeitet sind, Meldung wird auf dem Display
        //angezeigt
        void bothQueuesFinished ();

        //setze Funktionspointer
        void setReadFileFunction(void (*readFile) (char[]));

        void menu_setMenuItems(char items[][16], uint8_t amount);
        void menu_navigateMenu(uint8_t direction);
        void menu_controlMenu();


        void setLastEvent_id(uint16_t id);
    protected:
        //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
        bool loop();
    private:
        void menu_drawMenu();

        uint32_t afterErrorTime;
        uint32_t lastPrint;
        uint32_t startTime;
        uint16_t amount_MFC;
        uint16_t amount_valve;
        bool ready;
        bool started_transmission;

        //lastEvent-Variablen
        char lastEvent_type;
        uint16_t lastEvent_id;
        uint16_t lastEvent_value;
        uint32_t lastEvent_time;

        //menu Variablen
        char _items[MENU_MAX_AMOUNT_ENTRIES][16];
        int8_t _cursor_position;
        int8_t _selected_item;
        uint8_t _amount_of_items;
        bool _menu_open;

        LiquidCrystal_I2C *display; //LCD-Write-Class
        void (*readFile) (char[]); //speichere Funktionspointer
    };
}

#endif
