/*
 * TestVUSB.c
 *
 * Created: 05.05.2015 15:36:01
 *  Author: IGOR K.
 */

#include "prj.h"

unsigned int volatile timer_ms[NUMB_MS_TMR];
unsigned int volatile system_timer_10ms = 0;
unsigned int volatile blink_led = 500;

// ����� ����� �� 2 �����, ������ ���� - ��������� ���������� ���./����
static uchar reportBuf[2] = {0, 0};
static uchar dataLength = 0, dataReceived = 0;

const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = 
{
	0x06, 0x00, 0xff,                       // USAGE_PAGE (Generic Desktop)
	0x09, 0x01,                             // USAGE (Vendor Usage 1)
	0xa1, 0x01,                             // COLLECTION (Application)
	0x15, 0x00,                             //    LOGICAL_MINIMUM (0)        // min. �������� ��� ������
	0x26, 0xff, 0x00,                       //    LOGICAL_MAXIMUM (255)      // max. �������� ��� ������, 255 ��� �� ��������, � ����� ��������� � 1 ����
	0x75, 0x08,                             //    REPORT_SIZE (8)
	0x95, 0x02,								//    REPORT_COUNT (2)             
	0x09, 0x00,                             //    USAGE (Undefined)
	0xb2, 0x02, 0x01,                       //    FEATURE (Data,Var,Abs,Buf)
	0xc0                                    // END_COLLECTION
};

static uchar    currentAddress;
static uchar    bytesRemaining;

USB_PUBLIC uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t *rq = (void *)data;
	
	usbMsgPtr = reportBuf;
	if ((rq -> bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
		switch(rq->bRequest)
		{
			// ���� ������ ������
			case USBRQ_HID_GET_REPORT:
				buildReport();
				return sizeof(reportBuf);
			// ���� ������ ������
			case USBRQ_HID_SET_REPORT:
				dataLength  = (uchar)rq->wLength.word;
				dataReceived = 0;
				if(dataLength  > sizeof(reportBuf)) // limit to buffer size
					dataLength  = sizeof(reportBuf);
					
				return USB_NO_MSG;	// ������������ ���������� � ����� ������ � usbFunctionWrite()
		}
	}
	
	return 0;
}

USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len) 
{
	uchar i;
	// ��������� ���������� ������ ������ � �����
	for (i = 0; dataReceived < dataLength && i < len; i++, dataReceived++)
	reportBuf[dataReceived] = data[i];
	if (reportBuf[0] == 1)
		LED_PORT |= LED_RED;
	else LED_PORT &= ~LED_RED; 
	return (dataReceived == dataLength);
}

void buildReport()
{
	if (LED_PIN & LED_RED)
		reportBuf[0] = 1;
	else reportBuf[0] = 0;
}

//USB_PUBLIC uchar usbFunctionRead(uchar *data, uchar len)
//{
	 //if(len > bytesRemaining)
		//len = bytesRemaining;
//
	 //uchar *buffer = reportBuf;
//
	 //if(!currentAddress)        // �� ���� ����� ������ ��� �� ��������.
	 //{                          
		 //if (LED_PIN & LED_GREEN )
			//reportBuf[0] = 1;
		//else reportBuf[0] = 0;
	 //}
//
	 //uchar j;
	 //for(j = 0; j < len; j++)
	 //data[j] = buffer[j + currentAddress];
//
	 //currentAddress += len;
	 //bytesRemaining -= len;
	 //return len;
//}

void hadUsbReset()
{
	 /* Disable interrupts during oscillator calibration since
     * usbMeasureFrameLength() counts CPU cycles.
     */
    cli();
    calibrateOscillator();
    sei();
}

void calibrateOscillator()
{
	uchar       step = 128;
	uchar       trialValue = 0, optimumValue;
	int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5); //F_CPU=16.5 ���

	/* do a binary search: */
	do
	{
		OSCCAL = trialValue + step;
		x = usbMeasureFrameLength();    /* proportional to current real frequency */
		if(x < targetValue)             /* frequency still too low */
		trialValue += step;
		step >>= 1;
	} while(step > 0);
	
	/* We have a precision of +/- 1 for optimum OSCCAL here */
	/* now do a neighborhood search for optimum value */
	optimumValue = trialValue;
	optimumDev = x; /* this is certainly far away from optimum */
	for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++)
	{
		x = usbMeasureFrameLength() - targetValue;
		if(x < 0)
		x = -x;
		if(x < optimumDev)
		{
			optimumDev = x;
			optimumValue = OSCCAL;
		}
	}
	OSCCAL = optimumValue;
}

int main(void)
{
	uchar i;
	
	wdt_enable(WDTO_1S);
	usbInit();
	timer0_init();
	timers_init();
	leds_init();
	
	//������������� �����������, ����� ���������� ���� ���������� ��� �����������
	usbDeviceDisconnect();
	for (i = 0; i < 250; i++)
	{
		wdt_reset();
		_delay_ms(2);
	}
		
    usbDeviceConnect(); // ���������� ����������
    sei(); // ��������� ����������
	
    // � ����������� ����� ����� ����������� ���������
    while(1) 
	{
		wdt_reset();
		timers_proc();
		leds_proc();
		usbPoll();
	}
	
    return 0;
	
	//usbInit(); // ���������������� USB
	//usbDeviceConnect(); // ���������� ����������
	//sei(); // ��������� ����������
	//// � ����������� ����� ����� ����������� ���������
	//PORTB = LED_GREEN;
	//while(1) usbPoll();
	//return 0;
}