# 🧩 Langkah Percobaan 

RTOS pada **ESP32-S3** sudah tersedia secara **bawaan**, sehingga **tidak perlu mengunduh atau menginstal library tambahan**.  
Pada percobaan ini digunakan fungsi `xTaskCreatePinnedToCore()` untuk membuat task dan menempatkannya pada core tertentu.

---
**1. Button**
Langkah Percobaan:
- Konfigurasi Pin
- Setup task baca nilai Button
- Setup task tampilkan nilai
- Create task button
- Run task

**2. Buzzer**
Langkah Percobaan:
- Konfigurasi Pin
- Setup Task Buzzer
- Create dan run task

**3. Encoder**
Langkah Percobaan:
- Konfigurasi pin
- Setup task Encoder
- Create dan run task

**4. led**
Langkah Percobaan:
- Konfigurasi pin
- Setup task Led
- Create dan run task

**5. Oled**
Langkah Percobaan:
- Konfigurasi pin
- Setup task Oled
- Create dan run task

**6. Potentio**
Langkah Percobaan:
- Konfigurasi pin
- Setup task potentio
- Create dan run task

**7. Servo**
Langkah Percobaan:
- Konfigurasi pin
- Setup task servo
- Create dan run task

**8. Stepper**
Langkah Percobaan:
- Konfigurasi pin
- Setup task stepper
- Create dan run task

```
```

### ⚙️ Format Fungsi
```cpp
xTaskCreatePinnedToCore(
    taskFunction,     // fungsi task
    "TaskName",       // nama task
    stackSize,        // ukuran stack
    NULL,             // parameter
    priority,         // prioritas task
    NULL,             // handle task
    coreID            // 0 atau 1 untuk menentukan core
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
