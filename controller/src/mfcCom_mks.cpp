#include "mfcCom_mks.h"

namespace communication {
    MfcCom_Mks::MfcCom_Mks() {

    }

    MfcCom_Mks::~MfcCom_Mks() {

    }

    uint8_t MfcCom_Mks::writeValue(char address[], float value) {
        digitalWrite(PIN_ENABLE_MKS, HIGH);
        delayMicroseconds(MKS_WRITEENABLE_WAIT_TIME);

        strcpy(this->_address, address);

        uint8_t first_digit = (uint8_t) value;
        uint32_t last_digits = 0;
        if (first_digit < 10) {
            last_digits = (value - first_digit) * 100000;
            sprintf(this->_value, "%d.%05lu", first_digit, last_digits);
        } else {
            last_digits = (value - first_digit) * 10000;
            sprintf(this->_value, "%d.%04lu", first_digit, last_digits);
        }

        srl->print('M', "@");
        srl->print('M', this->_address);
        srl->print('M', "S"); //Setze Fluss
        srl->print('M', this->_value);
        srl->print('M', '\r'); //Termimierung

        srl->flush('M'); //warte bis Uebertragung abgeschlossen

        delayMicroseconds(MKS_READENABLE_WAIT_TIME);
        digitalWrite(PIN_ENABLE_MKS, LOW);

        delayMicroseconds(MKS_READ_START_TIME);
        uint8_t charId = 0;
        uint32_t starttime = millis();
        while (millis() - starttime < MKS_READ_TIMEOUT) {
             //warte, wenn gerade kein Zeichen in Buffer, aber String nicht abgeschlossen
            if (srl->available('M') <= 0)
                continue;
            char newChar = srl->read('M');
            if (newChar == '\r') { //string abgeschlossen
                this->_response[charId] = '\0';
                break;
            }
            this->_response[charId] = newChar;
            charId++;
        }

        //TODO: Antwort verarbeiten
        srl->println('L', this->_response); //nur zum Debuggen

        return 0;
    }

    uint32_t MfcCom_Mks::readValue(char address[]) {
        digitalWrite(PIN_ENABLE_MKS, LOW);

        strcpy(this->_address, address);

        srl->print('M', "@");
        srl->print('M', this->_address);
        srl->print('M', "F"); //Lese Fluss
        srl->print('M', '\r'); //Termimierung

        //TODO verarbeite Antwort
        return 0;
    }
}
