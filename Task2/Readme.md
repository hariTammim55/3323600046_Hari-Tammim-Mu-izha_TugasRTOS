## RTOS pada ESP32-S3 dengan FreeRTOS (Tanpa Library Tambahan)

ESP32-S3 sudah menggunakan **FreeRTOS bawaan**, sehingga **tidak perlu menginstal library RTOS tambahan**.  
Setiap fungsi `Task` dijalankan secara paralel menggunakan:

```cpp
xTaskCreatePinnedToCore(
  TaskFunction_t pvTaskCode,   // Fungsi task
  const char * const pcName,   // Nama task (untuk debug)
  const uint32_t usStackDepth, // Ukuran stack
  void *pvParameters,          // Parameter task (opsional)
  UBaseType_t uxPriority,      // Prioritas task
  TaskHandle_t *pvCreatedTask, // Handle task (opsional)
  const BaseType_t xCoreID     // Core tujuan (0 atau 1)
);
Contoh (sesuai program):

cpp
Salin kode
xTaskCreatePinnedToCore(EncoderTask,  "EncoderTask",  2048, NULL, 5, NULL, 0);
xTaskCreatePinnedToCore(ButtonTask,   "ButtonTask",   2048, NULL, 3, NULL, 0);
xTaskCreatePinnedToCore(PotentioTask, "PotentioTask", 2048, NULL, 3, NULL, 0);
xTaskCreatePinnedToCore(OledTask,     "OledTask",     4096, NULL, 2, NULL, 0);
xTaskCreatePinnedToCore(DisplayTask,  "DisplayTask",  2048, NULL, 1, NULL, 0);

xTaskCreatePinnedToCore(StepperTask,  "StepperTask",  4096, NULL, 3, NULL, 1);
xTaskCreatePinnedToCore(ServoTask,    "ServoTask",    2048, NULL, 2, NULL, 1);
xTaskCreatePinnedToCore(BuzzerTask,   "BuzzerTask",   2048, NULL, 2, NULL, 1);
xTaskCreatePinnedToCore(LedTask,      "LedTask",      2048, NULL, 1, NULL, 1);
Di dalam setiap task digunakan:

cpp
Salin kode
vTaskDelay(pdMS_TO_TICKS(x));
untuk memberikan delay non-blocking dalam satuan milidetik, sehingga task lain tetap bisa dijalankan oleh scheduler FreeRTOS.

Pembagian Core dan Prioritas Task
Core 0 — Input & Monitoring (Sensor + OLED + Serial)
Digunakan untuk tugas ringan yang bersifat pembacaan dan tampilan, tidak kritikal timing seketat aktuator.

Task pada Core 0:

EncoderTask — Priority 5

ButtonTask — Priority 3

PotentioTask — Priority 3

OledTask — Priority 2

DisplayTask — Priority 1

Alasan:

Core 0 difokuskan untuk handling input & UI sehingga terpisah dari beban kontrol aktuator di Core 1.

EncoderTask diberi prioritas tertinggi (5) karena perubahan encoder cepat dan sensitif, butuh respon lebih sering agar pembacaan tidak terlewat.

ButtonTask & PotentioTask prioritas 3 karena tetap butuh responsif, tapi tidak sekritis encoder.

OledTask prioritas 2 karena update tampilan periodik; boleh sedikit terlambat tanpa mengganggu sistem.

DisplayTask prioritas 1 karena hanya mencetak ke Serial (logging), paling tidak kritis.

Dengan konfigurasi ini, trafik input dan tampilan tetap halus tanpa mengganggu proses kontrol utama.

Core 1 — Output Realtime (Aktuator & Output Cepat)
Difokuskan untuk aktuator yang membutuhkan timing lebih stabil dan eksekusi lebih deterministik.

Task pada Core 1:

StepperTask — Priority 3

ServoTask — Priority 2

BuzzerTask — Priority 2

LedTask — Priority 1

Alasan:

Core 1 didedikasikan untuk kontrol aktuator agar tidak terganggu oleh proses OLED/Serial.

StepperTask prioritas 3 karena stepper membutuhkan timing relatif stabil saat generate pulsa langkah.

ServoTask prioritas 2 karena pergerakan servo butuh konsistensi namun tidak seketat stepper.

BuzzerTask prioritas 2 karena pola bunyi menggunakan timing mikrodetik/milidetik; dijaga agar tidak terlalu kalah oleh task lain.

LedTask prioritas 1 karena hanya indikator visual sederhana, tidak membutuhkan real-time tinggi.

Catatan Prioritas (FreeRTOS)
Angka lebih besar = prioritas lebih tinggi.

Task prioritas lebih tinggi akan lebih sering dieksekusi ketika ready.

Pembagian di atas memastikan:

Input cepat & akurat (encoder, tombol, potensiometer).

Aktuator berjalan stabil (stepper & servo di core terpisah).

Proses non-kritis (Serial, LED, sebagian OLED) tidak mengganggu fungsi utama sistem.
