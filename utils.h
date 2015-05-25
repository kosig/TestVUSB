/*
 * utils.h
 *
 * Created: 19.05.2015 11:24:53
 *  Author: IGOR
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#define LED_PORT PORTB
#define LED_DIR	DDRB
#define LED_PIN PINB
#define LED_GREEN (1<<PB3)
#define LED_RED (1<<PB4)

void timers_init();
void timers_proc();

void leds_init();
void leds_proc();


#endif /* UTILS_H_ */