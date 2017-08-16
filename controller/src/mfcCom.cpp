#include "mfcCom.h"

namespace communication {
    MfcCom::MfcCom() {
        Mks::init();
        Buerkert::init();
    }

    MfcCom::~MfcCom() {

    }

    bool MfcCom::writeValue(char type[], char address[], float value, float *destination) {
        if (strcmp(type, "MKS") == 0)
            return Mks::writeValue(address, value, destination);
        else if (strcmp(type, "Buerkert") == 0)
            return Buerkert::writeValue(address, value, destination);
        return false;
    }

    bool MfcCom::readValue(char type[], char address[], float *destination) {
        if (strcmp(type, "MKS") == 0)
            return Mks::readValue(address, destination);
        else if (strcmp(type, "Buerkert") == 0)
            return Buerkert::readValue(address, destination);
        return false;
    }

    // Initialisiere die Kommunikation mit den MFCs
    MfcCom *mfcCom = new MfcCom();
}
