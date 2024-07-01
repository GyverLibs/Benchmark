#pragma once
#include <Arduino.h>

// начать измерение
void benchBegin();

// закончить измерение. Вернёт кол-во тактов. Опционально подключить лог
uint32_t benchEnd(Stream* log = nullptr);