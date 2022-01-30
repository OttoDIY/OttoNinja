/*
*  name:     贪吃蛇 
*  version:  3.6 <NO BUG Version>
*  Author:   Apple tec.<[email]godfights@163.com[/email]>
*  Date:     2015-10-23
*  Description:  BiColor IIC点阵
*  note:     &#169;2015 Apple tec.All Rights
*                  Designed by LJ
*/
/*==================================================================================  贪吃蛇  =================================================================================================================*/
#include <Wire.h>                                                       // I2C链接库
#include "Adafruit_LEDBackpack.h"                                       // 点阵驱动库
#include "Adafruit_GFX.h"                                               // Arduino图形库 

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();               // 设置I2C点阵型号 

static const uint8_t __attribute__ ((progmem)) X [] = { B01000010, B01000010, B00100100, B00011000, B00011000, B00100100, B01000010, B01000010 };   // 字母 X 的logo代码，其实就是一个一个点的量灭组成的
                                         
int speedS = 8;                                                         // 贪吃蛇的初始速度
int socre;                                                              // 游戏得分
int xValue;                                                             // JoyStick-X
int yValue;                                                             // JoyStick-Y
int zPin = 4;                                                           // JoyStick-Z
int Enabled;                                                            // 重启游戏使能
int FX, FY;                                                             // 食物的坐标
int SX, SY;                                                             // 蛇头的坐标
int KEY, K;                                                             // 当前按键码和实际按键码
char s[129] = {};                                                       // 蛇身体坐标集合，一共64个点，二维坐标128个元素 其中0没有用到 故为129个元素
/***************************************************************************************************************************************************************************************************************/

void setup() {

  /*
  pinMode(A2, OUTPUT);              // 让A2 A3输出  A2高电平 A3 低电平来达到VCC GND的目的  20ma的输出能力足够点阵使用了
  pinMode(A3, OUTPUT);              // 在此模式下需将显示方向设置为 3
  digitalWrite(A2, HIGH); 
  digitalWrite(A3, LOW);  
  */
  
  pinMode(zPin, INPUT);
  
  RANDOM();                         // 获取一个随机食物坐标
  K = 0;                            // 初始化按键码，实际上就是行进方向 方向初始化为0 即在首次操作摇杆前 蛇不移动
  s[1] = 2;                         // x3
  s[2] = 2;                         // y3
  s[3] = 1;                         // x2
  s[4] = 2;                         // y2
  s[5] = 0;                         // x1
  s[6] = 2;                         // y1 初始化蛇的身体坐标       
  socre = 0;                        // 游戏起始得分
  
  matrix.begin(0x70);               // 设定IIC屏幕地址
  matrix.clear();                   // 屏幕清屏
  matrix.writeDisplay();            // 屏幕显示
  matrix.setRotation(1);            // 设置屏幕显示方向  0向左显示 1向上显示 2向右显示 3向下显示 默认值为0 若输入错误值4 显示方向保持默认0
  matrix.clear(); 
  
  Logo();                           // 绘制开机Logo
  
  matrix.writeDisplay();
  delay(1000); 
  
  matrix.setTextWrap(false);               // 设置字体平滑模式
  matrix.setTextSize(1);                   // 设置字号
  matrix.setTextColor(LED_GREEN);          // 设定LED颜色，0表示熄灭~ 
  
  for (int8_t x = 7; x >= -70; x--) {      // for (int8_t x = -75; x <= 7; x++) {} 向右滚屏
    matrix.clear();
    matrix.setCursor(x, 1);                // setCursor(int16_t x, int16_t y)
    matrix.print( "READY!!! G0 " );
    matrix.writeDisplay();
    delay(70);
  }
  // 循环绘制文字，获得滚动效果 
  
  matrix.clear(); 
  for(int i = 1; i <= 6; i += 2)                      // 从蛇头开始绘制三个点 s[1], s[3], s[5]为x坐标 s[2], s[4], s[6]为y坐标 蛇身排序： {s[5],s[6]}  {s[3],s[4]}  {s[1],s[2]} （右侧为蛇头）
  {
    matrix.drawPixel(s[i], s[i + 1], LED_RED);      // drawPixel(int16_t x, int16_t y, uint16_t color)
  }
  matrix.writeDisplay();
  delay(400);
}
// 循环绘制蛇的身体，实际上就是3个点咯~ 
/***************************************************************************************************************************************************************************************************************/  

