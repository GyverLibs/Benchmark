#include "Benchmark.h"

#if defined(AVR)
static volatile uint16_t _ovf_count = 0;
static uint8_t _saved_tccr1a;
static uint8_t _saved_tccr1b;
static uint8_t _saved_timsk1;
static uint16_t _saved_tcnt1;

ISR(TIMER1_OVF_vect) {
    _ovf_count++;
}
#else
static uint32_t _count = 0;
#if defined(ESP8266) || defined(ESP32)
static uint16_t _cpu_mhz = 0;
#endif
#endif

static uint32_t _last_ticks = 0;
static bool _active = false;

struct BenchStats {
    uint32_t min = 0;
    uint32_t max = 0;
    uint32_t avr = 0;
    bool started = false;
};

static BenchStats _stats;

static uint32_t _smooth(uint32_t value, uint32_t average, uint16_t window) {
    uint32_t divider = (uint32_t)window + 1;
    uint32_t delta = value > average ? value - average : average - value;
    uint32_t step = delta / divider * 2 + delta % divider * 2 / divider;
    return value > average ? average + step : average - step;
}

static void _printStatValue(Print& p, uint32_t us, bool print_ms) {
    if (print_ms) p.print(us / 1000.0f, 2);
    else p.print(us);
}

static void _benchPrintStats(Print& p, BenchStats& stats, uint32_t us, uint16_t window, bool print_ms) {
    if (!stats.started) {
        stats.min = stats.max = stats.avr = us;
        stats.started = true;
    } else {
        if (window) stats.avr = _smooth(us, stats.avr, window);
        if (us < stats.min) stats.min = us;
        if (us > stats.max) stats.max = us;
    }

    p.print(F("bench: "));
    _printStatValue(p, us, print_ms);
    if (window) {
        p.print(F(", avr: "));
        _printStatValue(p, stats.avr, print_ms);
    }
    p.print(F(", min: "));
    _printStatValue(p, stats.min, print_ms);
    p.print(F(", max: "));
    _printStatValue(p, stats.max, print_ms);
    p.println(print_ms ? F(" ms") : F(" us"));
}

void benchBegin() {
#if defined(AVR)
    uint8_t sreg = SREG;
    cli();
    if (!_active) {
        _saved_tccr1a = TCCR1A;
        _saved_tccr1b = TCCR1B;
        _saved_timsk1 = TIMSK1;
        _saved_tcnt1 = TCNT1;
    }
    TCCR1B = 0;
    TCCR1A = 0;
    TCNT1 = 0;
    _ovf_count = 0;
    TIFR1 = _BV(TOV1);
    TIMSK1 = _BV(TOIE1);
    TCCR1B = _BV(CS10);
    _active = true;
    SREG = sreg;
#elif defined(ESP8266) || defined(ESP32)
    _cpu_mhz = ESP.getCpuFreqMHz();
    _count = ESP.getCycleCount();
    _active = true;
#else
    _count = micros();
    _active = true;
#endif
}

static float _benchEnd() {
    uint32_t count = 0;
    _last_ticks = 0;
    if (!_active) return 0;

#if defined(AVR)
    uint8_t sreg = SREG;
    cli();
    TCCR1B = 0;
    uint16_t timer = TCNT1;
    uint16_t ovf = _ovf_count;
    if (TIFR1 & _BV(TOV1)) ovf++;
    count = ((uint32_t)ovf << 16) + timer;
    if (count >= 2) count -= 2;
    else count = 0;

    TIFR1 = _BV(TOV1);
    TCNT1 = _saved_tcnt1;
    TCCR1A = _saved_tccr1a;
    TIMSK1 = _saved_timsk1;
    TCCR1B = _saved_tccr1b;
    _active = false;
    SREG = sreg;
#elif defined(ESP8266) || defined(ESP32)
    count = ESP.getCycleCount() - _count;
    _active = false;
#else
    count = micros() - _count;
    _active = false;
#endif
    _last_ticks = count;

#if defined(AVR)
    return count * (float)(1000000.0f / F_CPU);
#elif defined(ESP8266) || defined(ESP32)
    return _cpu_mhz ? count / (float)_cpu_mhz : 0;
#else
    return count;
#endif
}

static uint32_t _benchUs(float us) {
#if defined(AVR) || defined(ESP8266) || defined(ESP32)
    return us > 0 ? (uint32_t)(us + 0.5f) : 0;
#else
    (void)us;
    return _last_ticks;
#endif
}

float benchEnd() {
    return _benchEnd();
}

float benchEnd(Print& p) {
    float us = _benchEnd();
    p.print(F("bench: "));
    p.print(us, 4);
    p.print(F(" us"));
#if defined(AVR) || defined(ESP8266) || defined(ESP32)
    p.print(F(" ("));
    p.print(_last_ticks);
    p.print(F(" ticks)"));
#endif
    p.println();
    return us;
}

float benchEndStat(Print& p, uint16_t window) {
    float us = _benchEnd();
    _benchPrintStats(p, _stats, _benchUs(us), window, false);
    return us;
}

void benchReset() {
    _stats = BenchStats();
}

uint32_t benchLoop(Print& p, uint32_t min_us, uint16_t window) {
    if (!_active) {
        benchBegin();
        return 0;
    }

    uint32_t us = _benchUs(_benchEnd());
    if (us >= min_us) _benchPrintStats(p, _stats, us, window, true);
    benchBegin();
    return us;
}

#if defined(AVR)
extern unsigned int __bss_end;
extern void* __brkval;
#endif

size_t getFreeHeap() {
#if defined(AVR)
    int i;
    return ((int)__brkval == 0) ? ((int)&i - (int)&__bss_end) : ((int)&i - (int)__brkval);
#elif defined(ESP8266) || defined(ESP32)
    return ESP.getFreeHeap();
#else
    return 0;
#endif
}
