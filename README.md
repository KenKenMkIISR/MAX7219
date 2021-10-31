# Raspberry Pi PicoおよびMachiKania type Mによる8x8x8マトリクスLED制御
8連結されたMAX7219を使ったマトリクスLEDをRaspberry Pi PicoおよびBASICコンパイラ搭載マイコンMachiKania type MのSPI通信で制御しました。  
拡張子pyはRaspberry Pi Pico用のPythonプログラム、BASはMachiKania type M用のBASICプログラムです。  
・文字列を横スクロール表示（max7219-string.py / ）  
・PACMANとモンスターのアニメーション（max7219-pacman.py）  
・落ちてきた複数のボールが跳ねるアニメーション（max7219-bounce.py）  
・金魚と水草のアニメーション（max7219-goldenfish.py）  
![](ledmatrix1.jpg)  
  
# 接続
Pico側 --------- LED側  
MOSI GPIO3 ---- DIN  
SCK GPIO2 ------ CLK  
SCS GPIO22 ----- CS  
GND ------------ GND  
+5V ------------- VCC (USBでは無理があるので別電源が必要)  
  
MachiKania type M  
MOSI G9 -------- DIN  
SCK F6 ---------- CLK  
SCS D9 ---------- CS  
GND ------------ GND  
+5V ------------- VCC  
