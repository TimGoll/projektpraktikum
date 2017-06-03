#include "mfcCom_mks.h"

namespace communication {
    MfCCom_Mks::MfCCom_Mks() {

    }

    MfCCom_Mks::~MfCCom_Mks() {

    }

    uint8_t MfCCom_Mks::writeValue(uint8_t address, float value) {
        sprintf(this->_address, "%02d", address);
        //TODO this->_value auf Form: x.xxxxx (1.5)

        srl->print('U', "@");
        srl->print('U', this->_address);
        srl->print('U', "S"); //Setze Fluss
        srl->print('U', this->_value);
        srl->print('U', '\r'); //Termimierung
    }

    uint32_t MfCCom_Mks::readValue(uint8_t address) {
        sprintf(this->_address, "%02d", address);

        srl->print('U', "@");
        srl->print('U', this->_address);
        srl->print('U', "F"); //Setze Fluss
        srl->print('U', '\r'); //Termimierung

        //TODO verarbeite Antwort
    }
}
