# 🧩 Langkah Percobaan 

RTOS pada **ESP32-S3** sudah tersedia secara **bawaan**, sehingga **tidak perlu mengunduh atau menginstal library tambahan**.  
Pada percobaan ini digunakan fungsi `xTaskCreatePinnedToCore()` untuk membuat task dan menempatkannya pada core tertentu.

---

### ⚙️ Format Fungsi
```cpp
xTaskCreatePinnedToCore(
  STEPPER,          // Nama fungsi task
  "STEPPER",        // Nama task
  4096,             // Ukuran stack (byte)
  NULL,             // Parameter task
  1,                // Prioritas task
  NULL,             // Handle task
  0                 // Core yang digunakan (Core 0)
);
```

---

### 🧠 Contoh Implementasi Fungsi Task
```cpp
void STEPPER(void *pvParameters) {
  while (1) {
    stepper.setSpeed(speed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    stepper.setSpeed(-speed);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
```

---

### ⏱️ Penjelasan Fungsi `vTaskDelay()`
```cpp
vTaskDelay(1000 / portTICK_PERIOD_MS);
```
Fungsi ini digunakan untuk **memberikan jeda (delay)** di dalam task tanpa menghentikan task lain yang berjalan.  
Nilai delay dihitung dalam **milidetik**, sehingga sistem tetap berjalan secara **multitasking** sesuai prinsip RTOS.

---

### 📁 File Pendukung
Gambar, kode lengkap, dan video hasil percobaan dapat dilihat pada **masing-masing folder** yang telah disediakan.

---

🧠 **Catatan:**  
Struktur RTOS seperti ini memungkinkan **pembagian beban kerja antar core secara paralel**, sehingga setiap peripheral seperti stepper, LED, atau sensor dapat dikontrol secara independen dan efisien.
