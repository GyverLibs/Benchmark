#pragma once
#include <Arduino.h>

// начать измерение
void benchBegin();

// закончить измерение. Вернёт микросекунды. Опционально подключить лог
float benchEnd(Stream* log = nullptr);

// закончить измерение. Вернёт микросекунды. С логом
float benchEnd(Stream& log);

// получить размер свободной памяти
size_t getFreeHeap();