#include <Arduino.h>

// Pin koneksi ke A4988
#define STEP_PIN    4
#define DIR_PIN     5
#define ENABLE_PIN  6

// parameter motor
const int stepsPerRev = 200;   

volatile long stepPosition = 0; 

void StepperTask(void *pvParameters) {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, LOW);

  const int stepDelayMicros = 1500; 

  for (;;) {
    // ---- Putar searah jarum jam ----
    digitalWrite(DIR_PIN, HIGH);
    for (int i = 0; i < stepsPerRev; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelayMicros);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelayMicros);
      stepPosition++;  
    }
    vTaskDelay(500 / portTICK_PERIOD_MS); 

    // ---- Putar berlawanan jarum jam ----
    digitalWrite(DIR_PIN, LOW);
    for (int i = 0; i < stepsPerRev; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelayMicros);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelayMicros);
      stepPosition--;  
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void MonitorTask(void *pvParameters) {
  for (;;) {
    long pos = stepPosition;   
    Serial.print("Posisi step: ");
    Serial.println(pos);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(500 / portTICK_PERIOD_MS); 

  xTaskCreatePinnedToCore(
    StepperTask,
    "StepperTask",
    4096,
    NULL,
    2,          
    NULL,
    0          
  );

  xTaskCreatePinnedToCore(
    MonitorTask,
    "MonitorTask",
    2048,
    NULL,
    1,
    NULL,
    1           
  );
}

void loop() {

}
