# MAX7219 x8 LED Maxtrix for Raspberry Pi Pico
#  Bouncing balls animation

# Connection
#  MOSI GPIO3  - DIN
#  SCK  GPIO2  - CLK
#  SCS  GPIO22 - CS
#  +5V         - VCC
#  GND         - GND

# 8x8 dot characters font data
Font=[0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C]

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

# clear all VRAM
def clrvram():
    for i in range(8):
        vram[i]=0

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
g=40 #gravity
while True:
    h=0
    while h<64*256:
        v=0
        x=64*256
        while x>=h:
            setbmp(x>>8,0)
            putled()         #output VRAM data to MAX7219 LED display
            time.sleep_ms(3)
            clrbmp(x>>8)
            v=v-g
            x=x+v
            if x<=h:
                x=h
                v=-v*4//5
                if v<100:
                    setbmp(h>>8,0)
                    h=h+8*256
    time.sleep_ms(1000)
    v=0
    while x>-15*256:
        j=x//256
        for i in range(1,9):
            setbmp(j,0)
            j=j-8
        putled()         #output VRAM data to MAX7219 LED display
        time.sleep_ms(1)
        v=v-g
        x=x+v
        clrvram()
    time.sleep_ms(500)
