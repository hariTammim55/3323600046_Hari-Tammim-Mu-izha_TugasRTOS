#include <Arduino.h>

#define Buzzer 4

void BuzzerTask(void *pvParameters){

  for(;;){
    tone(Buzzer, 2500);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    noTone(Buzzer);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    tone(Buzzer, 1000);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    noTone(Buzzer);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    BuzzerTask,       // nama fungsi
    "TaskBuzzer",     // nama task
    4096,           // stack size
    NULL,           // parameter
    1,              // prioritas
    NULL,           // handle
    0               // core 0
  );
}

void loop() {

}
