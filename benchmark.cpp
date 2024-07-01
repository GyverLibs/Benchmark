#include "benchmark.h"

#if defined(AVR)
static volatile uint16_t _ovf_count = 0;

ISR(TIMER1_OVF_vect) {
    _ovf_count++;
}
#elif defined(ESP8266) || defined(ESP32)
static volatile uint32_t _count = 0;
#else
#error "No benchmark implementation for given platform"
#endif

void benchBegin() {
#if defined(AVR)
    TCCR1A = TCCR1B = TCNT1 = _ovf_count = 0;  // Сброс таймера
    TIFR1 |= (1 << TOV1);
    TIMSK1 = (1 << TOIE0);  // Прерывание переполнения
    TCCR1B = (1 << CS10);   // Старт таймера
#elif defined(ESP8266) || defined(ESP32)
    _count = ESP.getCycleCount();
#endif
}

uint32_t benchEnd(Stream* log) {
#if defined(AVR)
    TCCR1B = 0;                                // остановить таймер
    uint32_t count = TCNT1 - 2;                // минус два такта на действия
    count += ((uint32_t)_ovf_count * 0xFFFF);  // с учетом переполнений
#elif defined(ESP8266) || defined(ESP32)
    uint32_t count = ESP.getCycleCount() - _count;
#endif
    if (log) {
        log->print("bench: ");
        log->print(count * (float)(1000000.0f / F_CPU), 4);
        log->print("us (");
        log->print(count);
        log->println(" ticks)");
    }
    return count;
}