# 🧩 Langkah Percobaan RTOS ESP32-S3 (Multi Task, Multi Core)

RTOS pada **ESP32-S3** sudah tersedia secara **bawaan**, sehingga tidak perlu mengunduh atau menginstal library tambahan.  
Pada proyek ini digunakan **FreeRTOS** dengan fungsi `xTaskCreatePinnedToCore()` untuk:
- Membuat beberapa task yang berjalan **secara paralel**
- Mengatur **task tertentu berjalan pada core tertentu**
- Mengatur **prioritas** tiap task sesuai kebutuhan real-time

Tujuan utamanya adalah memisahkan **task input/monitoring** dan **task output/aktuator** agar sistem lebih stabil, responsif, dan tidak saling mengganggu.

---

## ⚙️ Alasan Pembagian Core

### 🧩 Core 0 → Input & Monitoring (Sensor + OLED + Serial)

Core 0 digunakan untuk task yang:
- Berhubungan dengan **pembacaan data** (encoder, button, potensio)
- Menampilkan informasi ke **OLED**
- Mengirim data monitoring ke **Serial**

Karakteristiknya:
- Membutuhkan **respons cukup cepat**, terutama untuk encoder
- Tidak boleh terlalu terganggu oleh proses berat aktuator
- Tetap aman jika ada sedikit jitter visual (OLED, serial)

### ⚙️ Core 1 → Output Realtime (Stepper, Servo, Buzzer, LED)

Core 1 digunakan untuk task:
- **Aktuator** yang mengontrol gerakan dan sinyal fisik:
  - Stepper (pulse berurutan)
  - Servo
  - Buzzer
  - LED
- Membutuhkan timing yang lebih stabil agar:
  - Stepper tidak kehilangan langkah
  - Servo bergerak halus
  - Buzzer memiliki pola suara konsisten

Dengan memisahkan aktuator di Core 1, gangguan dari pembacaan sensor dan update display di Core 0 dapat diminimalkan.

---

## 🔢 Alasan Penentuan Prioritas Task

Prioritas (`uxPriority`) dipilih berdasarkan **seberapa kritis** dan **seberapa cepat** task harus dieksekusi:

### Core 0 (Sensor & Monitoring)
- `EncoderTask` → **Priority 5** (paling tinggi di Core 0)  
  Encoder harus dibaca secepat dan setepat mungkin agar tidak kehilangan perubahan posisi.
- `ButtonTask` → **Priority 3**  
  Button perlu responsif, tapi tidak sepenting encoder.
- `PotentioTask` → **Priority 3**  
  Pembacaan potensio cukup cepat, tetapi tidak seketat encoder.
- `OledTask` → **Priority 2**  
  Update tampilan tidak butuh real-time tinggi, boleh sedikit terlambat.
- `DisplayTask` → **Priority 1**  
  Hanya mencetak data ke Serial untuk debug/monitoring, sehingga menjadi prioritas paling rendah.

### Core 1 (Aktuator / Output)
- `StepperTask` → **Priority 3**  
  Stepper butuh timing cukup stabil agar gerakan halus dan tidak kehilangan step.
- `ServoTask` → **Priority 2**  
  Servo penting, tetapi masih lebih toleran dibanding stepper.
- `BuzzerTask` → **Priority 2**  
  Pola buzzer diatur cukup presisi, tapi bukan paling kritis.
- `LedTask` → **Priority 1**  
  Hanya indikator visual, sehingga boleh diproses terakhir.

Struktur ini membuat:
- Task yang **benar-benar kritis timing** (Encoder, Stepper) mendapat prioritas lebih tinggi.
- Task **informasi & visual** (OLED, Serial, LED) tidak mengganggu proses utama.

---

## 🧠 Contoh Format `xTaskCreatePinnedToCore()`

```cpp
xTaskCreatePinnedToCore(
  taskFunction,     // fungsi task
  "TaskName",       // nama task
  stackSize,        // ukuran stack
  NULL,             // parameter task
  priority,         // prioritas task
  NULL,             // handle task
  coreID            // 0 atau 1 untuk menentukan core
);
```

Struktur ini menunjukkan implementasi **multitasking berbasis RTOS** pada ESP32-S3 dengan pemisahan beban kerja antar core dan prioritas task yang terencana.
