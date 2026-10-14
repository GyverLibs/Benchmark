#include <Arduino.h>
#include <Benchmark.h>

void setup() {
    Serial.begin(115200);

    benchBegin();
    delay(1);
    float us = benchEnd();
    Serial.println(us);

    benchReset();
    for (uint8_t i = 0; i < 5; i++) {
        benchBegin();
        delay(1);
        benchEndStat(Serial, 10);  // current, avr, min, max
    }
}

void loop() {
    benchLoop(Serial, 5000, 10);
}
