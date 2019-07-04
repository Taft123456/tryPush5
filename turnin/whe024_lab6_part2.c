/* Author: whe024
 * Partner(s) Name: Wentao He
 * Lab Section: A21
 * Assignment: Lab #6  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B 	= 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

enum States{START, ONE, TWO, THREE, PAUSE, RESTART} state;

void Tick(){
	switch(state)
	{
		case START:
		{
			PORTB = 0x00;
			state = ONE; 
			break;
		}
		
		case ONE:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = PAUSE; 
				break;
			}
			else
			{
				state = TWO; 
				break;
			}
		}
		
		case TWO:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = PAUSE; 
				break;
			}
			else
			{
				state = THREE; 
				break;
			}
		}
		
		case THREE:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = PAUSE; 
				break;
			}
			else
			{
				state = ONE; 
				break;
			}
		}
		
		case PAUSE:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = PAUSE; 
				break;
			}
			else
			{
				state = RESTART; 
				break;
			}
		}
		
		case RESTART:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = ONE; 
				break;
			}
			else
			{
				state = RESTART; 
				break;
			}
			
		}
		default:
			break;
	}
	switch(state)
	{
		case START:
			break;
		
		case ONE:
		{
			PORTB = 0x01; 
			break;
		}
		
		case TWO:
		{
			PORTB = 0x02; 
			break;
		}
		
		case THREE:
		{
			PORTB = 0x04; 
			break;
		}
		
		case PAUSE:
			break;
		
		case RESTART:
			break;
		
		default:
			break;
	}    
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    DDRB = 0xFF;
    PORTA = 0xFF;
    PORTB = 0x00;   
    /* Insert your solution below */
    TimerSet(30);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag){};
	TimerFlag = 0;	
    }
    return 1;
}