void RANDOM() {
A:   
  FX = random(0, 7); 
  FY = random(0, 7);
  
  for(int i = 1; i <= 6 + 2 * socre; i+= 2)            // 防止食物出现在蛇身上
  {
    if( FX == s[i] and FY == s[i + 1])
      goto A;                               
  }
}
// 随机食物获取函数 
/***************************************************************************************************************************************************************************************************************/  

void myDelay( int Time) {                             // 在蛇移动的延时期间 我们要做一些事情 比如食物的闪烁和摇杆方向的读取
   
  for(int t = 1; t <= Time; t++)
  { 
    matrix.drawPixel(FX, FY, LED_GREEN);
    matrix.writeDisplay();
    delay(50);
    matrix.drawPixel(FX, FY, 0);
    matrix.writeDisplay();
    delay(50);
    joyStick();                                        // 读取摇杆方向        
  }
}
// 这个是演示函数，实际上通过不断闪现食物来完成延迟，这样既可以做到精确延时，也可以让食物不断闪动，来让大家不断的看到~ 
/***************************************************************************************************************************************************************************************************************/ 

void Endelay( unsigned long duration) {                // 字幕滚动的延时期间 我们也要做一些事情 比如判断中央键是否被按下 按下则重置游戏
  
  unsigned long Start = millis();
  while(millis() - Start <= duration) {
    if(digitalRead(zPin) == LOW) {
      delay(20);
      if(digitalRead(zPin) == LOW) {
        Enabled = 1;
        }
      }
    }
}
// 重启游戏专用延时函数  
/***************************************************************************************************************************************************************************************************************/ 

void Logo() {
                                               
  matrix.drawPixel(6, 0, LED_YELLOW);          // X 右边笔画的动画
  matrix.writeDisplay();
  delay(35);
  for ( int x = 1; x <= 6; x++) {               
    matrix.drawPixel(7-x, x, LED_YELLOW); 
    matrix.writeDisplay();
    delay(35);
    }
  matrix.drawPixel(1, 7, LED_YELLOW); 
  matrix.writeDisplay();
  delay(35);

  matrix.drawPixel(1, 0, LED_YELLOW);          // X 左边笔画的动画
  matrix.writeDisplay();
  delay(35);
  for ( int x = 1; x <= 6; x++) {              
    matrix.drawPixel(x, x, LED_YELLOW); 
    matrix.writeDisplay();
    delay(35);
    }
  matrix.drawPixel(6, 7, LED_YELLOW); 
  matrix.writeDisplay();
  delay(35);

  for (int x = 1; x <= 45; x++) {                         // logo闪烁
    matrix.clear(); 
    matrix.writeDisplay();
    delay(x); 
    matrix.drawBitmap(0, 0, X, 8, 8, LED_YELLOW);       // drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
    matrix.writeDisplay();
    delay(x);
    }
}
// Logo绘制函数，看不懂可以跳过~~~不过还是蛮简单的，仔细看下就明白了~
/***************************************************************************************************************************************************************************************************************/

void joyStick() {
  
  xValue = analogRead(A0);                             // JoyStick-X   最左值为0 最右值为1023 中间值为515
  yValue = analogRead(A1);                             // JoyStick-Y   最下值为0 最上值为1023 中间值为510
    
  if(yValue == 1023 && KEY != 5) {
    K = 8;
    } 
  else if(yValue == 0 && KEY != 8) {
    K = 5;
    }
  else if(xValue == 0 && KEY != 6 && K != 0) {         // 首次操作摇杆前 不能向左移动蛇
    K = 4;
    } 
  else if(xValue == 1023 && KEY != 4) {
    K = 6;
    }
    
  if(digitalRead(zPin) == LOW)                         // 当按下中央键后 速度变为 2  松开后 速度恢复到当前值
    speedS = 2;
  else  
    speedS = 8 - socre / 5; 
}
// 这里是在比对按键方向是否和目前蛇的行进方向相反，如果相反就放弃按键  
/***************************************************************************************************************************************************************************************************************/ 

