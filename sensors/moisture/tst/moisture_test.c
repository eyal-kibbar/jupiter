#include "ganymede.h"
#include "moisture.h"
#include "logging.h"


#include <avr/io.h>
#include <util/delay.h>


void setup()
{
    DDRC = 0b00000000;
    ADMUX = 0b01100001;
    //ADCSRA = 0b10000111;
    ADCSRA = 0b10000111;

}

void loop()
{
    //LOG_INFO(moisture, "testing");

    // Start an ADC conversion by setting ADSC bit (bit 6)
    ADCSRA = ADCSRA | (1 << ADSC);

    // Wait until the ADSC bit has been cleared
    //while(ADCSRA & (1 << ADSC));
    gmd_wfe((volatile uint8_t*)0x7A, 1 << ADSC, 0);

    LOG_INFO(moisture, "%d", ADCH);

    gmd_delay(10000);
}



TASK(STACK_SIZE);
