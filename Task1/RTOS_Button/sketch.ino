#include <Arduino.h>

#define button 16
#define led 10

int state = 0;

void ButtonTask(void *pvParameters){
  pinMode(button, INPUT_PULLUP);
  for(;;){
    state = digitalRead(button);
  }
}
void LedTask(void *pvParameters){
  pinMode(led, OUTPUT);
  for(;;){
    digitalWrite(led, state);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    ButtonTask,       // nama fungsi
    "TaskButton",     // nama task
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
