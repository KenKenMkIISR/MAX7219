// Golden Fish animation for PIC16F18313 by KENKEN
// MAX7219 64x8 dot LED Matrix

/*
 * MAX7219 x8
 * RA0 SDO
 * RA1 SCK
 * RA2 SDI (not use)
 * RA4 CS
 */

#include "mcc_generated_files/mcc.h"

#define CS_H LATA=0x10
#define CS_L LATA=0x00

const uint8_t font_fish[][16]={
	// Golden Fish 16x8 x3 patterns
	{
		0x1e,0x06,
		0x3f,0x1e,
		0x7f,0xbe,
		0xdf,0xf0,
		0xff,0xe0,
		0x7f,0xfe,
		0x3f,0x9f,
		0x0e,0xc0
	},{
		0x1e,0x02,
		0x3f,0x1f,
		0x7f,0xbe,
		0xdf,0xfc,
		0xff,0xe0,
		0x7f,0xf0,
		0x3f,0xde,
		0x0e,0x67
	},{
		0x1e,0x00,
		0x3f,0x0f,
		0x7f,0x9f,
		0xdf,0xfc,
		0x7f,0xe0,
		0xff,0xf8,
		0x3f,0xdf,
		0x0e,0x67
	}
};
const uint8_t font_fish2[][16]={
	// Golden Fish 16x8 x3 patterns(mirror)
	{
		0x60,0x78,
		0x78,0xfc,
		0x7d,0xfe,
		0x0f,0xfb,
		0x07,0xff,
		0x7f,0xfe,
		0xf9,0xfc,
		0x03,0x70,
	},{
		0x40,0x78,
		0xf8,0xfc,
		0x7d,0xfe,
		0x3f,0xfb,
		0x07,0xff,
		0x0f,0xfe,
		0x7b,0xfc,
		0xe6,0x70,
	},{
		0x00,0x78,
		0xf0,0xfc,
		0xf9,0xfe,
		0x3f,0xfb,
		0x07,0xfe,
		0x1f,0xff,
		0xfb,0xfc,
		0xe6,0x70,
	}
};
const uint8_t font_plants[][8]={
	// Plants 7x8 x3 patterns
	{
		0x88,
		0x84,
		0x54,
		0x54,
		0x68,
		0x50,
		0x20,
		0x20
	},{
		0x54,
		0x54,
		0x4c,
		0x58,
		0x70,
		0x50,
		0x20,
		0x20
	},{
		0x92,
		0x54,
		0x6c,
		0x28,
		0x70,
		0x50,
		0x20,
		0x20
	}
};
const uint8_t font_crab[][12]={
	// Crab 10x6 x2 patterns
	{
		0x12,0x00,
		0xbf,0x40,
		0x7f,0x80,
		0xbf,0x40,
		0x7f,0x80,
		0xa1,0x40
	},{
		0x92,0x40,
		0x7f,0x80,
		0xbf,0x40,
		0x7f,0x80,
		0xbf,0x40,
		0x52,0x80
	}
};

uint8_t vram[64],*vramp;
uint8_t i,j;
const uint8_t *bmp;
uint8_t bmpxsize,bmpysize;
uint8_t row,t;
int8_t fish_x;
int8_t fish_y=0;
int8_t fish_y_dir=1;
uint8_t fish_pattern=0;
int8_t bubble_x;
int8_t bubble_y=-1;
int8_t crab_x=0;
int8_t crab_dir=1;

volatile uint16_t delaycounter=0;
void one_ms_interrupt(void){
	delaycounter++;
}
void delayms(uint16_t t){
	while(delaycounter<t)
		asm("SLEEP"); // Go to Idle mode
	delaycounter=0;
}

void clrvram(void){
	vramp=vram;
	for(i=0;i<64;i++) *vramp++=0;
}

void setp(uint8_t x,uint8_t y){
	if(x>=64) return;
	if(y>=8) return;
	vramp=vram+(y<<3)+(x>>3);
	*vramp=*vramp | (1<<(7-(x & 7)));
}

void clrp(uint8_t x,uint8_t y){
	if(x>=64) return;
	if(y>=8) return;
	vramp=vram+(y<<3)+(x>>3);
	*vramp=*vramp & ~(1<<(7-(x & 7)));
}

void setbmp(uint8_t x,uint8_t y){
// set bitmap at (x,y) size (bmpxsize,bmpysize) bmp:bitmap address
	for(j=0;j<bmpysize;j++){
		for(i=0;i<bmpxsize;i++){
			if((i & 7)==0) t=*bmp++;
			if(t & 0x80) setp(x+i,y);
			else clrp(x+i,y);
			t<<=1;
		}
		y++;
	}
}

void clrbmp(uint8_t x,uint8_t y){
// clear bitmap at (x,y) size (bmpxsize,bmpysize)
	for(j=0;j<bmpysize;j++){
		for(i=0;i<bmpxsize;i++){
			clrp(x+i,y);
		}
		y++;
	}
}

