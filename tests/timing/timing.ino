IntervalTimer timer_1;
IntervalTimer timer_2;

bool setted = false;
bool setted2 = false;

void timer_1_test() {
  Serial.print("--> one: ");
  Serial.println(micros());
  timer_1.end();
}

void timer_2_test() {
  Serial.print("--> two: ");
  Serial.println(micros());
  timer_2.end();
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (millis() >= 5000 && !setted) {
    setted = true;
    Serial.print("setting callback1, time: ");
    Serial.println(micros());
    timer_1.begin(timer_1_test, 1000007);
  }

  if (millis() >= 5100 && !setted2) {
    setted2 = true;
    Serial.print("setting callback2, time: ");
    Serial.println(micros());
    timer_2.begin(timer_2_test, 500000);
  }
}
