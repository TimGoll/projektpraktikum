#include "common.h"

namespace cmn {
    void trim (char string[]) {
        char buffer[256];

        int i = 0, j = 0;
        while (string[i] == ' ') { //iteriere duch array bis zum ersten Zeichen
            i++;
        }
        while (string[i] != '\0') { //kopiere reststring
            buffer[j] = string[i];
            i++;j++;
        }
        j--; //verkleinere j um 1, da letztes Zeichen '\0' ist
        while (buffer[j] == ' ') { //iteriere duch array rueckwaerts bis zum ersten Zeichen
            j--;
        }
        buffer[j+1] = '\0';

        strcpy(string, buffer); //schiebe neuen String in alten
    }
};
