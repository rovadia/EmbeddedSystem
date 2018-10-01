/*
 * Project5.c
 *
 * Created: 5/31/2018 1:55:08 PM
 * Author : rovad
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/io.h>
#include "avr.h"
#include "avr.c"
#include "lcd.h"
#include "lcd.c"
#include <math.h>
#include <stdbool.h>
#define SONG_LENGTH 21




struct note {
	int f; //frequency
	double d; //duration
};

// length is 21
struct note frog[SONG_LENGTH]={
	{587, 2.0},
	{740, 1.5},
	{587, 1.0},
	{587, 0.5},
	{784, 1.0},
	{587, 1.0},
	{523, 1.0},
	{587, 2.0},
	{440*2, 1.5},
	{587, 1.0},
	{587, 0.5},
	{466*2, 1.0},
	{440*2, 1.0},
	{740, 1.0},
	{587, 1.0},
	{440*2, 2.0},
	{587*2, 0.5},
	{523, 1.0},
	{523, 1.5},
	{659, 1.0},
	{440*2, 2.5}
};


	
void playNote (int f, double d, float tempo, float volume) {
	int t = round(25000/(2*f));
	int k= (d*200)*tempo;
	int i;
	for (i=0;i<k;++i) {
		SET_BIT(PORTB,3);
		wait_avr(t*(1+volume));
		CLR_BIT(PORTB,3);
		wait_avr(t*(1-volume));
	}
}

void playMusic(struct note *song, int song_length, float tempo, float volume) {
	int i;
	for (i=0; i<song_length;++i) {
		playNote(song[i].f,song[i].d, tempo, volume);
		wait_avr(1000);
	}
}

struct stats {
	float current; //exactly what we get from a2c
};




unsigned short get_a2c() {
	SET_BIT(ADCSRA, ADSC);
	unsigned short result;
	
	while((ADCSRA & (1<<ADSC)));
	result = ADC;

	return result;
}


int main(void)
{
	
	SET_BIT(DDRB,0);
	SET_BIT(DDRB,1);
	SET_BIT(DDRB,2);
	SET_BIT(DDRB,3);
	SET_BIT(ADCSRA, ADEN);
	SET_BIT(ADMUX,6);
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
	
	SET_BIT(DDRC,0);
	SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);
	SET_BIT(DDRC,4);
	
	
	
	char* voltString="Voltage:";
	
	ini_lcd();
	clr_lcd();

	pos_lcd(0,2);
	puts_lcd2(voltString);
	
	char* songString = "Song:Frog";
	pos_lcd(1,3);
	puts_lcd2(songString);
	char buf[5];
	int i=0;
	
    /* Replace with your application code */
    while (1) 
    {
		
		unsigned short volt=get_a2c();
		float value = volt/1023.0*5.00;
		sprintf(buf,"%0.2f",value);
		
		pos_lcd(0,10);
		puts_lcd2(buf);
		
		if (value>4.95) {
			playNote(frog[i].f,frog[i].d, 0.5, 1.0);
			i++;
			if (i==SONG_LENGTH) {
				i=0;
			}
		}
		
		if (value<0.01) {
			playNote(frog[i].f,frog[i].d, 0.5, 1.0);
			i--;
			if (i==0) {
				i=SONG_LENGTH-1;
			}
		}
		
		if (i%5==0) {
			SET_BIT(PORTC,0);
			CLR_BIT(PORTC,1);
			CLR_BIT(PORTC,2);
			CLR_BIT(PORTC,3);
			CLR_BIT(PORTC,4);
		}
		else if (i%5==1) {
			CLR_BIT(PORTC,0);
			SET_BIT(PORTC,1);
			CLR_BIT(PORTC,2);
			CLR_BIT(PORTC,3);
			CLR_BIT(PORTC,4);
		}
		else if (i%5==2) {
			CLR_BIT(PORTC,0);
			CLR_BIT(PORTC,1);
			SET_BIT(PORTC,2);
			CLR_BIT(PORTC,3);
			CLR_BIT(PORTC,4);
		}
		else if (i%5==3) {
			CLR_BIT(PORTC,0);
			CLR_BIT(PORTC,1);
			CLR_BIT(PORTC,2);
			SET_BIT(PORTC,3);
			CLR_BIT(PORTC,4);
		}
		else {
			CLR_BIT(PORTC,0);
			CLR_BIT(PORTC,1);
			CLR_BIT(PORTC,2);
			CLR_BIT(PORTC,3);
			SET_BIT(PORTC,4);
		}
		
		
		wait_avr(1000);
    }
}

