This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/Benchmark.svg?color=brightgreen)](https://github.com/GyverLibs/Benchmark/releases/latest/download/Benchmark.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/Benchmark.svg)](https://registry.platformio.org/libraries/gyverlibs/Benchmark)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Benchmark?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Benchmark
Measure code execution time for Arduino and measure the amount of free RAM

### Compatibility
- AVR (**Library uses Timer 1**)
- ESP8266
- ESP32

## Contents
- [Use of use](#usage)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Use of use
```cpp
// begin
void benchBegin();

// finish measurement and return microseconds
float benchEnd();

// finish measurement and print precise microseconds and CPU ticks
float benchEnd(Print& log);

// print integer-microsecond statistics; window 0 disables average output
float benchEndStat(Print& log, uint16_t window = 10);

// reset benchEndStat and benchLoop statistics
void benchReset();

// loop meter: min_us in us, output in ms, return value in us
uint32_t benchLoop(Print& log, uint32_t min_us = 0, uint16_t window = 10);

// get the amount of free memory
size_t getFreeHeap();
```

### Example
```cpp
#include <Benchmark.h>

void setup() {
	Serial.begin(115200);
    benchBegin();
    delay(1);
    benchEnd(Serial);  // precise one-shot measurement
}
void loop() {
    benchLoop(Serial, 5000, 10);
}
```

`benchEnd(Serial)` prints one precise measurement: `bench: 1000.1250us (16002 ticks)`. For repeated long measurements use `benchEndStat(Serial, window)`. It rounds values to whole microseconds:

```text
bench: 1001, avr: 1000, min: 999, max: 1002 us
```

`benchLoop` uses the same `benchBegin()`/`benchEnd()` mechanism: the first call starts measurement, and each subsequent call ends it and starts another after logging. The return value and `min_us` are in microseconds; output is in milliseconds. `benchEndStat` and `benchLoop` share one statistics accumulator.

On AVR the library temporarily owns Timer1 and its overflow interrupt. `benchEnd()` restores the timer configuration.
On ESP the CPU frequency must remain unchanged between `benchBegin()` and `benchEnd()`.
Other architectures use `micros()` for measurement.

<a id="versions"></a>

## Versions
- v1.0.4
- v1.0

<a id="install"></a>
## Installation
- The library can be found by the name **Benchmark** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/Benchmark/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
