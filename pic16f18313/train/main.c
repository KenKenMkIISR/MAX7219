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
	0xA4,0x4A,
	0xA4,0x4A,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_romendensha[]={
	// romen_densha
	0x18,0x30,
	0xFF,0xFE,
	0x49,0x24,
	0x49,0x24,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_tokkyu_h[]={
	// tokkyu(head)
	0x18,0x30,
	0x3F,0xFE,
	0x52,0x22,
	0xD2,0x22,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_tokkyu_m[]={
	// tokkyu(middle)
	0x18,0x30,
	0xFF,0xFE,
	0x91,0x12,
	0x91,0x12,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_tokkyu_t[]={
	// tokkyu(tail)
	0x18,0x30,
	0xFF,0xF8,
	0x88,0x94,
	0x88,0x96,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_shinkansen_h[]={
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
const uint8_t font_shinkansen_m[]={
	// shinkansen(middle)
	0x00,0x70,
	0xFF,0xFE,
	0xFF,0xFE,
	0xAA,0xAA,
	0xFF,0xFE,
	0xFF,0xFE,
	0xFF,0xFE,
	0x28,0x28
};
const uint8_t font_shinkansen_t[]={
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
const uint8_t font_shinkansen2_h[]={
	// shinkansen2(head)
	0x00,0x04,
	0x00,0xFE,
	0x01,0x3E,
	0x0F,0xEA,
	0x7F,0xFE,
	0xFF,0xFE,
	0x7F,0xFE,
	0x14,0x14
};
const uint8_t font_shinkansen2_t[]={
	// shinkansen2(tail)
	0x40,0x00,
	0xFE,0x00,
	0xF9,0x00,
	0xAF,0xE0,
	0xFF,0xFC,
	0xFF,0xFE,
	0xFF,0xFC,
	0x50,0x50
};
const uint8_t font_sl[]={
	// steam locomotive
	0xF0,0x00,
	0x63,0xFE,
	0x63,0x18,
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
	0x01,0xF8,
	0x01,0x18,
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
const uint8_t font_kamotsu6[]={
	// stone car3
	0x00,0x00,
	0x00,0x00,
	0x00,0x00,
	0x40,0x04,
	0x7F,0xFC,
	0x7F,0xFC,
	0xFF,0xFE,
	0x24,0x48
};

const uint8_t * const ar_densha[]={
	font_densha,
	font_densha,
	font_densha,
	font_densha,
	font_densha,
	font_densha
};

const uint8_t * const ar_densha2[]={
	font_densha,
	font_densha,
	font_densha
};

const uint8_t * const ar_romendensha[]={
	font_romendensha
};

const uint8_t * const ar_tokkyu[]={
	font_tokkyu_h,
	font_tokkyu_m,
	font_tokkyu_m,
	font_tokkyu_m,
	font_tokkyu_m,
	font_tokkyu_t
};

const uint8_t * const ar_shinkansen1[]={
	font_shinkansen_h,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_t
};

const uint8_t * const ar_shinkansen2[]={
	font_shinkansen2_h,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen2_t
};

const uint8_t * const ar_shinkansen3[]={
	font_shinkansen2_h,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen2_t,
	font_shinkansen2_h,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen_m,
	font_shinkansen2_t
};

const uint8_t * const ar_sl[]={
	font_sl,
	font_tansuisha,
	font_kyakusha,
	font_kyakusha
};

uint8_t vram[64];
int8_t dir; // Train direction 1:left to right  -1:right to left

#define MAXTRAINS 16
uint8_t * trains[MAXTRAINS];

volatile uint16_t delaycounter=0;
void one_ms_interrupt(void){
	delaycounter++;
}
void delayms(uint16_t t){
	while(delaycounter<t)
		asm("SLEEP"); // Go to Idle mode
	delaycounter=0;
}

uint8_t rand;
uint8_t random()
{
	// r=r*5+1;
	rand=rand+rand+rand+rand+rand;
	rand++;
	return rand;
}

void clrvram(void){
	for(uint8_t *vramp=vram;vramp<vram+64;vramp++) *vramp=0;
}

void setp(uint8_t x,uint8_t y,uint8_t b){
	if(x>=64) return;
	if(y>=8) return;
	uint8_t *vramp=vram+(y<<3)+(x>>3);
	uint8_t d=(1<<(7-(x & 7)));
	if(b) *vramp=*vramp | d;
	else *vramp=*vramp & ~d;
}

void setbmp(int8_t x,const uint8_t *bmp){
// draw bitmap at vram x posiiton
	uint8_t d;
	int8_t x1;
	for(uint8_t j=0;j<8;j++){
		x1=x;
		for(uint8_t i=0;i<16;i++){
			if((i & 7)==0) d=*bmp++;
			setp(x1,j,d & 0x80);
			d<<=1;
			if(dir<0) x1++;
			else x1--;
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

void settrain(int16_t x,uint8_t n,const uint8_t *bmp){
	// set train bitmap with x position check
	// x: head x position
	// n: train number (1...)
	if(dir>0){
		if(x>=(n<<4)+63) return;
		n--;
		x-=n<<4;
		if(x<0) return;
	}
	else{
		if(x+(n<<4)<0) return;
		n--;
		x+=n<<4;
		if(x>=64) return;
	}
	setbmp(x,bmp);
}

void anim_train(uint8_t n,const uint8_t *ar_train[],uint8_t speed){
	// display and move train
	// n:number of trains
	// ar_train:train font array
	int16_t x,i;
	if(dir>0) x=0;
	else x=63;
	for(i=0;i<64+n*16;i++){
		clrvram();
		for(uint8_t j=1;j<=n;j++){
			settrain(x,j,ar_train[j-1]);
		}
		putled();
		delayms(speed);
		x+=dir;
	}
}

uint16_t initword_max7219[]={
	0x0f00, // Not Test Mode
	0x0c01, // Not Shutdown Mode
	0x0900, // No Decode Mode
	0x0a05, // Set Briteness
	0x0b07  // Scan All LEDs
};

void anim_sl(){
	anim_train(sizeof ar_sl/sizeof ar_sl[0] ,ar_sl,50);
}

void anim_densha(){
	if(random() & 1)
		anim_train(sizeof ar_densha/sizeof ar_densha[0],ar_densha,25);
	else
		anim_train(sizeof ar_densha2/sizeof ar_densha2[0],ar_densha2,30);
}

void anim_romendensha(){
	anim_train(sizeof ar_romendensha/sizeof ar_romendensha[0] ,ar_romendensha,40);
}

void anim_tokkyu(){
	anim_train(sizeof ar_tokkyu/sizeof ar_tokkyu[0],ar_tokkyu,20);
}

void anim_shinkansen(){
	uint8_t r=random();
	if(r<100)
		anim_train(sizeof ar_shinkansen1/sizeof ar_shinkansen1[0],ar_shinkansen1,15);
	else if(r<200)
		anim_train(sizeof ar_shinkansen2/sizeof ar_shinkansen2[0],ar_shinkansen2,15);
	else
		anim_train(sizeof ar_shinkansen3/sizeof ar_shinkansen3[0],ar_shinkansen3,15);
}

void anim_kamotsu(){
	uint8_t n,len;
	if(random() & 0x80) trains[0]=font_denkikikansha;
	else trains[0]=font_diesel;
	len=random() & (MAXTRAINS-1);
	len++;
	if(len<4) len+=7;
	for(n=1;n<len;n++){
		switch(random() & 7){
			case 0:
				trains[n]=font_kamotsu3;
				break;
			case 1:
				trains[n]=font_kamotsu4;
				break;
			case 2:
				trains[n]=font_kamotsu5;
				break;
			case 3:
				trains[n]=font_kamotsu6;
				break;
			case 4:
			case 5:
				trains[n]=font_kamotsu1;
				break;
			default:
				trains[n]=font_kamotsu2;
				break;
		}
	}
	anim_train(len,trains,30);
}

void anim_kamotsu2(){
	uint8_t n,r;
	if(random() & 1) trains[0]=font_denkikikansha;
	else trains[0]=font_diesel;
	for(n=1;n<MAXTRAINS;n++){
		r=random();
		if(r<180) trains[n]=font_kamotsu3;
		else if(r<205) trains[n]=font_kamotsu4;
		else if(r<230) trains[n]=font_kamotsu5;
		else trains[n]=font_kamotsu6;
	}
	anim_train(MAXTRAINS,trains,30);
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
	for(uint8_t j=0;j<5;j++){
		CS_L;
		for(uint8_t i=0;i<8;i++){
			SPI1_Exchange8bit((uint8_t)(initword_max7219[j]>>8));
			SPI1_Exchange8bit((uint8_t)(initword_max7219[j]));
		}
		CS_H;
	}
	clrvram();
	putled();
	rand=ADRESL;

    while (1)
    {
		uint8_t r=random();
		if(r & 1) dir=1;
		else dir=-1;
		if(r<60) anim_densha();
		else if(r<70) anim_romendensha();
		else if(r<120) anim_tokkyu();
		else if(r<180) anim_shinkansen();
		else if(r<210) anim_sl();
		else if(r<240) anim_kamotsu();
		else anim_kamotsu2();
		delayms(1000+((uint16_t)random()<<3));
		while(ADRESL & 3){
			random();
			delayms(1);
		}
	}
}
