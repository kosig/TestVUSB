/*
 * prj.h
 *
 * Created: 19.05.2015 8:37:50
 *  Author: IGOR
 */ 


#ifndef PRJ_H_
#define PRJ_H_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "usbdrv/usbdrv.h"
#include "usbdrv/usbconfig.h"
#include "timer.h"
#include "utils.h"

#define USB_LED_OFF 0
#define USB_LED_ON  1
#define USB_DATA_OUT 2
#define USB_DATA_WRITE 3
#define USB_DATA_IN 4


#define NUMB_MS_TMR	3	// 3 миллисекундных таймера
#define LED_BLINK_TMR	0

extern unsigned int volatile timer_ms[NUMB_MS_TMR];
extern unsigned int volatile system_tick;
extern unsigned int volatile system_timer_10ms;
extern unsigned int volatile blink_led;


#endif /* PRJ_H_ */