# Langkah Percobaan

RTOS pada ESP32-S3 sudah tersedia secara bawaan, sehingga tidak memerlukan instalasi library tambahan.

Program menggunakan fungsi xTaskCreatePinnedToCore() dengan format berikut:

xTaskCreatePinnedToCore(
  STEPPER,          // Nama fungsi task
  "STEPPER",        // Nama task (label)
  4096,             // Ukuran stack (byte)
  NULL,             // Parameter task
  1,                // Prioritas task
  NULL,             // Handle task
  0                 // Core yang digunakan (Core 0)
);


Setelah itu, buat fungsi sesuai dengan nama task yang telah didefinisikan sebelumnya.
Contoh fungsi task untuk mengatur arah dan kecepatan motor stepper:

void STEPPER(void *pvParameters) {
  for (;;) {
    stepper.setSpeed(speed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    stepper.setSpeed(-speed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


Syntax berikut digunakan untuk memberikan jeda (delay) di dalam task:

vTaskDelay(1000 / portTICK_PERIOD_MS);


Fungsi ini berfungsi untuk menunda eksekusi task selama waktu yang ditentukan (dalam satuan milidetik), tanpa mengganggu task lain yang berjalan secara paralel di sistem RTOS.
