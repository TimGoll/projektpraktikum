#include "mfcCom.h"

namespace communication {
    MfcCom::MfcCom() {
        this->_mks      = new MfcCom_Mks();
        this->_buerkert = new MfcCom_Buerkert();
    }

    MfcCom::~MfcCom() {

    }

    uint8_t MfcCom::writeValue(char type[], char address[], float value) {
        if (strcmp(type, "MKS") == 0)
            return this->_mks->writeValue(address, value);
        else if (strcmp(type, "Buerkert") == 0)
            return this->_buerkert->writeValue(address, value);
        else
            return 100; //TODO errorcode
    }

    uint32_t MfcCom::readValue(char type[], char address[]) {
        if (strcmp(type, "MKS") == 0)
            return this->_mks->readValue(address);
        else if (strcmp(type, "Buerkert") == 0)
            return this->_buerkert->readValue(address);
        else
            return 100; //TODO errorcode
    }

    // Initialisiere die Kommunikation mit den MFCs
    MfcCom *mfcCom = new MfcCom();
}
