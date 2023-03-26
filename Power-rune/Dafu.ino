/******************头文件定义************************/
#include <FastLED.h> //加载灯带头文件
/******************头文件定义************************/
#include <Arduino.h>
/******************宏定义***********************/

#define NUM_LEDS1 8 * 32 // 灯珠数量
#define NUM_LEDS2 52
#define NUM_LEDS3 83
#define NUM_LEDS NUM_LEDS2 + NUM_LEDS3
#define LED_TYPE WS2812 // 灯带型号
#define COLOR_ORDER GRB // 输入颜色的顺序 根据手册决定
CRGB leds[NUM_LEDS1];   // 构建灯珠对象
CRGB line[NUM_LEDS];

void Arrowhead(void);

/******************宏定义***********************/

/****************外部电路引脚定义**********************/

#define DATA_PIN1 5  // 灯板引脚
#define DATA_PIN2 13 // 灯条引脚

/****************外部电路引脚定义**********************/

/*********************变量数组等定义************************/

uint8_t max_bright = 200; // 亮度
uint8_t n = 0;            // LED灯的首地址
uint8_t taskflag = 0;
TaskHandle_t handleLEDCtrl = NULL;
/*********************变量数组等定义************************/

/*****************函数数组定义*********************/

void fillLED(void)
{
  uint8_t ar[32][8] = {
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 0, 0, 0, 0, 1, 1},
      {1, 1, 1, 0, 0, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},

      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},

      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},

      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {0, 1, 1, 1, 1, 1, 1, 0},
      {0, 0, 1, 1, 1, 1, 0, 0},
      {0, 0, 0, 1, 1, 0, 0, 0}};
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      leds[i * 8 + j] = CHSV(0, 255, 200 * ar[i][j]);
    }
  }
  FastLED.show();
  vTaskDelay(49);
}

void Arrowhead(void)
{
  uint8_t ar[8][8] = {
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1}};
  /*uint8_t ar[8][8] = {
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 0, 0, 0, 0, 1, 1},
      {1, 1, 1, 0, 0, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {0, 1, 1, 1, 1, 1, 1, 0},
      {0, 0, 1, 1, 1, 1, 0, 0},
      {0, 0, 0, 1, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0}};*/
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        leds[k * 64 + i * 8 + j] = CHSV(0, 255, 200 * ar[(i + n) % 8][j]);
      }
    }
  }
  n = (n + 7) % 8;
  FastLED.show();
  vTaskDelay(49);
}

void WS2812Init(void)
{
  delay(1000);                                                                                       // 上电延迟
  FastLED.addLeds<LED_TYPE, DATA_PIN1, COLOR_ORDER>(leds, NUM_LEDS1).setCorrection(TypicalLEDStrip); // 初始化灯带模板 无需变动复制粘贴即可
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(line, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(max_bright); // 初始化亮度
}

void LEDCtrl(void *Parameter)
{
  while (1)
  {
    switch (taskflag)
    {
    case 0:
      // Serial.printf("LED Refreshd");
      Arrowhead();
      break;
    case 1:
      fillLED();
      break;
    }
    vTaskDelay(1);
  }
  // Serial.printf("Task LEDCtrl ended.....");
  vTaskDelete(NULL);
}

void soundcheck(void *Parameter)
{
  while (1)
  {
    if (taskflag == 1)
    {
      vTaskDelay(5000);
      taskflag = 0;
    }
    if (digitalRead(13))
      taskflag = 1;
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}
/********************主函数************************/

void setup()
{
  // Serial.begin(115200);
  WS2812Init();
  pinMode(13, INPUT);
  for (int i = NUM_LEDS2 + 1; i < NUM_LEDS3; i++)
  {
    line[i] = CHSV(0, 255, 200);
  }
  xTaskCreate(LEDCtrl,      // 任务函数
              "LEDCtrl",    // 任务名字
              1024,         // 任务堆栈大小
              NULL,         // 传递给任务函数的参数
              2,            // 任务优先级
              NULL);        // 任务句柄
  xTaskCreate(soundcheck,   // 任务函数
              "soundcheck", // 任务名字
              1024,         // 任务堆栈大小
              NULL,
              1,
              NULL);
  // if (handleLEDCtrl)
  // Serial.printf("Task LEDCtrl is created .........Done");
}

void loop()
{
}