#include "mfcCom_mks.h"

namespace communication {
    MfcCom_Mks::MfcCom_Mks() {

    }

    MfcCom_Mks::~MfcCom_Mks() {

    }

    uint8_t MfcCom_Mks::writeValue(char address[], float value) {
        //sprintf(this->_address, "%02d", address);
        strcpy(this->_address, address);
        //TODO this->_value auf Form: x.xxxxx (1.5)

        srl->print('M', "@");
        srl->print('M', this->_address);
        srl->print('M', "S"); //Setze Fluss
        srl->print('M', this->_value);
        srl->print('M', '\r'); //Termimierung

        srl->println('M', ""); //DEBUG: zeilenumbruch

        return 0;
    }

    uint32_t MfcCom_Mks::readValue(char address[]) {
        //sprintf(this->_address, "%02d", address);
        strcpy(this->_address, address);

        srl->print('M', "@");
        srl->print('M', this->_address);
        srl->print('M', "F"); //Setze Fluss
        srl->print('M', '\r'); //Termimierung

        //TODO verarbeite Antwort
        return 0;
    }
}
