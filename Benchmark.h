#pragma once
#include <Arduino.h>

// начать измерение
void benchBegin();

// закончить измерение. Вернёт микросекунды
float benchEnd();

// закончить измерение и вывести микросекунды и такты
float benchEnd(Print& log);

// закончить измерение и вывести статистику в целых микросекундах
float benchEndStat(Print& log, uint16_t window = 10);

// сбросить статистику benchEndStat и benchLoop
void benchReset();

// begin/end измеритель loop. min_us в микросекундах, вывод в ms, window 0 отключает avr
uint32_t benchLoop(Print& log, uint32_t min_us = 0, uint16_t window = 10);

// получить размер свободной памяти
size_t getFreeHeap();
