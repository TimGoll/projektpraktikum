#include "main_labCom.h"

namespace communication {
    Main_LabCom::Main_LabCom() {
        this->reading = true;
        this->sending = false;

        this->headerLineCounter = 0;

        Serial.println("LabCom erstellt.");
    }
    Main_LabCom::~Main_LabCom() {

    }

    void Main_LabCom::setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl) {
        this->main_mfcCtrl = main_mfcCtrl;
    }

    int Main_LabCom::readLine() {
        this->bufferCharIndex = 0; //setze index auf Startposition zurueck
        unsigned long startTime = millis();

        //Lese so lange, bis Zeile vollstaendig empfangen wurde, oder Timeout erreicht wurde (Main-Thread wird blockiert)
        while(millis() < startTime + SERIAL_READ_TIMEOUT) {
            if (Serial.available() > 0) { //Lese Daten, wenn vorhanden
                this->inDataBuffer[this->bufferCharIndex] = Serial.read(); //Serial.read() gibt einen einzelnen Char zurueck

                //UEBERPRUEFUNG
                if (this->bufferCharIndex == 0) {
                    if (this->inDataBuffer[this->bufferCharIndex] != '<') { //erstes Zeichen muss oeffnender Tag sein
                        Serial.println("ERROR - Falscher Zeilenbeginn");
                        return ERR_SERIAL_READ_WRONG_LINE_BEGIN;
                    }
                }
                if (this->inDataBuffer[this->bufferCharIndex] == '\n') { //Zeile zuende
                    if (this->inDataBuffer[this->bufferCharIndex -1] != '>') { //vorheriges Zeichen muss schliessender Tag sein
                        Serial.println("ERROR - Falsches Zeilenende");
                        return ERR_SERIAL_READ_WRONG_LINE_ENDING;
                    } else { //vollstaendiger String abgeschlossen
                        this->inDataBuffer[this->bufferCharIndex +1] = '\0';
                        Serial.println("");
                        Serial.print("Eingabestring akzeptiert: ");
                        Serial.print(this->inDataBuffer);
                        return 1;
                    }
                }

                this->bufferCharIndex++;
                if (this->bufferCharIndex == SERIAL_READ_MAX_LINE_SIZE) {
                    return ERR_SERIAL_READ_MAX_STRING_SIZE;
                    Serial.println("ERROR - Eingabestring zu lang");
                }
            }
        }
        Serial.print("ERROR - Timeout: ");
        Serial.println(this->inDataBuffer);
        return ERR_SERIAL_READ_TIMEOUT; //Timeout
    }

    int Main_LabCom::splitLine() {
        int currentCharIndex = 0;
        int endCharIndex = this->bufferCharIndex -1; //da letztes Zeichen '>' ist

        //Schleife, die durch die Ziel-Array-Indexe navigiert
        for (int arrayIndex = 0; arrayIndex < SERIAL_READ_MAX_BLOCK_AMOUNT; arrayIndex++) {
            //Schleife, die durch die Char-Positionen eines Ziel-Array-Eintrags navigiert
            for (int arrayCharIndex = 0; arrayCharIndex < SERIAL_READ_MAX_BLOCK_SIZE; arrayCharIndex++) {
                currentCharIndex++; //Wird anfangs um 1 erhoet, Zeichen 1 ist zu ignorieren ('<')

                if (this->inDataBuffer[currentCharIndex] == ',') { //Eintrag zu ende
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    break;
                }

                if (currentCharIndex == endCharIndex) { //Zeilenende erreicht
                    this->inDataArray[arrayIndex][arrayCharIndex] = '\0';
                    return arrayIndex +1; //Index und Groesse sind immer um 1 verschieden
                }

                this->inDataArray[arrayIndex][arrayCharIndex] = this->inDataBuffer[currentCharIndex];
            }
        }
    }

    void Main_LabCom::start(unsigned long startTime) {
        //starte MFCs
        main_mfcCtrl->start(startTime);

        //starte Ventile
    }

    //////////////////// MAINLOOP ////////////////////

    bool Main_LabCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        // TODO: Eventuell immer Zeilenweise lesen mit while(Serial.available()), ist zu testen.
        // Verlagerung des Codes in seperate Lese/Schreibfunktion sinnvoll
        if (this->reading) { //Empfange Messprogramm
            if (Serial.available() > 0) {
                int errCode = this->readLine();
                if (errCode == 1) { //Funktion wird ausgefuerhrt und bei Erfolg in If gegangen
                    //Der Header muss einzeln verarbeitet werden, daher gibt es einen headerLineCounter,
                    //der die erwartete Zeile speichert
                    int arraySize = this->splitLine();

                    if (this->headerLineCounter == 0) { //ZEILE 1: MFC+Ventilanzahl
                        this->amount_MFC   = atoi(this->inDataArray[0]);
                        this->amount_valve = atoi(this->inDataArray[1]);

                        //erstelle MFC-Objekte in der mfc_main
                        main_mfcCtrl->createMFC(this->amount_MFC);

                        this->headerLineCounter = 1;
                    }

                    else if (this->headerLineCounter == 1) { //ZEILE 2: MFC-Adressen
                        main_mfcCtrl->setAdresses(this->inDataArray);

                        this->headerLineCounter = 2;
                    }

                    else if (this->headerLineCounter == 2) { //ZEILE 3: MFC-Typen
                        main_mfcCtrl->setTypes(this->inDataArray);

                        this->headerLineCounter = 3;
                    }

                    else if (this->headerLineCounter == 3) { //ZEILE 3: Ventil-Pins
                        //TODO: Setze Ventilpins

                        Serial.println("PLATZHALTER Ventilpins gesetzt.");

                        this->headerLineCounter = 4;
                    }

                    else if (this->headerLineCounter == 4) { //ZEILE 4: Letzte Zeile, hier wird ein 'begin' erwartet
                        if (strcmp(this->inDataArray[0], "begin") == 0) {
                            Serial.println("Header vollstaendig.");

                            this->headerLineCounter = 5;
                        }
                    }

                    else if (this->headerLineCounter == 5) { //ZEILE 5: Eventliste
                        if (strcmp(this->inDataArray[0], "M") == 0) { //MFC
                            main_mfcCtrl->setEvent(
                                atoi(this->inDataArray[1]), //MFC-ID
                                atoi(this->inDataArray[2]), //value //TODO: verschiedene Werte je nach typ
                                strtoul(this->inDataArray[3], NULL, 0) //time
                            );
                        } else if (strcmp(this->inDataArray[0], "V") == 0) { //Ventil
                            //TODO: Setzte Ventilevents
                            Serial.println("PLATZHALTER Ventilevent gesetzt.");
                        }

                        else if (strcmp(this->inDataArray[0], "end") == 0) { //Am ende wechselt labCom in den Sende-Modus
                            Serial.println("Uebertragung abgeschlossen.");

                            this->reading = false;
                            this->sending = true;

                            this->start(millis());
                        }
                    }
                } else {
                    //TODO: ErrorCode muss verarbeitet werden (Displayanzeige?)
                }
            }
        }

        if (this->sending) { //Sende Messwerte parallel zur Messung

            //Testaufruf der Funktionen (loop-aehnlich)
            //for (int i = 0; i < this->amount_MFC; i++) {
            //    this->mfc_list[i]->compute();
            //}
        }

        return true;
    }
}
