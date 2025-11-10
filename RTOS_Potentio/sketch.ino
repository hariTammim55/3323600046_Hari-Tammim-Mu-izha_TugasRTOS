#include <Arduino.h>

#define Pot 7
#define led 8

uint16_t ADC = 0;
uint16_t PWM = 0;

void PotentioTask(void *pvParameters){
  pinMode(Pot, INPUT);
  for(;;){
    ADC = analogRead(Pot);
    Serial.print("ADC = ");
    Serial.println(ADC);
    vTaskDelay(50 / portTICK_PERIOD_MS);  
  }
}
void LedTask(void *pvParameters){
  pinMode(led, OUTPUT);
  for(;;){
    PWM = (ADC*255) / 4095;
    analogWrite(led,PWM);
    vTaskDelay(10 / portTICK_PERIOD_MS);  
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    PotentioTask,       // nama fungsi
    "TaskPotentio",     // nama task
    4096,           // stack size
    NULL,           // parameter
    1,              // prioritas
    NULL,           // handle
    0               // core 0
  );

  xTaskCreatePinnedToCore(
    LedTask,       // nama fungsi
    "TaskLed",     // nama task
    4096,           // stack size
    NULL,           // parameter
    1,              // prioritas
    NULL,           // handle
    0               // core 0
  );
}

void loop() {

}
