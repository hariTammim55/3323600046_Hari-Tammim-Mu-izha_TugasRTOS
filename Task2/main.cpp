#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ----------------- Pin Mapping -----------------
#define LED_PIN 10
#define POT_PIN 11
#define BTN_PIN 7
#define BUZZER_PIN 3
#define SCL_PIN 9
#define SDA_PIN 8
#define CLK_PIN 12
#define DT_PIN 13
#define SW_PIN 14
#define EN_PIN 45
#define DIR_PIN 47
#define STEP_PIN 48
#define SERVO_PIN 15

// ----------------- OLED Config -----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----------------- Servo & Stepper -----------------
Servo myServo;
const int stepsPerRev = 200;
volatile long stepPosition = 0;

// ----------------- Shared Variables -----------------
volatile int potValue = 0;
volatile bool buttonPressed = false;
volatile long encoderValue = 0;

// ----------------- Encoder Internal State -----------------
int lastStateCLK = HIGH;
unsigned long lastEncoderBtnPress = 0;

// =========================================================
// TASK: LED
// =========================================================
void LedTask(void *pvParameters)
{
  bool state = false;
  for (;;)
  {
    state = !state;
    digitalWrite(LED_PIN, state);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// =========================================================
// TASK: BUTTON (BTN_PIN)
// =========================================================
void ButtonTask(void *pvParameters)
{
  for (;;)
  {
    if (digitalRead(BTN_PIN) == LOW)
    {
      buttonPressed = true;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// =========================================================
// TASK: BUZZER
// =========================================================
void BuzzerTask(void *pvParameters)
{
  for (;;)
  {
    unsigned long start = millis();
    while (millis() - start < 1000)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(2000);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(2000);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// =========================================================
// TASK: OLED
// =========================================================
void OledTask(void *pvParameters)
{
  int counter = 0;
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    // Gagal? Lanjut saja.
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED FreeRTOS ESP32-S3");
  display.display();

  for (;;)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Task OLED Running");
    display.setCursor(0, 16);
    display.print("Counter: ");
    display.println(counter++);
    display.display();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// =========================================================
// TASK: POTENSIO
// =========================================================
void PotentioTask(void *pvParameters)
{
  for (;;)
  {
    potValue = analogRead(POT_PIN);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

// =========================================================
// TASK: ENCODER ROTARY
// =========================================================
void EncoderTask(void *pvParameters)
{
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK_PIN);

  for (;;)
  {
    int clk = digitalRead(CLK_PIN);
    if (lastStateCLK == HIGH && clk == LOW)
    {
      encoderValue += (digitalRead(DT_PIN) == HIGH) ? 1 : -1;
    }
    lastStateCLK = clk;

    if (digitalRead(SW_PIN) == LOW && millis() - lastEncoderBtnPress > 200)
    {
      lastEncoderBtnPress = millis();
    }
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

// =========================================================
// TASK: SERVO (DIPERBAIKI)
// =========================================================
void ServoTask(void *pvParameters)
{
  myServo.attach(SERVO_PIN);

  for (;;)
  {
    myServo.write(0);
    vTaskDelay(pdMS_TO_TICKS(1500));
    myServo.write(90);
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

// =========================================================
// TASK: STEPPER
// =========================================================
void StepperTask(void *pvParameters)
{
  digitalWrite(EN_PIN, LOW);
  const int stepDelayMicros = 1500;

  for (;;)
  {
    digitalWrite(DIR_PIN, HIGH);
    for (int i = 0; i < stepsPerRev; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelayMicros);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelayMicros);
      stepPosition++;
    }
    vTaskDelay(pdMS_TO_TICKS(500));

    digitalWrite(DIR_PIN, LOW);
    for (int i = 0; i < stepsPerRev; i++)
    {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(stepDelayMicros);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(stepDelayMicros);
      stepPosition--;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// =========================================================
// TASK: DISPLAY SERIAL
// =========================================================
void DisplayTask(void *pvParameters)
{
  for (;;)
  {
    int pot = potValue;
    long enc = encoderValue;
    bool btn = buttonPressed;
    if (btn)
      buttonPressed = false;

    Serial.print("Potentio : ");
    Serial.print(pot);
    Serial.print(" || Button : ");
    Serial.print(btn ? "1" : "0");
    Serial.print(" || Encoder : ");
    Serial.println(enc);

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// =========================================================
// SETUP
// =========================================================
void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  // ==== Core 0 untuk sensor dan display
  xTaskCreatePinnedToCore(EncoderTask, "EncoderTask", 2048, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(ButtonTask, "ButtonTask", 2048, NULL, 4, NULL, 0);
  xTaskCreatePinnedToCore(PotentioTask, "PotentioTask", 2048, NULL, 3, NULL, 0);
  xTaskCreatePinnedToCore(OledTask, "OledTask", 4096, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(DisplayTask, "DisplayTask", 2048, NULL, 1, NULL, 0);

  // ===== Core 1 untuk Output Realtime
  xTaskCreatePinnedToCore(StepperTask, "StepperTask", 4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(ServoTask, "ServoTask", 2048, NULL, 4, NULL, 1);
  xTaskCreatePinnedToCore(BuzzerTask, "BuzzerTask", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(LedTask, "LedTask", 2048, NULL, 1, NULL, 1);
}

void loop()
{
  // Tidak digunakan
}