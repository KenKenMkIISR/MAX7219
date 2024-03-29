REM SPI LED Matrix x8 (MAX7219 LED Driver)
REM 64x8 VRAM
REM Bouncing balls

USEVAR VRAM,D0,D1,D2,D3,D4,D5,D6,D7

SPI 10000,16,0:REM 10MHz/16bit/Mode0

REM Not Test Mode
D=$0F00:SPIWRITE D,D,D,D,D,D,D,D

REM Not Shutdown Mode
D=$0C01:SPIWRITE D,D,D,D,D,D,D,D

REM No Decode Mode
D=$0900:SPIWRITE D,D,D,D,D,D,D,D

REM Set Briteness
D=$0A05:SPIWRITE D,D,D,D,D,D,D,D

REM Scan All LEDs
D=$0B07:SPIWRITE D,D,D,D,D,D,D,D

DIM VRAM(2*8-1),P(1)
RESTORE FONT
FOR I=0 TO 1:P(I)=READ():NEXT

SYSTEM 200,0:REM Video OFF

G=40:REM Gravity
DO
 H=0
 DO
  V=0:X=64<<8
  DO
   GOSUB SETBMP,X>>8,P
   GOSUB PUTLED
   WAIT 1
   GOSUB CLRBMP,X>>8
   V=V-G
   X=X+V
   IF X<=H THEN
    X=H:V=-V*4/5
    IF V<100 THEN
     GOSUB SETBMP,H>>8,P
     H=H+$800
    ENDIF
   ENDIF
  LOOP WHILE X>=H
 LOOP WHILE H<$4000

 WAIT 60

 V=0
 DO
  J=X/256
  FOR I=1 TO 8:GOSUB SETBMP,J,P:J=J-8:NEXT
  GOSUB PUTLED
  WAIT 1
  V=V-G/3
  X=X+V
  GOSUB CLRVRM
 LOOP WHILE X>-8*256
 GOSUB PUTLED
 WAIT 30
LOOP
 
SYSTEM 200,1:REM Video ON
END

LABEL SETBMP
 REM Put 8x8 Character
 REM arg1:x
 REM arg2:bitmap address

 VAR I,J,D
 FOR I=0 TO 7
  D=PEEK(ARGS(2)+(I AND 4)+(3-(I AND 3)))
  FOR J=0 TO 7
   IF D AND $80 THEN
    GOSUB SETP,J+ARGS(1),I
   ELSE
    GOSUB CLRP,J+ARGS(1),I
   ENDIF
   D=D<<1
  NEXT
 NEXT
RETURN

LABEL CLRBMP
 REM Clear 8x8 Character
 REM arg1:x

 VAR I,J
 FOR I=0 TO 7
  FOR J=0 TO 7
   GOSUB CLRP,J+ARGS(1),I
  NEXT
 NEXT
RETURN

LABEL CLRVRM
 VAR I
 FOR I=0 TO 15:VRAM(I)=0:NEXT
RETURN

LABEL SETP
 REM Set point (arg1,arg2)
 VAR P
 IF ARGS(1)<0 OR ARGS(1)>=64 THEN RETURN
 IF ARGS(2)<0 OR ARGS(2)>=8 THEN RETURN
 P=(ARGS(2)<<1)+(ARGS(1)>>5)
 VRAM(P)=VRAM(P) OR (1<<(31-(ARGS(1) AND $1F)))
RETURN

LABEL CLRP
 REM Clear point (arg1,arg2)
 VAR P
 IF ARGS(1)<0 OR ARGS(1)>=64 THEN RETURN
 IF ARGS(2)<0 OR ARGS(2)>=8 THEN RETURN
 P=(ARGS(2)<<1)+(ARGS(1)>>5)
 VRAM(P)=VRAM(P) AND ((1<<(31-(ARGS(1) AND $1F))) XOR -1)
RETURN

LABEL PUTLED
 REM Output VRAM to LED
 VAR I,Y
 FOR I=0 TO 7
  Y=(I+1)<<8
  D0=Y+((VRAM(I*2  )>>24) AND $FF)
  D1=Y+((VRAM(I*2  )>>16) AND $FF)
  D2=Y+((VRAM(I*2  )>> 8) AND $FF)
  D3=Y+((VRAM(I*2  )    ) AND $FF)
  D4=Y+((VRAM(I*2+1)>>24) AND $FF)
  D5=Y+((VRAM(I*2+1)>>16) AND $FF)
  D6=Y+((VRAM(I*2+1)>> 8) AND $FF)
  D7=Y+((VRAM(I*2+1)    ) AND $FF)
  SPIWRITE D0,D1,D2,D3,D4,D5,D6,D7
 NEXT
RETURN

LABEL FONT
DATA $3C7EFFFF,$FFFF7E3C
