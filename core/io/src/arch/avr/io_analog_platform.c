#include "io_platform.h"
#include "ganymede.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

static uint8_t adcsra;

void io_analog_init()
{
    PRR &= ~(_BV(PRADC));

#if F_CPU >= 16000000 // 16 MHz / 128 = 125 KHz
    adcsra = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
#elif F_CPU >= 8000000 // 8 MHz / 64 = 125 KHz
    adcsra = _BV(ADPS2) | _BV(ADPS1);
#elif F_CPU >= 4000000 // 4 MHz / 32 = 125 KHz
    adcsra = _BV(ADPS2) | _BV(ADPS0);
#elif F_CPU >= 2000000 // 2 MHz / 16 = 125 KHz
    adcsra = _BV(ADPS2);
#elif F_CPU >= 1000000 // 1 MHz / 8 = 125 KHz
    adcsra = _BV(ADPS1) | _BV(ADPS0);
#else // 128 kHz / 2 = 64 KHz -> This is the closest you can get, the prescaler is 2
    adcsra = _BV(ADPS0);
#endif
	// enable a2d conversions
    adcsra |= _BV(ADEN);
    DIDR0 = _BV(ADC5D) | _BV(ADC4D)| _BV(ADC3D)| _BV(ADC2D)| _BV(ADC1D)| _BV(ADC0D);
}


void io_analog_read(uint8_t pin, enum io_analog_ref_e ref, uint16_t* val)
{
    uint8_t mux;
    uint8_t* p_val = (uint8_t*)val;

    switch (ref) {
        case IO_ANALOG_REF_AREF:     mux = 0; break;
        case IO_ANALOG_REF_AVcc:     mux = _BV(REFS0); break;
        case IO_ANALOG_REF_INTERNAL: mux = _BV(REFS0) | _BV(REFS1); break;
        default: return;
    }

    ADMUX = mux | (pin & 0xF);

    ADCSRA = adcsra | _BV(ADSC);

    // wait for ADC conversation to end
    //gmd_wfe(&ADCSRA, _BV(ADSC), _BV(ADSC), 0);
    // polling
    while (0 != (ADCSRA & _BV(ADSC)));

    // hardware bug: ADCL must be read before ADCH
    p_val[0] = ADCL;
    p_val[1] = ADCH;
}
