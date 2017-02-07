#include "main_display.h"

namespace io {
    Main_Display::Main_Display() {
        this->afterErrorTime  = 0;
        this->lastPrint       = 0;
        this->startTime       = 0;

        this->amountMFC       = 0;
        this->amountValve     = 0;

        this->ready           = false;

        this->lastEvent_type  = 'x';
        this->lastEvent_id    = 5;
        this->lastEvent_value = 0;
        this->lastEvent_time  = 0;

        this->amount_queueFinished = 0;

        this->display         = new LiquidCrystal_I2C(0x38, 10, 9, 8, 4, 20);

        this->display->init();
        this->display->clear();
        this->display->backlight_setColor(255,255,255);

        //Zeige an, dass Objekt erzeugt wurde und Board bereit ist
        this->boardIsReady();
    }
    Main_Display::~Main_Display() {

    }

    void Main_Display::throwError(int errorNumber) {
        if (errorNumber >= 1000 && errorNumber < 2000) {
            int arrayIndex = errorNumber % 1000; //hole Index der Arrays, indem 1000er "entfernt" wird
            this->display->backlight_setColor(255,180,0);

            this->display->updateDisplayMatrix(
                error_1000[arrayIndex][0],
                error_1000[arrayIndex][1],
                error_1000[arrayIndex][2],
                error_1000[arrayIndex][3]
            );

            this->afterErrorTime = millis() + ERR_1000_TIME;
        } else

        if (errorNumber >= 5000 && errorNumber < 6000) {
            int arrayIndex = errorNumber % 5000; //hole Index der Arrays, indem 1000er "entfernt" wird
            this->display->backlight_setColor(255,0,0);

            this->display->updateDisplayMatrix(
                error_5000[arrayIndex][0],
                error_5000[arrayIndex][1],
                error_5000[arrayIndex][2],
                error_5000[arrayIndex][3]
            );

            this->afterErrorTime = millis() + ERR_5000_TIME;
        }
    }

    void Main_Display::boardIsReady() {
        this->display->updateDisplayMatrix(
            "    BOARD BEREIT    ",
            "                    ",
            "      Warte auf     ",
            "    Uebertragung    "
        );
    }

    void Main_Display::header_started(int amountMFC, int amountValve) {
        this->amountMFC   = amountMFC;
        this->amountValve = amountValve;

        this->display->updateDisplayMatrix(
            "   DATEN GESTARTET  ",
            "                    ",
            "     Uebertrage     ",
            "       Header       "
        );
    }

    void Main_Display::event_started() {
        this->display->updateDisplayMatrix(
            "   HEADER KOMPLETT  ",
            "                    ",
            "     Uebertrage     ",
            "     Eventliste     "
        );
    }

    void Main_Display::event_finished() {
        this->display->updateDisplayMatrix(
            " EVENTLISTE KOMPLETT",
            "                    ",
            "   Warte auf Start  ",
            "     der Messung    "
        );
    }

    void Main_Display::start(unsigned long startTime) {
        this->startTime = startTime;
        this->ready     = true;

        this->display->updateDisplayMatrix(
            "                    ",
            " MESSUNG  GESTARTET ",
            "                    ",
            "                    "
        );

        //setze diese Meldung als Error, um Displayuasgabe fuer Zeit zu sperren
        this->afterErrorTime = millis() + 500;
    }

    void Main_Display::setLastEvent (char type, int id, int value, unsigned int time) {
        this->lastEvent_type  = type;
        this->lastEvent_id    = id;
        this->lastEvent_value = value;
        this->lastEvent_time  = time;
    }

    void Main_Display::setLastEvent_id (int id) {
        this->lastEvent_id  = id;
    }

    void Main_Display::bothQueuesFinished () {
        //MFC und Ventil Eventlisten sind vollstaendig abgearbeitet
        srl->println('D', "Alle Eventlisten abgearbeitet. Programm endet hier.");

        this->display->backlight_setColor(0,255,0);

        char endTime_string[12];
        cmn::getTimeString(millis() - this->startTime, endTime_string);

        //Baue Anzeigetext
        char displayText [DISPLAY_SIZE_HEIGHT][DISPLAY_SIZE_WIDTH +1]; //TODO: Eins groesser, da '\0'
        sprintf(displayText[0], "         #M:%02d #V:%02d\0", this->amountMFC, this->amountValve);
        sprintf(displayText[1], "                    \0");
        sprintf(displayText[2], "   ABGESCHLOSSEN    \0");
        sprintf(displayText[3], "    %s     \0", endTime_string);

        this->display->updateDisplayMatrix(
            displayText[0],
            displayText[1],
            displayText[2],
            displayText[3]
        );

        this->afterErrorTime = 4294967295; //maximale Zeit in u_int32
    }





    bool Main_Display::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (millis() >= this->afterErrorTime) { //Errors haben Vorrang und blockieren Ausgabe
            this->display->backlight_setColor(255,255,255);

            if (this->ready && millis() >= this->lastPrint + DISPLAY_REDRAW_INTERVALL) {
                //Erstelle String der letzten Event Zeit
                char lastEventTime_string[12];
                cmn::getTimeString(this->lastEvent_time, lastEventTime_string);

                //Erstelle String der aktuellen Laufzeit, relativ zum Messstart
                char currentTime_string[12];
                cmn::getTimeString(millis() - this->startTime, currentTime_string);

                //Baue Anzeigetext
                char displayText [DISPLAY_SIZE_HEIGHT][DISPLAY_SIZE_WIDTH +1]; //TODO: Eins groesser, da '\0'
                sprintf(displayText[0], "         #M:%02d #V:%02d\0", this->amountMFC, this->amountValve);
                sprintf(displayText[1], "                    \0");
                sprintf(displayText[2], "LAUFZEIT:%s\0", currentTime_string);
                sprintf(displayText[3], "%c%02d-%04d-%s\0", this->lastEvent_type, this->lastEvent_id, this->lastEvent_value, lastEventTime_string);

                this->display->updateDisplayMatrix(
                    displayText[0],
                    displayText[1],
                    displayText[2],
                    displayText[3]
                );

                this->lastPrint = millis();
            }
        }

        return true;
    }
}
