#include "Benchmark.h"

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

float benchEnd(Stream* log) {
#if defined(AVR)
    TCCR1B = 0;                                // остановить таймер
    uint32_t count = TCNT1 - 2;                // минус два такта на действия
    count += ((uint32_t)_ovf_count * 0xFFFF);  // с учетом переполнений
#elif defined(ESP8266) || defined(ESP32)
    uint32_t count = ESP.getCycleCount() - _count;
#endif

    float us = count * (float)(1000000.0f / F_CPU);
    if (log) {
        log->print("bench: ");
        log->print(us, 4);
        log->print("us (");
        log->print(count);
        log->println(" ticks)");
    }
    return us;
}

float benchEnd(Stream& log) {
    return benchEnd(&log);
}

uint32_t benchLoop(Stream& log, uint32_t uptime, uint32_t min_time, uint16_t window) {
    static uint32_t tmr = 0;
    static uint32_t minv = 0xffffffff, maxv = 0, avr;

    uint32_t t = uptime - tmr;
    if (tmr && t >= min_time) {
        if (!tmr) avr = t;
        if (window) avr += (int32_t(t) - int32_t(avr)) * 2 / (window + 1);
        if (t < minv) minv = t;
        if (t > maxv) maxv = t;

        log.print("loop: ");
        log.print(t);
        if (window) {
            log.print(", avr: ");
            log.print(avr);
        }
        log.print(", min: ");
        log.print(minv);
        log.print(", max: ");
        log.print(maxv);
        log.println();
    }
    tmr = uptime;
    return t;
}

#if defined(AVR)
extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void* __brkval;
#endif

size_t getFreeHeap() {
#if defined(AVR)
    int i;
    return ((int)__brkval == 0) ? ((int)&i - (int)&__bss_end) : ((int)&i - (int)__brkval);
#elif defined(ESP8266) || defined(ESP32)
    return ESP.getFreeHeap();
#else
#error "No free heap implementation for given platform"
#endif
}