#include "main_display.h"

namespace io {
    Main_Display::Main_Display() {
        this->afterErrorTime  = 0;
        this->lastPrint       = 0;
        this->startTime       = 0;

        this->amount_MFC       = 0;
        this->amount_valve     = 0;

        this->ready           = false;

        this->lastEvent_type  = 'x';
        this->lastEvent_id    = 5;
        this->lastEvent_value = 0;
        this->lastEvent_time  = 0;

        this->started_transmission = false;

        this->display         = new LiquidCrystal_I2C(0x38, 35, 36, 37, 4, 20);

        this->display->init();
        this->display->clear();
        this->display->backlight_setColor(255,255,255);
        uint8_t customChar[8] = {
            0b00000,
            0b00000,
            0b01000,
            0b01100,
            0b01110,
            0b01100,
            0b01000,
            0b00000
        };
        this->display->createChar(0, customChar);

        this->_cursor_position = 0;
        this->_selected_item = 0;
        this->_menu_open = false;

        //Zeige an, dass Objekt erzeugt wurde und Board bereit ist
        this->boardIsReady();
    }
    Main_Display::~Main_Display() {

    }

    void Main_Display::setReadFileFunction(void (*readFile) (char[])) {
        this->readFile = readFile;
    }

    void Main_Display::throwError(uint16_t errorNumber) {
        if (errorNumber >= 1000 && errorNumber < 2000) {
            uint16_t arrayIndex = errorNumber % 1000; //hole Index der Arrays, indem 1000er "entfernt" wird
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
            uint16_t arrayIndex = errorNumber % 5000; //hole Index der Arrays, indem 1000er "entfernt" wird
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
            "  Uebertrage Daten  ",
            " oder oeffne Menue  "
        );
    }

    void Main_Display::header_started(uint16_t amount_MFC, uint16_t amount_valve) {
        this->amount_MFC   = amount_MFC;
        this->amount_valve = amount_valve;

        this->started_transmission = true;

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

    void Main_Display::start(uint32_t startTime) {
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

    void Main_Display::setLastEvent (char type, uint16_t id, uint16_t value, uint32_t time) {
        this->lastEvent_type  = type;
        this->lastEvent_id    = id;
        this->lastEvent_value = value;
        this->lastEvent_time  = time;
    }

    void Main_Display::setLastEvent_id (uint16_t id) {
        this->lastEvent_id  = id;
    }

    void Main_Display::bothQueuesFinished () {
        //MFC und Ventil Eventlisten sind vollstaendig abgearbeitet
        srl->println('D', "Alle Eventlisten abgearbeitet. Programm endet hier.");
        srl->println('D', "Fuer weitere Messung Board bitte resetten.");

        digitalWrite(PIN_FINISHLED, HIGH);

        this->display->backlight_setColor(0,255,0);

        char endTime_string[12];
        cmn::getTimeString(millis() - this->startTime, endTime_string);

        //Baue Anzeigetext
        char displayText[DISPLAY_SIZE_HEIGHT][DISPLAY_SIZE_WIDTH +1]; //Eins groesser, da '\0'
        sprintf(displayText[0], "         #M:%02d #V:%02d", this->amount_MFC, this->amount_valve);
        sprintf(displayText[1], "                    ");
        sprintf(displayText[2], " ABGESCHLOSSEN NACH ");
        sprintf(displayText[3], "    %s     ", endTime_string);

        this->display->updateDisplayMatrix(
            displayText[0],
            displayText[1],
            displayText[2],
            displayText[3]
        );

        this->afterErrorTime = 4294967295; //maximale Zeit in uint32_t
    }


    void Main_Display::menu_setMenuItems(char items[][16], uint8_t amount) {
        this->_amount_of_items = amount +1; //+1, da noch 'ZURUECK' dazu kommt
        strcpy(this->_items[0], ".. ZURUECK");
        for (uint8_t i = 0; i < amount; i++) {
            strcpy(this->_items[i+1], items[i]);
        }
    }

    void Main_Display::menu_controlMenu() {
        if (!this->started_transmission) {
            if (!this->_menu_open) { //Menue nicht geoeffnet, stelle Anzeige dar
                this->_menu_open = true;
                this->menu_drawMenu();
                this->display->setSymbol(0, 2,0);
            } else { //menu geoeffnet, bestaetige Auswahl
                this->_menu_open = false;
                if (this->_selected_item == 0) {
                    this->boardIsReady();
                } else {
                    this->readFile(this->_items[this->_selected_item]); //lese Datei ein
                    this->event_finished();
                }
            }
        }
    }

    void Main_Display::menu_navigateMenu(uint8_t direction) {
        if (this->_menu_open) {
            if (direction == 0) { //nach oben
                this->_cursor_position--;
                this->_cursor_position = max(this->_cursor_position, 0);
                this->_selected_item--;
                this->_selected_item = max(this->_selected_item, 0);
            } else if (direction == 1) { //nach unten
                this->_cursor_position++;
                this->_cursor_position = min(this->_cursor_position, (this->_amount_of_items -1 < 3) ? this->_amount_of_items -1 : 3);
                this->_selected_item++;
                this->_selected_item = min(this->_selected_item, this->_amount_of_items -1);
            }

            this->menu_drawMenu();
            this->display->setSymbol(0, 2,this->_cursor_position);
        }
    }

    void Main_Display::menu_drawMenu() {
        uint8_t first_line_id = this->_selected_item - this->_cursor_position;

        char displayText[DISPLAY_SIZE_HEIGHT][DISPLAY_SIZE_WIDTH +1]; //eins groesser da '\0'
        strcpy(displayText[0], "M| ");
        strcat(displayText[0], this->_items[first_line_id]);
        strcpy(displayText[1], "E| ");
        if (this->_amount_of_items > 1) strcat(displayText[1], this->_items[first_line_id + 1]);
        strcpy(displayText[2], "N| ");
        if (this->_amount_of_items > 2) strcat(displayText[2], this->_items[first_line_id + 2]);
        strcpy(displayText[3], "U| ");
        if (this->_amount_of_items > 3) strcat(displayText[3], this->_items[first_line_id + 3]);

        this->display->updateDisplayMatrix(
            displayText[0],
            displayText[1],
            displayText[2],
            displayText[3]
        );
    }




    bool Main_Display::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (millis() >= this->afterErrorTime) { //Errors haben Vorrang und blockieren Ausgabe
            this->display->backlight_setColor(255,255,255);
                                                                                         //da die Startzeit 1000ms in die Zukunft gesetzt wird
            if (this->ready && millis() >= this->lastPrint + DISPLAY_REDRAW_INTERVALL && millis() >= this->startTime) {
                //Erstelle String der letzten Event Zeit
                char lastEventTime_string[12];
                cmn::getTimeString(this->lastEvent_time, lastEventTime_string);

                //Erstelle String der aktuellen Laufzeit, relativ zum Messstart
                char currentTime_string[12];
                cmn::getTimeString(millis() - this->startTime, currentTime_string);

                //Baue Anzeigetext
                char displayText [DISPLAY_SIZE_HEIGHT][DISPLAY_SIZE_WIDTH +1]; //Eins groesser, da '\0'
                sprintf(displayText[0], "         #M:%02d #V:%02d", this->amount_MFC, this->amount_valve);
                sprintf(displayText[1], "                    ");
                sprintf(displayText[2], "LAUFZEIT:%s", currentTime_string);
                sprintf(displayText[3], "%c%02d-%04d-%s", this->lastEvent_type, this->lastEvent_id, this->lastEvent_value, lastEventTime_string);

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
