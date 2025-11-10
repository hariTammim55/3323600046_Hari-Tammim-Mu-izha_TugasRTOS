#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void OledTask(void *pvParameters) {
  int counter = 0;

  // Inisialisasi OLED di dalam task (pastikan Wire sudah diinisialisasi)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Gagal inisialisasi OLED!");
    vTaskDelete(NULL);  // hentikan task jika gagal
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED FreeRTOS ESP32-S3");
  display.display();

  for (;;) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Task OLED Running");
    display.setCursor(0, 20);
    display.print("Counter: ");
    display.println(counter++);
    display.display();

    Serial.println("OLED updated");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); // I2C pin untuk ESP32-S3 di Wokwi

  // Buat task OLED
  xTaskCreatePinnedToCore(
    OledTask,       // nama fungsi
    "TaskOLED",     // nama task
    4096,           // stack size
    NULL,           // parameter
    1,              // prioritas
    NULL,           // handle
    0               // core 0
  );
}

void loop() {

}
