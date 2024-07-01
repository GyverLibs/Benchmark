#include <Arduino.h>
#include <benchmark.h>

void setup() {
    Serial.begin(115200);

    benchBegin();
    delay(1);
    benchEnd(&Serial);
}

void loop() {

}