#include <Arduino.h>

#define CLK 4
#define DT  5
#define SW  6

int counter = 0;
int lastStateCLK = 0;
unsigned long lastButtonPress = 0;

void EncoderTask(void *pvParameters) {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT,  INPUT_PULLUP);
  pinMode(SW,  INPUT_PULLUP);

  lastStateCLK = digitalRead(CLK);

  for (;;) {
    int currentStateCLK = digitalRead(CLK);

    // hitung hanya saat terjadi falling edge: HIGH -> LOW
    if (lastStateCLK == HIGH && currentStateCLK == LOW) {
      if (digitalRead(DT) == HIGH) {
        counter++;          // searah jarum jam
      } else {
        counter--;          // berlawanan
      }
      Serial.print("Posisi: ");
      Serial.println(counter);
    }

    lastStateCLK = currentStateCLK;

    // tombol push (SW) dengan debounce sederhana
    if (digitalRead(SW) == LOW) {
      if (millis() - lastButtonPress > 200) {
        Serial.println("Tombol ditekan!");
        lastButtonPress = millis();
      }
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(500 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    EncoderTask,
    "TaskEncoder",
    4096,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {

}
