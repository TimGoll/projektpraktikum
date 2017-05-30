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
        sprintf(timeString_temp, "%02d:%02d:%02d:%02d\0", days, hours, minutes, seconds);
        strcpy(timeString_out, timeString_temp);
    }

    void split(char destination[][16], char source[], char symbol) {
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
    }


    void integerToByte(unsigned long value, int bytesize, char output[]) {
      int basisToPower_i; //nimmt den Wert 255^i mit i:[0,1,2,..,bytesize-1] an; wird zur Bestimmung der Koeffizienten xi benötigt (siehe unten)
      int xi;

      //value*=MAX_FLOAT_POINT_SHIFT; //aktuell eh nicht brauchbar, da int kein Komma unterstuetzt

      //for-Schleife bestimmt Koeffizienten xi im 256-System: value|_10erSystem = (x0*1 + x1*255 + x2*(255^2) + x3*(255^3) + ...)|_256er-System   mit i:[0,1,2,..,bytesize-1]
      //und schreibt das ASCII-Zeichen mit dem ASCII-Wert xi als Char-Zeichen in output[i]
      for (int i = bytesize-1; i >= 0; i--){
        basisToPower_i = 1;
        //for-Schleife realisiert basis=255^i
        for (int k=0; k < i; k++){
          basisToPower_i *= 255;
        }

      xi        = value / basisToPower_i; //xi = value mod (255^i)
      value     = value - xi*basisToPower_i;
      output[i] = xi; //Schreibt ASCII-Zeichen mit dem ASCII-Wert xi als Char-Zeichen in output[i]
      }
      //output[bytesize-(i+1)]="\0"; //TODO
      output[bytesize-(1)]='\0';
      //return output; //du brauchst hier kein return, da du die Adresse vom output der Funktion uebergibst.
    }
};
