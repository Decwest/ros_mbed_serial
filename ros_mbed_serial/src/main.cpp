#include "mbedserial.h"

static BufferedSerial pc(USBTX, USBRX, 115200);
Mbedserial Ms(pc);
extern DigitalOut myled;

void CallBack_float();
void CallBack_int();
void CallBack_string();

int main()
{
  myled = 1;

  //受信コールバック関数の設定
  Ms.float_attach(CallBack_float);
  Ms.int_attach(CallBack_int);
  Ms.string_attach(CallBack_string);

  while (1)
  {
    Ms.read_data();
    myled = !myled;
    // int size = 2;            // 配列サイズを取得
    // float f[2] = {1.0, 3.0}; // データを取得
    // // 送信
    // Ms.float_write(f, size);
    // 受信
    // string word = "aa";
    // // 送信
    // Ms.string_write(word, word.size());
    // wait_us(10000);
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

void CallBack_string()
{
  // 受信
  int size = Ms.stringarraysize; // 配列サイズを取得
  std::string s = Ms.getstring;  // データを取得
  // 送信
  Ms.string_write(s, size);
}