void gameover() {                                      // 一旦执行到Gameover 程序停止 需要按下中央键后以重启游戏
  
  Enabled = 0;
  while(Enabled == 0) {
    
    for (int8_t x = 7; x >= -125 && Enabled == 0; x--) {
      matrix.clear();
      matrix.setCursor(x, 1);
      matrix.print( "GAME OVER YOUR SOCRE:" );
      matrix.writeDisplay();
      Endelay(70);
      }
    
    for (int8_t x = 7; x >= -39 && Enabled == 0; x--) {
      matrix.clear();
      matrix.setCursor(x, 1);
      matrix.print(socre);
      matrix.writeDisplay();
      Endelay(70);
      } 
    }
    // GameOver函数，显示游戏结束和得分 
    
    for(int i = 0; i < 129; i++) {
      s[i] = 0; 
      }
    
    K = 0;                  // 重置摇杆方向
    s[1] = 2;
    s[2] = 2;
    s[3] = 1;
    s[4] = 2;
    s[5] = 0;
    s[6] = 2;               // 重置蛇的坐标
    socre = 0;              // 重置游戏得分
}
/***************************************************************************************************************************************************************************************************************/  

void loop() { 

  KEY = K;                                      // 蛇每移动一次 方向才能改变一次
  matrix.clear(); 
  
  if(KEY == 8)                                  // 蛇向上运动
  { 
    for(int i = 6 + 2 * socre; i > 3; i= i - 2)
    {
      s[i] = s[i - 2];
      s[i - 1] = s[i - 3];
    }
    s[2] = s[2] - 1;
    if(s[2] < 0)                                // 超出边框的从另一边继续出现
      s[2] = 7;
  }
  else if(KEY == 5)                             // 蛇向下运动
  {
    for(int i = 6 + 2 * socre; i > 3; i= i - 2)
    {
      s[i] = s[i - 2];
      s[i - 1] = s[i - 3];
    }
    s[2] = s[2] + 1;
    if(s[2] > 7)                                // 超出边框的从另一边继续出现
      s[2] = 0;
  }
  else if(KEY == 4)                             // 蛇向左运动
  {
    for(int i = 6 + 2 * socre; i > 3; i= i - 2)
    {
      s[i] = s[i - 2];
      s[i - 1] = s[i - 3];
    }
    s[1] = s[1] - 1;
    if(s[1] < 0)                                // 超出边框的从另一边继续出现
      s[1] = 7;
  }
  else if(KEY == 6)                             // 蛇向右运动
  {
    for(int i = 6 + 2 * socre; i > 3; i= i - 2)
    {
      s[i] = s[i - 2];
      s[i - 1] = s[i - 3];
    }
    s[1] = s[1] + 1;
    if(s[1] > 7)                                // 超出边框的从另一边继续出现
      s[1] = 0;
  }
  // 根据按键来移动蛇的身体
  
  for(int i = 1; i <= 6 + 2 * socre; i+= 2)     // 从蛇头开始绘制
  {
    matrix.drawPixel(s[i], s[i + 1], LED_RED);
  }
  matrix.writeDisplay();
  
  myDelay(speedS);                                //（)内为延时次数 延时一次为100ms  实测200ms速度很合适 故将加速时速度设置为 2  
  
  SX = s[1];
  SY = s[2]; 
  // 获取蛇头的坐标
  
  for(int i = 3; i <= 6 + 2 * socre; i+= 2)
  {
    if( SX == s[i] && SY == s[i + 1])
      gameover();                               
  }
  // 判断蛇头是否碰到自己的身体，碰到就跳转到GameOver
  
  if(SY == FY && SX == FX)
  {
    socre ++;
    RANDOM();
    if( !(socre % 5)) {                       // 根据得分加快蛇的速度 每吃5个食物 速度快100ms
      speedS --; 
      if(speedS < 2)                          // 速度下限为200ms 如果速度小于200ms 速度值仍为200ms
        speedS = 2;
      }
  }
  // 判断是否吃到了食物~吃到了就会加分和重新随机生成食物~
}
