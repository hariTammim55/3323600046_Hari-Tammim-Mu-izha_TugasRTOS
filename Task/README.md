# 🧩 Langkah Percobaan 

RTOS pada **ESP32-S3** sudah tersedia secara **bawaan**, sehingga **tidak perlu mengunduh atau menginstal library tambahan**.  
Pada percobaan ini digunakan fungsi `xTaskCreatePinnedToCore()` untuk membuat task dan menempatkannya pada core tertentu.

---

## 🧱 Daftar Percobaan

**1. Button**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task baca nilai Button  
- Setup task tampilkan nilai  
- Create task Button  
- Run task  

**2. Buzzer**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task Buzzer  
- Create dan run task  

**3. Encoder**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task Encoder  
- Create dan run task  

**4. LED**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task LED  
- Create dan run task  

**5. OLED**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task OLED  
- Create dan run task  

**6. Potensio**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task Potensio  
- Create dan run task  

**7. Servo**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task Servo  
- Create dan run task  

**8. Stepper**  
Langkah Percobaan:  
- Konfigurasi pin  
- Setup task Stepper  
- Create dan run task  

---

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
void Led2Task(void *pvParameters) {
  pinMode(LED_2, OUTPUT);
  bool state = false;

  for (;;) {
    state = !state;
    digitalWrite(LED_2, state);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 300 ms
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
