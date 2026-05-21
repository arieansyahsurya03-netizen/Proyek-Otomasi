# 🚀 QUICK START GUIDE

## ⚡ Dalam 30 Detik

### 1️⃣ Install
```bash
cd "d:\Project PPO\web-dashboard"
npm install
```

### 2️⃣ Run
```bash
npm run dev
```

### 3️⃣ Open Browser
```
http://localhost:3000
```

✅ **DONE!** Dashboard sudah jalan dengan data simulasi!

---

## 🎮 Dashboard Controls

### Tombol Mode
- Klik **AUTO** atau **MANUAL**
- Indikator akan glow cyan

### Emergency Stop (RED BUTTON)
- Klik untuk matikan PUMP & VALVE
- Ada confirmation dialog
- Animasi pulsing untuk warning

### Toggle Switches
- Geser toggle untuk ON/OFF
- Active = glow hijau
- Real-time sync ke semua client

### Monitoring
- Lihat gauge 5 sensor (%)
- Lihat average humidity (besar)
- Lihat temperature
- Lihat status aktual

---

## 📡 Test dengan cURL

**Ubah Mode:**
```bash
curl -X POST http://localhost:3000/api/mode `
  -H "Content-Type: application/json" `
  -d '{"mode":"MANUAL"}'
```

**Pump ON:**
```bash
curl -X POST http://localhost:3000/api/pump `
  -H "Content-Type: application/json" `
  -d '{"state":true}'
```

**Valve OFF:**
```bash
curl -X POST http://localhost:3000/api/valve `
  -H "Content-Type: application/json" `
  -d '{"state":false}'
```

**Send ESP32 Data:**
```bash
curl -X POST http://localhost:3000/api/esp32-data `
  -H "Content-Type: application/json" `
  -d '{
    "sensors": [
      {"moisture": 65, "temperature": 28},
      {"moisture": 72, "temperature": 27},
      {"moisture": 58, "temperature": 29},
      {"moisture": 81, "temperature": 26},
      {"moisture": 68, "temperature": 28}
    ],
    "pump": true,
    "valve": false
  }'
```

---

## 🛠️ Basic Commands

```bash
npm install              # Install dependencies
npm run dev              # Development mode (watch + hot reload)
npm run build            # Build TypeScript
npm start                # Production server
npm run clean            # Clean up
```

---

## 📁 File Structure

```
web-dashboard/
├── src/index.ts         ← Server code
├── public/
│   ├── index.html      ← Dashboard HTML
│   ├── style.css       ← Styling
│   └── app.js          ← Frontend logic
├── dist/               ← Compiled (auto-generated)
├── package.json        ← Dependencies
└── README.md           ← Full documentation
```

---

## 🔌 ESP32 Integration

### HTTP POST Method (Recommended)

1. Update `SERVER` di ESP32 code dengan IP server
2. Upload code ke ESP32
3. ESP32 akan kirim data setiap 5 detik
4. Dashboard update real-time!

Lihat file `ESP32_EXAMPLE.ino` untuk contoh lengkap.

---

## 📱 Responsive Design

Buka di mobile/tablet:
- Resize browser → layout auto adjust
- Single column di mobile
- Multi-column di desktop
- Touch-friendly buttons

---

## 🚨 Troubleshooting

### Port 3000 terpakai?
```bash
# Ubah di .env
PORT=3001
npm run dev
```

### Module error?
```bash
npm install
npm run build
```

### WebSocket tidak connect?
- Check server running (`npm run dev`)
- Press F12 → Console
- Refresh page

---

## 💡 Features

✅ Glassmorphism UI
✅ Real-time WebSocket
✅ 5 Sensor Gauges
✅ Emergency Stop
✅ Mode Selector
✅ Actuator Control
✅ Responsive Design
✅ Dark Theme
✅ Smooth Animations

---

## 📚 More Info

See `README.md` untuk dokumentasi lengkap!

---

**Happy Farming! 🌱💧**
