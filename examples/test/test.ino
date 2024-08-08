#include <Arduino.h>
#include <Benchmark.h>

void setup() {
    Serial.begin(115200);

    benchBegin();
    delay(1);
    benchEnd(&Serial);
}

void loop() {

}