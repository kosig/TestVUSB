/*
 * utils.c
 *
 *
 * Created: 19.05.2015 11:18:45
 *  Author: IGOR
 */ 

#include "utils.h"
#include "prj.h"

// Инициализация таймеров, которые могут быть использованы при работе программы 
void timers_init()
{
	unsigned char t;
	
	for (t = 0; t < NUMB_MS_TMR; t++)
	{
		timer_ms[t] = 0;
	}
}

// Обработка значений таймеров
void timers_proc()
{
	unsigned int ms;
	unsigned char timer;
	
	cli();
	ms = system_timer_10ms;
	system_timer_10ms = 0;
	sei();
	
	for (timer = 0; timer < NUMB_MS_TMR; timer++)
	{
		cli();
		timer_ms [timer] += ms;
		if (timer_ms [timer] < ms)
			timer_ms [timer] = 0xFFFF;
		sei();
	}
}

void leds_init()
{
	 LED_DIR = LED_GREEN | LED_RED;
	 timer_ms[LED_BLINK_TMR] = 0; 
}

void leds_proc()
{
	if (timer_ms[LED_BLINK_TMR] == blink_led)
	{
		timer_ms[LED_BLINK_TMR] = 0;
		LED_PORT ^= LED_GREEN;
	}
}

