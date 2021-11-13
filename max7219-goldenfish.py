# MAX7219 x8 LED Maxtrix for Raspberry Pi Pico
#  Golden fish animation

# Connection
#  MOSI GPIO3  - DIN
#  SCK  GPIO2  - CLK
#  SCS  GPIO22 - CS
#  +5V         - VCC
#  GND         - GND

# Golden Fish 16x8 x3 patterns
# Golden Fish(mirror) 16x8 x3 patterns
# Plants 7x8 x3 patterns
BMP=[\
    0x1E060000,0x3F1E0000,0x7FBE0000,0xDFF00000,\
    0xFFE00000,0x7FFE0000,0x3F9F0000,0x0EC00000,\
    0x1E020000,0x3F1F0000,0x7FBE0000,0xDFFC0000,\
    0xFFE00000,0x7FF00000,0x3FDE0000,0x0E670000,\
    0x1E000000,0x3F0F0000,0x7F9F0000,0xDFFC0000,\
    0x7FE00000,0xFFF80000,0x3FDF0000,0x0E670000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x00000000,0x00000000,0x00000000,0x00000000,\
    0x88000000,0x84000000,0x54000000,0x54000000,\
    0x68000000,0x50000000,0x20000000,0x20000000,\
    0x54000000,0x54000000,0x4C000000,0x58000000,\
    0x70000000,0x50000000,0x20000000,0x20000000,\
    0x92000000,0x54000000,0x6C000000,0x28000000,\
    0x70000000,0x50000000,0x20000000,0x20000000,\
    0x12000000,0xBF400000,0x7F800000,0xBF400000,\
    0X7F800000,0xA1400000,0x00000000,0x00000000,\
    0X92400000,0x7F800000,0xBF400000,0x7F800000,\
    0XBF400000,0x52800000,0x00000000,0x00000000
]

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

# set bitmap number p (size:m,n) on (x,y)
def setbmpmn(x,y,m,n,p):
    for i in range(n):
        c=BMP[p*8+i]
        b=0x80000000
        for j in range(m):
            if c & b:
                setp(x+j,y+i)
            else:
                clrp(x+j,y+i)
            b=b>>1

# clear area (size:m,n) on (x,y)
def clrbmpmn(x,y,m,n):
    for i in range(n):
        for j in range(m):
            clrp(x+j,y+i)

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
putled64(0x0a,0x0f0f0f0f0f0f0f0f) # Set Brighteness
putled64(0x0b,0x0707070707070707) # Scan All LEDs

vram=[0,0,0,0,0,0,0,0]

# Create mirror data
for i in range(24):
    d=0
    for j in range(16):
        d=(d<<1) | ((BMP[i]>>(16+j)) & 1)
    BMP[24+i]=d<<16

bubble_y=-1
fish_y=0
fish_vy=1
pattern=0
crab_x=0
crab_vx=1

# Main loop
while True:
    for fish_x in range(70,-21,-1):
        setbmpmn( 5,0,7,8,6+pattern)         #display plants 1
        setbmpmn(23,0,7,8,6+(pattern+1)%3)   #display plants 2
        setbmpmn(42,0,7,8,6+(pattern+2)%3)   #display plants 3
        setbmpmn(crab_x,2,10,6,9+(crab_x & 1)) #display crab
        setbmpmn(fish_x,fish_y,16,8,pattern) #display golden fish
        if bubble_y>=0:
            setp(bubble_x,bubble_y)          #display bubble
        putled()         #output VRAM data to MAX7219 LED display
        time.sleep_ms(70)
        clrbmpmn(fish_x,fish_y,16,8)         #clear golden fish
        clrbmpmn(crab_x,2,10,6)              #clear crab
        if bubble_y>=0:
            clrp(bubble_x,bubble_y)          #clear bubble
            bubble_y=bubble_y-1              #move bubble
        if fish_x & 1:
            pattern=(pattern+1)%3            #next bitmap pattern
        if (fish_x & 3)==0:
            fish_y=fish_y+fish_vy            #golden fish up/down
            if fish_y<-1:
                fish_vy=1
            elif fish_y>1:
                fish_vy=-1
                bubble_x=fish_x-8            #bubble appear
                bubble_y=7
        if (fish_x & 7)==2:
            crab_x=crab_x+crab_vx            #move crab
            if crab_x==0:
                crab_vx=1
            elif crab_x==54:
                crab_vx=-1
    for fish_x in range(-20,71):
        setbmpmn( 5,0,7,8,6+pattern)         #display plants 1
        setbmpmn(23,0,7,8,6+(pattern+1)%3)   #display plants 2
        setbmpmn(42,0,7,8,6+(pattern+2)%3)   #display plants 3
        setbmpmn(crab_x,2,10,6,9+(crab_x & 1)) #display crab
        setbmpmn(fish_x,fish_y,16,8,3+pattern) #display golden fish(mirror)
        if bubble_y>=0:
            setp(bubble_x,bubble_y)          #display bubble
        putled()         #output VRAM data to MAX7219 LED display
        time.sleep_ms(70)
        clrbmpmn(fish_x,fish_y,16,8)         #clear golden fish
        clrbmpmn(crab_x,2,10,6)              #clear crab
        if bubble_y>=0:
            clrp(bubble_x,bubble_y)          #clear bubble
            bubble_y=bubble_y-1              #move bubble
        if fish_x & 1:
            pattern=(pattern+1)%3            #next bitmap pattern
        if (fish_x & 3)==0:
            fish_y=fish_y+fish_vy            #golden fish up/down
            if fish_y<-1:
                fish_vy=1
            elif fish_y>1:
                fish_vy=-1
                bubble_x=fish_x+24           #bubble appear
                bubble_y=7
        if (fish_x & 7)==2:
            crab_x=crab_x+crab_vx            #move crab
            if crab_x==0:
                crab_vx=1
            elif crab_x==54:
                crab_vx=-1
