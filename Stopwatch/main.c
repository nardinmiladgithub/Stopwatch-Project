/*
 * main.c
 *     Subject: Stop-watch Project
 *  Created on: Sep 10, 2024
 *      Author: Nardin Milad
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char modeFlag=0;    /* 0 for Increment, 1 for Count-down */
unsigned char most_hour=0,least_hour=0;
unsigned char most_min=0,least_min=0;
unsigned char most_sec=0,least_sec=0;
unsigned char incHourFlag=0;
unsigned char decHourFlag=0;
unsigned char incMinFlag=0;
unsigned char decMinFlag=0;
unsigned char incSecFlag=0;
unsigned char decSecFlag=0;
unsigned char checkFlag=0;

#define INCREMENT_TIME(least, most, least_max, most_max){ \
		if (least == least_max) \
		{ \
			least = 0; \
			if (most == most_max) \
				most = 0; \
			else most++; \
		} \
		else least++; \
}

#define DECREMENT_TIME(least, most, least_max, most_max){ \
		if (least == 0) \
        { \
			least = least_max; \
			if (most == 0) \
                most = most_max; \
			else most--; \
		} \
		else least--; \
}

void IO_Ports_configuration(void)
{
	DDRA |= 0x3F;
	/* Configure the first 6-pins in PORTA as output pins for six 7-segment */
	DDRB &= 0x00;       /* Configure PORTB as input port */
	PORTB |= 0xFF;      /* Activate internal pull-up resistors */
	DDRC |= 0x0F;       /* Configure the first 4-pins in PORTC as output pins */
	DDRD |=(1<<0);      /* Output pin for buzzer */
	DDRD &=~(1<<2)&~(1<<3);           /* Configure input pins */
	DDRD |=(1<<4)|(1<<5);             /* Output pin for LEDs */
	PORTD |=(1<<2);                   /* Activate internal pull-up resistor */
	PORTD |=(1<<4);                   /* Turn on red LED initially */
	PORTD &=~(1<<0)&~(1<<5);          /* Turn buzzer and yellow LED off initially */
	PORTC = (PORTC & 0xF0) | (0x00);  /* Initialize 7-segment by zeros */
}

void Timer1_configuration(void)
{
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);
	/*
	 * Enable FOC1A indicate that it is Non-PWM mode
	 * Enable WGM12 indicate CTC mode (mode number 4)
	 * Enable CS12, CS10 indicate that pre-scaler = 1024
	 */
	TCNT1 = 0;             /* Initial value register */
	OCR1A = 15625;         /* Compare value register */
	TIMSK |= (1<<OCIE1A);  /* Output Compare A Match Interrupt Enable */
	SREG |=(1<<7);
}

void INT0_configuration(void)
{
	GICR |=(1<<INT0);              /* Set module interrupt enable */
	MCUCR |=(1<<ISC01);            /* Configure falling edge */

}

void INT1_configuration(void)
{
	MCUCR |=(1<<ISC11)|(1<<ISC10); /* Configure raising edge */
	GICR |=(1<<INT1);              /* Set module interrupt enable */
}

void INT2_configurtion(void)
{
	MCUCSR &=~(1<<ISC2);           /* Configure falling edge */
	GICR |=(1<<INT2);              /* Set module interrupt enable */
}

void display(unsigned char least_sec, unsigned char most_sec, unsigned char least_min, unsigned char most_min, unsigned char least_hour, unsigned char most_hour)
{
	_delay_ms(2);
	// Display least significant digit of seconds
	PORTA = 0b00100000;
	PORTC = (PORTC & 0xF0) | (least_sec & 0x0F);
	_delay_ms(2);

	// Display most significant digit of seconds
	PORTA = 0b00010000;
	PORTC = (PORTC & 0xF0) | (most_sec & 0x0F);
	_delay_ms(2);

	// Display least significant digit of minutes
	PORTA = 0b00001000;
	PORTC = (PORTC & 0xF0) | (least_min & 0x0F);
	_delay_ms(2);

	// Display most significant digit of minutes
	PORTA = 0b00000100;
	PORTC = (PORTC & 0xF0) | (most_min & 0x0F);
	_delay_ms(2);

	// Display least significant digit of hours
	PORTA = 0b00000010;
	PORTC = (PORTC & 0xF0) | (least_hour & 0x0F);
	_delay_ms(2);

	// Display most significant digit of hours
	PORTA = 0b00000001;
	PORTC = (PORTC & 0xF0) | (most_hour & 0x0F);
	_delay_ms(2);
}

