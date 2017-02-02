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

    void getTimeString(unsigned long time, char timeString_out[]) {
        int days    = time / 86400000;
        time        = time % 86400000;

        int hours   = time / 3600000;
        time        = time % 3600000;

        int minutes = time / 60000;
        time        = time % 60000;

        int seconds = time / 1000;

        char timeString_temp[12];
        sprintf(timeString_temp, "%02d:%02d:%02d:%02d\0", days, hours, minutes, seconds);
        strcpy(timeString_out, timeString_temp);
    }

    //TODO setze \0 an Ende des outputstrings
    char* integerToByte(unsigned long value, int bytesize, char output[]) {
      int power; //Potenz??
      int xi;

      //value*=MAX_AMOUNT_DEC_PLACES; //aktuell eh nicht brauchbar, da int kein Komma unterstuetzt

      for (int i=0; i < bytesize; i++){
        power = 1;
        for (int k=0; k < bytesize - i -1; k++){ //enspricht power = power^(MAX_ROW_BYTES-i-1) -> für i=0 ist power=255^(3-0-1)=255^2, i=1 ist power=255^(3-1-1)=255, i=0 ist power=255^(3-2-1)=255^0=1
          power *= 255;
        }
        xi        = value % power; //x_i = value mod (255^î)
        value     = value - xi*power;
        output[i] = xi; //Schreibt ASCII-Zeichen mit dem ASCII-Wert xi als Char-Zeichen in output
      }
      return output;
    }
};
