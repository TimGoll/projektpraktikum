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

        srl->print('U', "@");
        srl->print('U', this->_address);
        srl->print('U', "S"); //Setze Fluss
        srl->print('U', this->_value);
        srl->print('U', '\r'); //Termimierung

        srl->println('U', ""); //DEBUG: zeilenumbruch
    }

    uint32_t MfcCom_Mks::readValue(char address[]) {
        sprintf(this->_address, "%02d", address);

        srl->print('U', "@");
        srl->print('U', this->_address);
        srl->print('U', "F"); //Setze Fluss
        srl->print('U', '\r'); //Termimierung

        //TODO verarbeite Antwort
    }
}
