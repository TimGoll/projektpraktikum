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
        while (j >= 0 && buffer[j] == ' ') { //iteriere duch array rueckwaerts bis zum ersten Zeichen
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

    void sort(char sorting[][16], uint8_t size) {
        if (size < 2) return;

        char tmp[16];

        //Bubblesort
        for (uint8_t n = size -1; n > 1; n--) {
            for (uint8_t i = 0; i < n; i++) {
                //vergleiche Strings Zeichen fuer Zeichen
                uint8_t char_pos = 0;
                while (true) {
                    if (sorting[i][char_pos] > sorting[i+1][char_pos]) { //tauschen
                        strcpy(tmp, sorting[i]);
                        strcpy(sorting[i], sorting[i+1]);
                        strcpy(sorting[i+1], tmp);
                    }
                    if (sorting[i][char_pos] < sorting[i+1][char_pos]) //bereits richtig positioniert
                        break;

                    //wenn beide Zeichen gleich sind, dann Schleife weiter durchlaufen

                    //String-Ende Abbruchbedingungen
                    if (sorting[i][char_pos] == '\0' || sorting[i+1][char_pos] == '\0')
                        break;
                    char_pos++;
                    if (char_pos == 16)
                        break;
                }
            }
        }
    }

    float charArrayToFloat(char input[]) {
        uint8_t max_length = 10;

        char first[4];
        uint8_t first_counter = 0;
        char second[8];
        uint8_t second_counter = 0;

        for (first_counter = 0; first_counter < max_length; first_counter++) {
            if (input[first_counter] == '\0')
                return (float) atoi(input);

            if (input[first_counter] == '.')
                break;

            first[first_counter] = input[first_counter];
        }
        first[first_counter] = '\0'; //schliesse String ab, for Schleife erhoeht Counter automatisch um ein weiteres

        for (second_counter = 0; second_counter < max_length; second_counter++) {
            if (input[second_counter+1] == '\0')
                break;

            second[second_counter] = input[first_counter + second_counter+1];
        }
        second[second_counter] = '\0';

        //-2, da Schleife eins weiter zaehlt und \0 nicht gezaehlt werden soll
        float result = (float) ( atoi(first) + ( (float) atoi(second) ) / pow(10, second_counter-2) );

        return result;
    }

    //Funktioniert, wird aber derzeit nicht genutzt
    void integerToByte(uint32_t value, uint8_t bytesize, char output[]) {
        union {
            uint8_t _convert32_to_8_bit[4];
            uint32_t convert32_to_8_bit;
        };
        convert32_to_8_bit = value;

        for (uint8_t i = 0; i < bytesize; i++) {
            output[bytesize - (i+1)] = _convert32_to_8_bit[i];
        }
        output[bytesize] = '\0';
    }
};
