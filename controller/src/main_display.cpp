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

        this->display         = new LCD_I2C(0x00, 10, 9, 8, 4, 20);


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
            //this->lcd_print(error_1000[arrayIndex][0]);

            srl->println('D', "--------------------");
            srl->println('D', error_1000[arrayIndex][0]);
            srl->println('D', error_1000[arrayIndex][1]);
            srl->println('D', error_1000[arrayIndex][2]);
            srl->println('D', error_1000[arrayIndex][3]);
            srl->println('D', "--------------------");

            this->afterErrorTime = millis() + ERR_1000_TIME;
        } else

        if (errorNumber >= 5000 && errorNumber < 6000) {
            int arrayIndex = errorNumber % 5000; //hole Index der Arrays, indem 1000er "entfernt" wird
            this->display->backlight_setColor(255,0,0);
            //this->lcd_print(error_1000[arrayIndex][0]);

            srl->println('D', "--------------------");
            srl->println('D', io::error_5000[arrayIndex][0]);
            srl->println('D', io::error_5000[arrayIndex][1]);
            srl->println('D', io::error_5000[arrayIndex][2]);
            srl->println('D', io::error_5000[arrayIndex][3]);
            srl->println('D', "--------------------");

            this->afterErrorTime = millis() + ERR_5000_TIME;
        }
    }

    void Main_Display::boardIsReady() {
        srl->println('D', "--------------------");
        srl->println('D', "    BOARD BEREIT    ");
        srl->println('D', "                    ");
        srl->println('D', "      Warte auf     ");
        srl->println('D', "    Uebertragung    ");
        srl->println('D', "--------------------");
    }

    void Main_Display::header_started(int amountMFC, int amountValve) {
        this->amountMFC   = amountMFC;
        this->amountValve = amountValve;

        srl->println('D', "--------------------");
        srl->println('D', "   DATEN GESTARTET  ");
        srl->println('D', "                    ");
        srl->println('D', "     Uebertrage     ");
        srl->println('D', "       Header       ");
        srl->println('D', "--------------------");
    }

    void Main_Display::event_started() {
        srl->println('D', "--------------------");
        srl->println('D', "   HEADER KOMPLETT  ");
        srl->println('D', "                    ");
        srl->println('D', "     Uebertrage     ");
        srl->println('D', "     Eventliste     ");
        srl->println('D', "--------------------");
    }

    void Main_Display::event_finished() {
        srl->println('D', "--------------------");
        srl->println('D', " EVENTLISTE KOMPLETT");
        srl->println('D', "                    ");
        srl->println('D', "   Warte auf Start  ");
        srl->println('D', "     der Messung    ");
        srl->println('D', "--------------------");
    }

    void Main_Display::start(unsigned long startTime) {
        this->startTime = startTime;
        this->ready     = true;

        srl->println('D', "--------------------");
        srl->println('D', "                    ");
        srl->println('D', " MESSUNG  GESTARTET ");
        srl->println('D', "                    ");
        srl->println('D', "                    ");
        srl->println('D', "--------------------");

        //setze diese Meldung als Error, um Displayuasgabe fuer Zeit zu sperren
        this->afterErrorTime = millis() + ERR_1000_TIME;
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
                sprintf(displayText[0], "           M:%02d V:%02d\0", this->amountMFC, this->amountValve);
                sprintf(displayText[1], "                    \0");
                sprintf(displayText[2], "LAUFZEIT:%s", currentTime_string);
                sprintf(displayText[3], "%c%02d-%04d-%s", this->lastEvent_type, this->lastEvent_id, this->lastEvent_value, lastEventTime_string);

                srl->println('D', "--------------------");
                srl->println('D', displayText[0]);
                srl->println('D', displayText[1]);
                srl->println('D', displayText[2]);
                srl->println('D', displayText[3]);
                srl->println('D', "--------------------");

                this->lastPrint = millis();
            }
        }

        return true;
    }
}
