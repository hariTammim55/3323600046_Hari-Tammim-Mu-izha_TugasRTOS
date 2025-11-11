# 🧩 Langkah Percobaan RTOS ESP32-S3 (Multi Task, Multi Core)

RTOS pada **ESP32-S3** sudah tersedia secara **bawaan**, sehingga tidak perlu mengunduh atau menginstal library tambahan.  
Pada proyek ini digunakan **FreeRTOS** dengan fungsi `xTaskCreatePinnedToCore()` untuk:
- Membuat beberapa task yang berjalan **secara paralel**
- Menentukan **task dijalankan pada core tertentu**
- Mengatur **prioritas** masing-masing task sesuai kebutuhan real-time

Tujuan utama:
- Memisahkan proses **input/monitoring** dan **output/aktuator**
- Menjaga sistem tetap **responsif**, **stabil**, dan **tidak saling mengganggu** antar task.

---

## ⚙️ Contoh Format Fungsi `xTaskCreatePinnedToCore()`

```cpp
xTaskCreatePinnedToCore(
  taskFunction,   // fungsi task
  "TaskName",     // nama task (label)
  stackSize,      // ukuran stack
  NULL,           // parameter task
  priority,       // prioritas task
  NULL,           // handle task (opsional)
  coreID          // ID core: 0 atau 1
);
```

Contoh penggunaan pada proyek ini:
- `EncoderTask`, `ButtonTask`, `PotentioTask`, `OledTask`, `DisplayTask` dijalankan di **Core 0**
- `StepperTask`, `ServoTask`, `BuzzerTask`, `LedTask` dijalankan di **Core 1**

---

## 🧩 Alasan Pembagian Core

### 🧠 Core 0 → Input & Monitoring (Sensor, OLED, Serial)

Core 0 difokuskan untuk task yang bersifat:
- **Membaca data** dari input:
  - Rotary encoder
  - Push button
  - Potensiometer
- **Menampilkan informasi** ke OLED
- **Monitoring** melalui Serial (`DisplayTask`)

Alasan:
- Task input dan tampilan membutuhkan **polling yang rutin**, namun tidak semuanya sangat ketat timing-nya.
- Dengan menempatkan input & display di Core 0, pembacaan sensor tidak terganggu oleh proses aktuator yang berat.
- Jika terjadi delay kecil pada OLED atau Serial, sistem kontrol utama tetap aman.

---

### ⚙️ Core 1 → Output / Aktuator (Stepper, Servo, Buzzer, LED)

Core 1 difokuskan untuk task:
- **Kontrol aktuator**:
  - Stepper motor (`StepperTask`)
  - Servo motor (`ServoTask`)
  - Buzzer (`BuzzerTask`)
  - LED indikator (`LedTask`)

Alasan:
- Aktuator tertentu, seperti **stepper** dan **servo**, membutuhkan timing yang lebih stabil.
- Dengan memisahkan ke Core 1:
  - Pulsa stepper lebih konsisten
  - Gerakan servo lebih halus
  - Pola buzzer lebih stabil
- Gangguan dari proses baca sensor dan update OLED di Core 0 dapat diminimalkan.

---

## 🔢 Alasan Penentuan Prioritas Task

Prioritas mencerminkan **seberapa penting** dan **seberapa cepat** task harus dieksekusi dibanding task lain.

### Core 0 (Input & Monitoring)

- `EncoderTask` → **Priority 5**  
  Encoder sensitif terhadap perubahan posisi. Prioritas tinggi mencegah kehilangan step rotasi.
- `ButtonTask` → **Priority 3**  
  Membutuhkan respon cukup cepat, tetapi tidak sekritis encoder.
- `PotentioTask` → **Priority 3**  
  Pembacaan analog rutin; cukup penting namun tidak real-time ketat.
- `OledTask` → **Priority 2**  
  Update tampilan boleh sedikit terlambat, tidak kritis.
- `DisplayTask` → **Priority 1**  
  Hanya untuk log Serial (debug/monitoring), sehingga aman ditempatkan sebagai prioritas terendah.

### Core 1 (Output / Aktuator)

- `StepperTask` → **Priority 3**  
  Menghasilkan pulsa step yang berurutan. Butuh prioritas lebih tinggi untuk menjaga kehalusan gerakan.
- `ServoTask` → **Priority 2**  
  Mengatur posisi servo secara periodik. Perlu stabil namun masih di bawah stepper.
- `BuzzerTask` → **Priority 2**  
  Pola suara diatur dengan delay mikrodetik; cukup penting untuk konsistensi, namun bukan paling kritis.
- `LedTask` → **Priority 1**  
  Indikator visual. Tidak membutuhkan respon cepat, sehingga prioritas paling rendah di Core 1.

---

## 📌 Ringkasan Desain

- **Core 0**: Fokus pada **input dan tampilan**, menjaga pembacaan sensor tetap teratur tanpa dibebani aktuator.
- **Core 1**: Fokus pada **aktuator dan proses yang butuh timing lebih stabil**.
- **Prioritas** diatur agar:
  - Task dengan kebutuhan timing tinggi (Encoder, Stepper) mendapat prioritas lebih besar.
  - Task non-kritis (LED, Serial, tampilan) tidak mengganggu alur utama.

Dengan arsitektur ini, proyek menunjukkan implementasi **multitasking FreeRTOS yang terstruktur** pada ESP32-S3, di mana pemisahan core dan prioritas membantu menjaga sistem tetap responsif dan andal.

---

## 🔗 Link Simulasi
https://wokwi.com/projects/447288031064180737