void Adjustment(void)
{
	// Hours increment
	if (!(PINB & (1<<1)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<1)))
		{
			if(incHourFlag == 0)
			{
			INCREMENT_TIME(least_hour, most_hour, 9, 9);
			incHourFlag = 1;
			}
		}
	}
	else incHourFlag = 0;

	// Hours decrement
	if (!(PINB & (1<<0)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<0)))
		{
			if(decHourFlag == 0)
			{
			DECREMENT_TIME(least_hour, most_hour, 9, 9);
			decHourFlag = 1;
			}
		}
	}
	else decHourFlag = 0;

	// Minutes increment
	if (!(PINB & (1<<4)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<4)))
		{
			if(incMinFlag == 0)
			{
			INCREMENT_TIME(least_min, most_min, 9, 5);
			incMinFlag = 1;
			}
		}
	}
	else incMinFlag = 0;

	// Minutes decrement
	if (!(PINB & (1<<3)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<3)))
		{
			if(decMinFlag == 0)
			{
			DECREMENT_TIME(least_min, most_min, 9, 5);
			decMinFlag = 1;
			}
		}
	}
	else decMinFlag = 0;

	// Seconds increment
	if (!(PINB & (1<<6)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<6)))
		{
			if(incSecFlag == 0)
			{
			INCREMENT_TIME(least_sec, most_sec, 9, 5);
			incSecFlag = 1;
			}
		}
	}
	else incSecFlag = 0;

	// Seconds decrement
	if (!(PINB & (1<<5)))
	{
		_delay_ms(30);
		if (!(PINB & (1<<5)))
		{
			if(decSecFlag == 0)
			{
			DECREMENT_TIME(least_sec, most_sec, 9, 5);
			decSecFlag = 1;
			}
		}
	}
	else decSecFlag = 0;

	// Update 7-segment displays
	display(least_sec, most_sec, least_min, most_min, least_hour, most_hour);
}

ISR(INT0_vect)    /* Reset the stop-watch */
{
	TCNT1=0;
	least_sec=0,most_sec=0;
	least_min=0,most_min=0;
	least_hour=0,most_hour=0;
	/* Update 7-segment displays */
	display(least_sec, most_sec, least_min, most_min, least_hour, most_hour);
}

ISR(INT1_vect)    /* Pause TIMER1 */
{
	TCCR1B &= 0x00;
}

ISR(INT2_vect)    /* Resume the count-down */
{
	Timer1_configuration();
}

ISR(TIMER1_COMPA_vect)
{
	if (modeFlag == 0)  /* Count-up mode */
	{
		INCREMENT_TIME(least_sec, most_sec, 9, 5);  // Increment seconds
		if (least_sec == 0 && most_sec == 0)        // Increment minutes
		{
			INCREMENT_TIME(least_min, most_min, 9, 5);
		}
		if (least_min == 0 && most_min == 0 && least_sec == 0 && most_sec == 0)  // Increment hours
		{
			INCREMENT_TIME(least_hour, most_hour, 9, 9);
		}
	}

	else if (modeFlag == 1)  /* Count-down mode */
	{
		DECREMENT_TIME(least_sec, most_sec, 9, 5);  // Decrement seconds
		if (least_sec == 9 && most_sec == 5)        // Decrement minutes
		{
			DECREMENT_TIME(least_min, most_min, 9, 5);
		}
		if (least_min == 9 && most_min == 5 && least_sec == 9 && most_sec == 5)  // Decrement hours
		{
			DECREMENT_TIME(least_hour, most_hour, 9, 9);
		}
        // Special case when timer reach 00:00:00
		if (least_sec == 0 && most_sec == 0 && least_min == 0 && most_min == 0 && least_hour == 0 && most_hour == 0)
		{
			TCCR1B &= 0x00;     // Stop time
			PORTD |= (1 << 0);  // Turn on buzzer (alarm)
		}
	}

	/* Update 7-segment displays */
	display(least_sec, most_sec, least_min, most_min, least_hour, most_hour);
}

int main(void)
{
	SREG |= (1<<7);           // Global interrupt enable I-BIT
	PORTD |= (1<<4);          // Turn on the red LED (default mode)
	PORTD &=~ (1<<5);         // Turn off the yellow LED
	IO_Ports_configuration();
	Timer1_configuration();
	INT0_configuration();
	INT1_configuration();
	INT2_configurtion();
	while(1)
	{
		if(!(PINB & (1<<7)))
		{
			_delay_ms(30);     /* To avoid de-bounce issue */
			if(!(PINB & (1<<7)))
			{
				if(checkFlag == 0)
				{
				modeFlag ^=(1<<0);    // Toggle flag of mode
				PORTD ^= (1<<4);      // Turn on, off the red LED
				PORTD ^= (1<<5);      // Turn on, off the yellow LED
				checkFlag = 1;
				}
			}
		}
		else checkFlag = 0;

		Adjustment();

		/* Refresh 7-segment displays */
		display(least_sec, most_sec, least_min, most_min, least_hour, most_hour);
	}
}
