#include <Arduino.h>

#define LED_1 7
#define LED_2 15
#define LED_3 16

void Led1Task(void *pvParameters) {
  pinMode(LED_1, OUTPUT);
  bool state = false;

  for (;;) {
    state = !state;
    digitalWrite(LED_1, state);
    vTaskDelay(1500 / portTICK_PERIOD_MS);  // 500 ms
  }
}

void Led2Task(void *pvParameters) {
  pinMode(LED_2, OUTPUT);
  bool state = false;

  for (;;) {
    state = !state;
    digitalWrite(LED_2, state);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 300 ms
  }
}

void Led3Task(void *pvParameters) {
  pinMode(LED_3, OUTPUT);
  bool state = false;

  for (;;) {
    state = !state;
    digitalWrite(LED_3, state);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // 800 ms
  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    Led1Task,
    "LED1",
    2048,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    Led2Task,
    "LED2",
    2048,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    Led3Task,
    "LED3",
    2048,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
}
