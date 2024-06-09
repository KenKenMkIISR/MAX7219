// Train animation for PIC16F18313 by KENKEN
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

const uint8_t font_densha[]={
	// densha
	0x18,0x30,
	0xFF,0xFE,
	0x49,0x24,
	0x49,0x24,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_shinkansen1[]={
	// shinkansen(head)
	0x02,0x00,
	0x0F,0xFE,
	0x1F,0xFE,
	0x7A,0xAA,
	0xCF,0xFE,
	0xFF,0xFE,
	0x7F,0xFE,
	0x14,0x14
};
const uint8_t font_shinkansen2[]={
	// shinkansen(middle)
	0x00,0x00,
	0xFF,0xFE,
	0xFF,0xFE,
	0xAA,0xAA,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_shinkansen3[]={
	// shinkansen(tail)
	0x00,0x80,
	0xFF,0xE0,
	0xFF,0xF0,
	0xAA,0xBC,
	0xFF,0xE6,
	0xFF,0xFE,
	0xFF,0xFC,
	0x50,0x50
};
const uint8_t font_sl[]={
	// steam locomotive
	0xF0,0x00,
	0x63,0xFE,
	0x63,0x1A,
	0xFF,0x1C,
	0xFF,0xFC,
	0xFF,0xFC,
	0xFF,0xFE,
	0x50,0xD8
};
const uint8_t font_tansuisha[]={
	// tender car
	0x00,0x00,
	0x00,0x00,
	0x3F,0xF8,
	0x7F,0xFC,
	0x7F,0xFE,
	0x7F,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_kyakusha[]={
	// Passenger Car
	0x00,0x00,
	0xFF,0xFE,
	0x49,0x24,
	0x49,0x24,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_denkikikansha[]={
	// Electric locomotives
	0x0C,0x18,
	0x7F,0xFE,
	0x9F,0xFE,
	0x9F,0x02,
	0xFE,0xFE,
	0xFD,0xAA,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_diesel[]={
	// Diesel locomotive
	0x00,0x00,
	0x01,0xF0,
	0x01,0x10,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_kamotsu1[]={
	// Container car1
	0x00,0x00,
	0x7E,0xFC,
	0x7E,0xFC,
	0x7E,0xFC,
	0x7E,0xFC,
	0x7E,0xFC,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_kamotsu2[]={
	// Container car2
	0x00,0x00,
	0x7F,0xFC,
	0x7F,0xFC,
	0x7F,0xFC,
	0x7F,0xFC,
	0x60,0x0C,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_kamotsu3[]={
	// Tank car
	0x00,0x00,
	0x03,0x80,
	0x3F,0xF8,
	0x7F,0xFC,
	0x7F,0xFC,
	0x3F,0xF8,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_kamotsu4[]={
	// stone car1
	0x00,0x00,
	0x7F,0xFC,
	0x7F,0xFC,
	0x7F,0xFC,
	0x3F,0xF8,
	0x1F,0xF0,
	0xFF,0xFE,
	0x24,0x48
};
const uint8_t font_kamotsu5[]={
	// stone car2
	0x00,0x00,
	0x7F,0xFC,
	0x7F,0xFC,
	0x7E,0xFC,
	0x3C,0x78,
	0x18,0x30,
	0xFF,0xFE,
	0x24,0x48
};

uint8_t vram[64];

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
	uint8_t *vramp=vram;
	for(uint8_t i=0;i<64;i++) *vramp++=0;
}

void setp(uint8_t x,uint8_t y){
	if(x>=64) return;
	if(y>=8) return;
	uint8_t *vramp=vram+(y<<3)+(x>>3);
	*vramp=*vramp | (1<<(7-(x & 7)));
}

void clrp(uint8_t x,uint8_t y){
	if(x>=64) return;
	if(y>=8) return;
	uint8_t *vramp=vram+(y<<3)+(x>>3);
	*vramp=*vramp & ~(1<<(7-(x & 7)));
}

void setbmp(int8_t x,const uint8_t *bmp){
// draw bitmap at vram x posiiton (left to right)
	uint8_t d;
	for(uint8_t j=0;j<8;j++){
		for(uint8_t i=0;i<16;i++){
			if((i & 7)==0) d=*bmp++;
			if(d & 0x80) setp((uint8_t)(x+i),j);
			else clrp((uint8_t)(x+i),j);
			d<<=1;
		}
	}
}

void setbmp_r(int8_t x,const uint8_t *bmp){
// draw bitmap at vram x posiiton (right to left)
	uint8_t d;
	for(uint8_t j=0;j<8;j++){
		for(uint8_t i=0;i<16;i++){
			if((i & 7)==0) d=*bmp++;
			if(d & 0x80) setp((uint8_t)(x-i),j);
			else clrp((uint8_t)(x-i),j);
			d<<=1;
		}
	}
}

void putled(void){
// transfer vram data to LED matrix
	uint8_t *vramp=vram;
	for(uint8_t row=1;row<=8;row++){
		CS_L;
		for(uint8_t i=0;i<8;i++){
			SPI1_Exchange8bit(row);
			SPI1_Exchange8bit(*vramp++);
		}
		CS_H;
	}
}

void settrain_f(int16_t x,uint8_t n,const uint8_t *bmp){
	// set train bitmap with x position check (forward)
	// x: head x position
	// n: train number (1...)
	if(x>=(n<<4)+63) return;
	n--;
	x-=n<<4;
	if(x<0) return;
	setbmp_r(x,bmp);
}

void settrain_b(int16_t x,uint8_t n,const uint8_t *bmp){
	// set train bitmap with x position check (backward)
	// x: head x position
	// n: train number (1...)
	if(x+(n<<4)<0) return;
	n--;
	x+=n<<4;
	if(x>=64) return;
	setbmp(x,bmp);
}

void anim_densha_f(void){
	// display and move densha forward (left to right)
	for(int16_t x=0;x<64+16*3;x++){
		clrvram();
		settrain_f(x,1,font_densha);
		settrain_f(x,2,font_densha);
		settrain_f(x,3,font_densha);
		putled();
		delayms(30);
	}
}

void anim_densha_b(void){
	// display and move densha backward (right to left)
	for(int16_t x=63;x>-16*3;x--){
		clrvram();
		settrain_b(x,1,font_densha);
		settrain_b(x,2,font_densha);
		settrain_b(x,3,font_densha);
		putled();
		delayms(30);
	}
}

void anim_shinkansen_f(void){
	// display and move shinkansen forward (left to right)
	for(int16_t x=0;x<64+16*5;x++){
		clrvram();
		settrain_f(x,1,font_shinkansen1);
		settrain_f(x,2,font_shinkansen2);
		settrain_f(x,3,font_shinkansen2);
		settrain_f(x,4,font_shinkansen2);
		settrain_f(x,5,font_shinkansen3);
		putled();
		delayms(15);
	}
}

void anim_shinkansen_b(void){
	// display and move shinkansen backward (right to left)
	for(int16_t x=63;x>-16*5;x--){
		clrvram();
		settrain_b(x,1,font_shinkansen1);
		settrain_b(x,2,font_shinkansen2);
		settrain_b(x,3,font_shinkansen2);
		settrain_b(x,4,font_shinkansen2);
		settrain_b(x,5,font_shinkansen3);
		putled();
		delayms(15);
	}
}

void anim_sl_f(void){
	// display and move SL forward (left to right)
	for(int16_t x=0;x<64+16*4;x++){
		clrvram();
		settrain_f(x,1,font_sl);
		settrain_f(x,2,font_tansuisha);
		settrain_f(x,3,font_kyakusha);
		settrain_f(x,4,font_kyakusha);
		putled();
		delayms(50);
	}
}

void anim_sl_b(void){
	// display and move SL backward (right to left)
	for(int16_t x=63;x>-16*4;x--){
		clrvram();
		settrain_b(x,1,font_sl);
		settrain_b(x,2,font_tansuisha);
		settrain_b(x,3,font_kyakusha);
		settrain_b(x,4,font_kyakusha);
		putled();
		delayms(50);
	}
}

void anim_kamatsu1_f(void){
	// display and move freight train(1) forward (left to right)
	for(int16_t x=0;x<64+16*6;x++){
		clrvram();
		settrain_f(x,1,font_denkikikansha);
		settrain_f(x,2,font_kamotsu2);
		settrain_f(x,3,font_kamotsu1);
		settrain_f(x,4,font_kamotsu1);
		settrain_f(x,5,font_kamotsu2);
		settrain_f(x,6,font_kamotsu1);
		putled();
		delayms(30);
	}
}

void anim_kamatsu1_b(void){
	// display and move freight train(1) backward (right to left)
	for(int16_t x=63;x>-16*6;x--){
		clrvram();
		settrain_b(x,1,font_denkikikansha);
		settrain_b(x,2,font_kamotsu2);
		settrain_b(x,3,font_kamotsu1);
		settrain_b(x,4,font_kamotsu1);
		settrain_b(x,5,font_kamotsu2);
		settrain_b(x,6,font_kamotsu1);
		putled();
		delayms(30);
	}
}

void anim_kamatsu2_f(void){
	// display and move freight train(2) forward (left to right)
	for(int16_t x=0;x<64+16*8;x++){
		clrvram();
		settrain_f(x,1,font_diesel);
		settrain_f(x,2,font_kamotsu2);
		settrain_f(x,3,font_kamotsu3);
		settrain_f(x,4,font_kamotsu3);
		settrain_f(x,5,font_kamotsu4);
		settrain_f(x,6,font_kamotsu5);
		settrain_f(x,7,font_kamotsu2);
		settrain_f(x,8,font_kamotsu1);
		putled();
		delayms(30);
	}
}

void anim_kamatsu2_b(void){
	// display and move freight train(2) backward (right to left)
	for(int16_t x=63;x>-16*8;x--){
		clrvram();
		settrain_b(x,1,font_diesel);
		settrain_b(x,2,font_kamotsu2);
		settrain_b(x,3,font_kamotsu3);
		settrain_b(x,4,font_kamotsu3);
		settrain_b(x,5,font_kamotsu4);
		settrain_b(x,6,font_kamotsu5);
		settrain_b(x,7,font_kamotsu2);
		settrain_b(x,8,font_kamotsu1);
		putled();
		delayms(30);
	}
}

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
	for(uint8_t i=0;i<8;i++){
		// Not Test Mode
		SPI1_Exchange8bit(0x0f);
		SPI1_Exchange8bit(0x00);
	}
	CS_H;
	CS_L;
	for(uint8_t i=0;i<8;i++){
		// Not Shutdown Mode
		SPI1_Exchange8bit(0x0c);
		SPI1_Exchange8bit(0x01);
	}
	CS_H;
	CS_L;
	for(uint8_t i=0;i<8;i++){
		// No Decode Mode
		SPI1_Exchange8bit(0x09);
		SPI1_Exchange8bit(0x00);
	}
	CS_H;
	CS_L;
	for(uint8_t i=0;i<8;i++){
		// Set Briteness
		SPI1_Exchange8bit(0x0a);
		SPI1_Exchange8bit(0x05);
	}
	CS_H;
	CS_L;
	for(uint8_t i=0;i<8;i++){
		// Scan All LEDs
		SPI1_Exchange8bit(0x0b);
		SPI1_Exchange8bit(0x07);
	}
	CS_H;
	clrvram();
	putled();

    while (1)
    {
		anim_sl_f();
		delayms(1000);

		anim_densha_b();
		delayms(1000);

		anim_shinkansen_b();
		delayms(1000);

		anim_kamatsu1_f();
		delayms(1000);

		anim_sl_b();
		delayms(1000);

		anim_kamatsu2_b();
		delayms(1000);

		anim_shinkansen_f();
		delayms(1000);

		anim_kamatsu1_b();
		delayms(1000);

		anim_densha_f();
		delayms(1000);

		anim_kamatsu2_f();
		delayms(1000);
	}
}
