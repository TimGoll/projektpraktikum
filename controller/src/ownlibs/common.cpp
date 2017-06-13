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

    void getTimeString(uint32_t time, char timeString_out[]) {
        uint16_t days    = time / 86400000;
        time        = time % 86400000;

        uint16_t hours   = time / 3600000;
        time        = time % 3600000;

        uint16_t minutes = time / 60000;
        time        = time % 60000;

        uint16_t seconds = time / 1000;

        char timeString_temp[12];
        sprintf(timeString_temp, "%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
        strcpy(timeString_out, timeString_temp);
    }

    uint8_t split(char destination[][16], char source[], char symbol) {
        uint8_t destination_id = 0;
        uint8_t destination_iterator = 0;
        for (uint8_t i = 0; source[i] != '\0'; i++) {
            if (source[i] == symbol) {
                destination[destination_id][destination_iterator] = '\0';
                destination_id++;
                destination_iterator = 0;
            } else {
                destination[destination_id][destination_iterator] = source[i];
                destination_iterator++;
            }
        }
        destination[destination_id][destination_iterator] = '\0';

        return destination_id+1;
    }

    void integerToByte(uint32_t value, uint8_t bytesize, char output[]) {
        union {
            uint8_t _convert32_to_8_bit[4];
            uint32_t convert32_to_8_bit;
        };
        convert32_to_8_bit = value;

        for (uint8_t i = 0; i < bytesize; i++) {
            output[i] = _convert32_to_8_bit[i];
        }
        output[bytesize] = '\0';
    }
};
