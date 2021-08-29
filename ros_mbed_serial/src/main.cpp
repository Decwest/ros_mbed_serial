#include "mbedserial.h"

static BufferedSerial pc(USBTX, USBRX, 115200);
Mbedserial Ms(pc);

DigitalOut myled(LED1);

void CallBack_float();
void CallBack_int();
void CallBack_char();

int main()
{
  myled = 1;

  //受信コールバック関数の設定
  Ms.float_attach(CallBack_float);
  Ms.int_attach(CallBack_int);
  Ms.char_attach(CallBack_char);

  while (1)
  {
    Ms.read_data();
    myled = !myled;
    // int size = 2;            // 配列サイズを取得
    // int f[2] = {1, 3}; // データを取得
    // // 送信
    // Ms.int_write(f, size);
    // 受信
    // string word = "abc";
    // // 送信
    // Ms.char_write(word.c_str(), word.size());
    wait_us(10000);
  }
}

// コールバック関数の定義
void CallBack_float()
{
  // 受信
  int size = Ms.floatarraysize; // 配列サイズを取得
  float *f = Ms.getfloat;       // データを取得
  // 送信
  Ms.float_write(f, size);
}

void CallBack_int()
{
  // 受信
  int size = Ms.intarraysize; // 配列サイズを取得
  int *i = Ms.getint;         // データを取得
  // 送信
  Ms.int_write(i, size);
}

void CallBack_char()
{
  // 受信
  int size = Ms.chararraysize; // 配列サイズを取得
  char *c = Ms.getchar;        // データを取得
  // 送信
  Ms.char_write(c, size);
}