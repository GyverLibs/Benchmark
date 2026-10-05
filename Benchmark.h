#pragma once
#include <Arduino.h>

// начать измерение
void benchBegin();

// закончить измерение. Вернёт микросекунды. Опционально подключить лог
float benchEnd(Stream* log = nullptr);

// закончить измерение. Вернёт микросекунды. С логом
float benchEnd(Stream& log);

// измеритель loop, передать например (Serial, millis(), 5). window - размер окна скользящего среднего, 0 чтобы отключить
uint32_t benchLoop(Stream& log, uint32_t uptime, uint32_t min_time, uint16_t window = 10);

// получить размер свободной памяти
size_t getFreeHeap();