void setup() {
  Serial.begin(115200);
  Serial.println(charArrayToFloat("12.123456"),16);
}

void loop() {
  // put your main code here, to run repeatedly:

}

float charArrayToFloat(char input[]) {
        uint8_t max_length = 10;

        char first[4];
        uint8_t first_counter = 0;
        char second[8];
        uint8_t second_counter = 0;

        for (first_counter = 0; first_counter < max_length; first_counter++) {
            if (input[first_counter] == '\0')
                return 0;

            if (input[first_counter] == '.')
                break;

            first[first_counter] = input[first_counter];
        }
        first[first_counter] = '\0'; //schliesse String ab, for Schleife erhoeht Counter automatisch um ein weiteres

        for (second_counter; second_counter < max_length; second_counter++) {
            if (input[second_counter+1] == '\0')
                break;

            second[second_counter] = input[first_counter + second_counter+1];
        }
        second[second_counter] = '\0';

        //-2, da Schleife eins weiter zaehlt und \0 nicht gezaehlt werden soll
        float result = (float) ( atoi(first) + ( (float) atoi(second) ) / pow(10, second_counter-2) );

        return result;
    }
