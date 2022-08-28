REM SPI LED Matrix x8 (MAX7219 LED Driver)
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

S$="MachiKania "
WHILE 1
 FOR I=0 TO LEN(S$)*8-1
  GOSUB PUTLED,S$,I:WAIT 2
 NEXT
WEND
LABEL PUTLED
 VAR I,P,F,D,L
 F=SYSTEM(103):REM Font Address
 L=LEN(ARGS$(1)):P=ARGS(2)/8
 FOR I=1 TO 8
  D=PEEK(F+PEEK(ARGS(1)+P)*8)<<16
  D=D+(PEEK(F+PEEK(ARGS(1)+(P+1)%L)*8)<<8)
  D=D+PEEK(F+PEEK(ARGS(1)+(P+2)%L)*8)
  D=D>>(8-ARGS(2)%8)

  E=PEEK(F+PEEK(ARGS(1)+(P+2)%L)*8)<<16
  E=E+(PEEK(F+PEEK(ARGS(1)+(P+3)%L)*8)<<8)
  E=E+PEEK(F+PEEK(ARGS(1)+(P+4)%L)*8)
  E=E>>(8-ARGS(2)%8)

  G=PEEK(F+PEEK(ARGS(1)+(P+4)%L)*8)<<16
  G=G+(PEEK(F+PEEK(ARGS(1)+(P+5)%L)*8)<<8)
  G=G+PEEK(F+PEEK(ARGS(1)+(P+6)%L)*8)
  G=G>>(8-ARGS(2)%8)

  H=PEEK(F+PEEK(ARGS(1)+(P+6)%L)*8)<<16
  H=H+(PEEK(F+PEEK(ARGS(1)+(P+7)%L)*8)<<8)
  H=H+PEEK(F+PEEK(ARGS(1)+(P+8)%L)*8)
  H=H>>(8-ARGS(2)%8)

  SPIWRITE (I<<8)+((D>>8) AND $FF),(I<<8)+(D AND $FF),(I<<8)+((E>>8) AND $FF),(I<<8)+(E AND $FF),(I<<8)+((G>>8) AND $FF),(I<<8)+(G AND $FF),(I<<8)+((H>>8) AND $FF),(I<<8)+(H AND $FF)
  F=F+1
 NEXT
RETURN
