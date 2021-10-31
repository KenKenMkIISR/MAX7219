# MAX7219 x8 LED Maxtrix for Raspberry Pi Pico
#  PACMAN animation

# Connection
#  MOSI GPIO3  - DIN
#  SCK  GPIO2  - CLK
#  SCS  GPIO22 - CS
#  +5V         - VCC
#  GND         - GND

# 8x8 dot characters font data
Font=[ \
    0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,\
    0x3C,0x7E,0x3F,0x1F,0x1F,0x3F,0x7E,0x3C,\
    0x1C,0x0E,0x07,0x07,0x07,0x07,0x0E,0x1C,\
    0x3C,0x7E,0x3F,0x1F,0x1F,0x3F,0x7E,0x3C,\
    0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,\
    0x3C,0x7E,0xFC,0xF8,0xF8,0xFC,0x7E,0x3C,\
    0x38,0x70,0xE0,0xE0,0xE0,0xE0,0x70,0x38,\
    0x3C,0x7E,0xFC,0xF8,0xF8,0xFC,0x7E,0x3C,\
    0x3C,0x7E,0xFF,0x93,0x93,0xFF,0xFF,0xAB,\
    0x3C,0x7E,0xDB,0xFF,0x99,0x66,0xFF,0xA5,\
    0x07,0x1A,0x74,0xEE,0xDF,0xDF,0x7F,0x0E,\
    0x3C,0x7E,0x7A,0x5E,0x7E,0x34,0x3C,0x18,\
    0x00,0x7E,0xFF,0xBF,0xFD,0xEF,0x7E,0x3C,\
    0x08,0x66,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,\
    0x1C,0x08,0x3C,0x7E,0x7E,0x7E,0x7E,0x3C,\
    0x49,0x5D,0x7F,0x7F,0x3E,0x2A,0x08,0x08,\
    0x18,0x3C,0x6E,0x5E,0x5E,0x5E,0xDF,0xFF,\
    0x3E,0x26,0x1C,0x1C,0x18,0x1C,0x08,0x08]

# output 64bit data to MAX7219, line y(1-8)
def putled64(y,d):
    cs.value(0)
    for x in range(7,-1,-1):
        spi.write(bytearray([y,d>>(x*8)]))
    cs.value(1)

# output VRAM data to MAX7219 x8 display
def putled():
    for y in range(8):
        putled64(y+1,vram[y])

# set number p's font data(8x8) on position x
def setbmp(x,p):
    p=p*8
    for i in range(8):
        c=Font[p]
        for j in range(8):
            if c & 0x80:
                setp(x+j,i)
            else:
                clrp(x+j,i)
            c=c<<1
        p=p+1

# clear 8x8 area on position x
def clrbmp(x):
    for i in range(8):
        for j in range(8):
            clrp(x+j,i)

# set point (x,y)
def setp(x,y):
    if x>=0 and x<64 and y>=0 and y<8:
        vram[y]=vram[y] | (1<<(63-x))

# clear point (x,y)
def clrp(x,y):
    if x>=0 and x<64 and y>=0 and y<8:
        vram[y]=vram[y] & ((1<<63-x) ^ 0xffffffffffffffff)

from machine import Pin, SPI
import time

cs = Pin(22, Pin.OUT)
spi = SPI(0, baudrate=10000000, bits=8, polarity=0, phase=0, mosi=Pin(3), sck=Pin(2))

# Initialize MAX7219 x8
putled64(0x0c,0x0101010101010101) # Not Shutdown Mode
putled64(0x09,0x0000000000000000) # No Decode Mode
putled64(0x0a,0x0505050505050505) # Set Brighteness
putled64(0x0b,0x0707070707070707) # Scan All LEDs

vram=[0,0,0,0,0,0,0,0]
pacman=0
fruits=0
cookie=0
while True:
    for x in range(1,8):
        setp(x*8+3,4)    #display cookies
    setbmp(16,10+fruits) #display fruits
    for x in range(63,-1,-1):
        setbmp(x,pacman) #display pacman
        setbmp(x+12,8)   #display monsters
        setbmp(x+22,8)
        setbmp(x+32,8)
        setbmp(x+42,8)
        if cookie<4:
            setp(3,3)    #blink power cookie
            setp(4,3)
            setp(3,4)
            setp(4,4)
        else:
            clrp(3,3)
            clrp(4,3)
            clrp(3,4)
            clrp(4,4)
        putled()         #output VRAM data to MAX7219 LED display
        time.sleep_ms(1)
        # clear all items
        clrbmp(x)
        clrbmp(x+12)
        clrbmp(x+22)
        clrbmp(x+32)
        clrbmp(x+42)
        pacman=(pacman+1)%4
        cookie=(cookie+1)%8

    for x in range(0,65):
        setbmp(x,4+pacman)
        setbmp(x+12,9)   #display ijike monsters
        setbmp(x+22,9)
        setbmp(x+32,9)
        setbmp(x+42,9)
        putled()
        time.sleep_ms(1)
        clrbmp(x)
        clrbmp(x+12)
        clrbmp(x+22)
        clrbmp(x+32)
        clrbmp(x+42)
        pacman=(pacman+1)%4
    fruits=(fruits+1)%8