void putled(void){
	vramp=vram;
	for(row=1;row<=8;row++){
		CS_L;
		for(i=0;i<8;i++){
			SPI1_Exchange8bit(row);
			SPI1_Exchange8bit(*vramp++);
		}
		CS_H;
	}
}

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
	TMR0_SetInterruptHandler(one_ms_interrupt);

	// Enable the Global Interrupts
	INTERRUPT_GlobalInterruptEnable();

	// Enable the Peripheral Interrupts
	INTERRUPT_PeripheralInterruptEnable();

	CPUDOZE=0x80; // Idle Enable

	// init MAX7219
	CS_L;
	for(i=0;i<8;i++){
		// Not Shutdown Mode
		SPI1_Exchange8bit(0x0c);
		SPI1_Exchange8bit(0x01);
	}
	CS_H;
	CS_L;
	for(i=0;i<8;i++){
		// No Decode Mode
		SPI1_Exchange8bit(0x09);
		SPI1_Exchange8bit(0x00);
	}
	CS_H;
	CS_L;
	for(i=0;i<8;i++){
		// Set Briteness
		SPI1_Exchange8bit(0x0a);
		SPI1_Exchange8bit(0x05);
	}
	CS_H;
	CS_L;
	for(i=0;i<8;i++){
		// Scan All LEDs
		SPI1_Exchange8bit(0x0b);
		SPI1_Exchange8bit(0x07);
	}
	CS_H;
	clrvram();
	putled();

    while (1)
    {
		// Move golden fish right to left
		for(fish_x=70;fish_x>=-20;fish_x--){
			// set plants bitmap
			bmpxsize=7;
			bmpysize=8;
			if(fish_pattern==0){
				bmp=font_plants[0];setbmp( 5,0);
				bmp=font_plants[2];setbmp(23,0);
				bmp=font_plants[1];setbmp(42,0);
			}
			else if(fish_pattern==1){
				bmp=font_plants[1];setbmp( 5,0);
				bmp=font_plants[0];setbmp(23,0);
				bmp=font_plants[2];setbmp(42,0);
			}
			else {
				bmp=font_plants[2];setbmp( 5,0);
				bmp=font_plants[1];setbmp(23,0);
				bmp=font_plants[0];setbmp(42,0);
			}
			// set crab bitmap
			bmpxsize=10;
			bmpysize=6;
			bmp=font_crab[crab_x & 1];setbmp(crab_x,2);
			// set golden fish bitmap
			bmpxsize=16;
			bmpysize=8;
			bmp=font_fish[fish_pattern];setbmp(fish_x,fish_y);
			// set bubble
			if(bubble_y>=0) setp(bubble_x,bubble_y);

			putled();
			delayms(100);

			// clear golden fish
			bmpxsize=16;
			bmpysize=8;
			clrbmp(fish_x,fish_y);
			// clear crab
			bmpxsize=10;
			bmpysize=6;
			clrbmp(crab_x,2);
			// clear bubble
			if(bubble_y>=0) clrp(bubble_x,bubble_y--);

			// move characters
			if(fish_x & 1){
				fish_pattern++;
				if(fish_pattern==3) fish_pattern=0;
			}
			if((fish_x & 3)==0) fish_y+=fish_y_dir;
			if(fish_y<-1) fish_y_dir=1;
			else if(fish_y>1){
				fish_y_dir=-1;
				bubble_y=7;
				bubble_x=fish_x-8;
			}
			if((fish_x & 7)==2) crab_x+=crab_dir;
			if(crab_x==0) crab_dir=1;
			else if(crab_x==54) crab_dir=-1;
		}

		// Move golden fish left to right
		for(fish_x=-20;fish_x<=70;fish_x++){
			// set plants bitmap
			bmpxsize=7;
			bmpysize=8;
			if(fish_pattern==0){
				bmp=font_plants[0];setbmp( 5,0);
				bmp=font_plants[2];setbmp(23,0);
				bmp=font_plants[1];setbmp(42,0);
			}
			else if(fish_pattern==1){
				bmp=font_plants[1];setbmp( 5,0);
				bmp=font_plants[0];setbmp(23,0);
				bmp=font_plants[2];setbmp(42,0);
			}
			else {
				bmp=font_plants[2];setbmp( 5,0);
				bmp=font_plants[1];setbmp(23,0);
				bmp=font_plants[0];setbmp(42,0);
			}
			// set crab bitmap
			bmpxsize=10;
			bmpysize=6;
			bmp=font_crab[crab_x & 1];setbmp(crab_x,2);
			// set golden fish bitmap
			bmpxsize=16;
			bmpysize=8;
			bmp=font_fish2[fish_pattern];setbmp(fish_x,fish_y);
			// set bubble
			if(bubble_y>=0) setp(bubble_x,bubble_y);

			putled();
			delayms(100);

			// clear golden fish
			bmpxsize=16;
			bmpysize=8;
			clrbmp(fish_x,fish_y);
			// clear crab
			bmpxsize=10;
			bmpysize=6;
			clrbmp(crab_x,2);
			// clear bubble
			if(bubble_y>=0) clrp(bubble_x,bubble_y--);

			// move characters
			if(fish_x & 1){
				fish_pattern++;
				if(fish_pattern==3) fish_pattern=0;
			}
			if((fish_x & 3)==0) fish_y+=fish_y_dir;
			if(fish_y<-1) fish_y_dir=1;
			else if(fish_y>1){
				fish_y_dir=-1;
				bubble_y=7;
				bubble_x=fish_x+24;
			}
			if((fish_x & 7)==2) crab_x+=crab_dir;
			if(crab_x==0) crab_dir=1;
			else if(crab_x==54) crab_dir=-1;
		}
    }
}
/**
 End of File
*/