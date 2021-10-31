# 8連結MAX7219マトリクスLEDのマイコン制御（64x8ドット）
Amazon等で格安で購入できる4連結のMAX7219搭載マトリクスLEDを2個購入し、マイコンで制御してみました。  
制御に使ったのはRaspberry Pi PicoおよびBASICコンパイラ搭載マイコンMachiKania type Mです。  
![](ledmatrix1.jpg)  
## 加工
余分なコネクタは外し、ボード同士を接続します。  
基板固定のため、アクリル板を細長く切断し、ねじ止めしました。  
また、LEDを直接見ると眩しいので、白いアクリル板やフィルム等を上から被せられるようにしました。  
![](ledmatrix3.jpg)  
![](ledmatrix4.jpg)  
## 公開プログラム
拡張子「py」はRaspberry Pi Pico用のPythonプログラム、「BAS」はMachiKania type M用のBASICプログラムです。  
  
・max7219-string.py / SPILED6.BAS  
　文字列を横スクロールして表示するプログラム  
  
・max7219-pacman.py / SPILED7.BAS  
　PACMANとモンスターが動くアニメーション  
  
・max7219-bounce.py / SPILED8.BAS  
　縦置きして、複数のボールが落ちてきて跳ねるアニメーション  
  
・max7219-goldenfish.py / SPILED9.BAS  
　水草の揺れる水槽を金魚が泳ぎ回るアニメーション  
![](ledmatrix5.jpg)  
![](ledmatrix6.jpg)  
  
## 接続方法
LED用の電源はUSBからではなく、別電源を用意してください。  
  
Pico側 --------- LED側  
MOSI GPIO3 ---- DIN  
SCK GPIO2 ------ CLK  
SCS GPIO22 ----- CS  
GND ------------ GND  
+5V ------------- VCC  
  
MachiKania type M  
MOSI G9 -------- DIN  
SCK F6 ---------- CLK  
SCS D9 ---------- CS  
GND ------------ GND  
+5V ------------- VCC  
