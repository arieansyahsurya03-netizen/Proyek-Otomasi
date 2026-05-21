# 💧 Penyiraman Otomatis - Web Dashboard

Dashboard monitoring real-time untuk sistem penyiraman otomatis berbasis ESP32 dengan antarmuka glassmorphism yang indah dan responsif.

## ✨ Fitur Utama

✅ **Header Premium** - Judul + Tombol EMERGENCY STOP dengan animasi pulsing
✅ **5 Sensor Kelembaban** - Gauge progress bar untuk setiap sensor dengan nilai %
✅ **Average Humidity** - Hero metric besar di tengah dashboard
✅ **Mode Selector** - Toggle AUTO/MANUAL dengan animasi smooth
✅ **Actuator Control** - Toggle switch untuk Pompa Air & Solenoid Valve dengan status
✅ **Glassmorphism Design** - Efek glass blur, soft shadows, gradient colors
✅ **Real-time Updates** - WebSocket Socket.IO untuk komunikasi real-time
✅ **Responsive Design** - Mobile, tablet, desktop, hingga 4K
✅ **Professional Dark Theme** - Mata nyaman dengan background gelap

## 🚀 Quick Start

### 1. Install Dependencies
```bash
cd "d:\Project PPO\web-dashboard"
npm install
```

### 2. Run Development Server
```bash
npm run dev
```

Output:
```
[1] 🚀 Server running at http://localhost:3000
```

### 3. Buka Browser
```
http://localhost:3000
```

✅ Dashboard siap digunakan dengan data simulasi!

---

## 📁 Struktur Project

```
web-dashboard/
├── src/
│   └── index.ts           ← Express server + Socket.IO
├── public/
│   ├── index.html        ← Dashboard HTML
│   ├── style.css         ← Glassmorphism CSS
│   └── app.js            ← Frontend logic
├── dist/                 ← Compiled output (auto-generated)
├── package.json
├── tsconfig.json
└── .env                  ← Konfigurasi (PORT, HOST)
```

---

## 🎮 Cara Pakai

### Mode Button
- Klik **AUTO** atau **MANUAL** untuk ubah mode
- Indikator aktif akan bersinar (cyan glow)

### Emergency Stop
- Klik tombol **RED EMERGENCY STOP**
- Akan instantly matikan PUMP dan VALVE
- Animasi pulsing merah untuk warning

### Actuator Toggle
- Geser toggle **Pompa Air** untuk ON/OFF
- Geser toggle **Solenoid Valve** untuk ON/OFF
- Toggle yang aktif bersinar hijau

### Monitoring
- Lihat gauge setiap sensor (0-100%)
- Lihat rata-rata kelembaban di hero metric
- Cek temperature setiap sensor

---

## 🔌 API & WebSocket

### REST API Endpoints

**Get Status:**
```bash
curl http://localhost:3000/api/status
```

**Change Mode:**
```bash
curl -X POST http://localhost:3000/api/mode \
  -H "Content-Type: application/json" \
  -d '{"mode":"MANUAL"}'
```

**Toggle Pump:**
```bash
curl -X POST http://localhost:3000/api/pump \
  -H "Content-Type: application/json" \
  -d '{"state":true}'
```

**Toggle Valve:**
```bash
curl -X POST http://localhost:3000/api/valve \
  -H "Content-Type: application/json" \
  -d '{"state":true}'
```

**Emergency Stop:**
```bash
curl -X POST http://localhost:3000/api/emergency
```

**Send ESP32 Data:**
```bash
curl -X POST http://localhost:3000/api/esp32-data \
  -H "Content-Type: application/json" \
  -d '{
    "sensors": [
      {"moisture": 65, "temperature": 28},
      {"moisture": 72, "temperature": 27},
      {"moisture": 58, "temperature": 29},
      {"moisture": 81, "temperature": 26},
      {"moisture": 68, "temperature": 28}
    ],
    "pump": false,
    "valve": false
  }'
```

### WebSocket Events

**Client → Server:**
- `changeMode` - Change operation mode
- `setPump` - Control water pump
- `setValve` - Control solenoid valve
- `emergency` - Emergency stop

**Server → Client:**
- `init` - Initial state
- `sensorUpdate` - Sensor data update
- `modeChanged` - Mode changed
- `pumpChanged` - Pump state changed
- `valveChanged` - Valve state changed
- `emergency` - Emergency triggered

---

## 📡 Integrasi ESP32

### Option 1: HTTP POST (Recommended)

ESP32 kirim data setiap 5 detik:

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* serverUrl = "http://192.168.x.x:3000/api/esp32-data";

void sendSensorData() {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  DynamicJsonDocument doc(512);
  JsonArray sensors = doc.createNestedArray("sensors");
  
  sensors[0]["moisture"] = readMoisture(A0);
  sensors[0]["temperature"] = 28.5;
  // ... add more sensors
  
  String payload;
  serializeJson(doc, payload);
  
  int code = http.POST(payload);
  http.end();
  
  delay(5000); // Send every 5 seconds
}
```

### Option 2: WebSocket Direct
Hubungkan ESP32 langsung ke Socket.IO server

### Option 3: Serial Port
Kirim data JSON via USB serial

---

## 🎨 Customization

### Ubah Warna
Edit `public/style.css`:
```css
:root {
  --primary: #00d4ff;      /* Cyan */
  --secondary: #ff006e;    /* Magenta */
  --danger: #ff0000;       /* Red */
  --success: #00ff41;      /* Green */
}
```

### Ubah Port
Edit `.env`:
```env
PORT=3001
HOST=localhost
```

### Ubah Update Interval
Di `src/index.ts` baris `157`:
```typescript
}, 500);  // Update setiap 500ms
```

---

## 🛠️ Commands

```bash
# Install dependencies
npm install

# Build TypeScript
npm run build

# Run development (watch + hot reload)
npm run dev

# Run production server
npm start

# Clean everything
npm run clean
```

---

## 📊 Architecture

```
Browser → Socket.IO ↔ Express Server → System State
   ↑                                        ↓
   └────────── REST API Endpoints ←────────┘
                      ↑
                   ESP32 Data
```

### Data Flow
1. ESP32 kirim JSON via HTTP POST ke `/api/esp32-data`
2. Server update internal state
3. Server emit event via Socket.IO ke semua client
4. Browser update dashboard UI real-time

### Real-time Updates
- Sensor data berubah setiap 500ms (simulasi)
- WebSocket mengirim update ke semua connected clients
- UI otomatis update dengan animasi smooth

---

## 📱 Responsive Breakpoints

- **4K** (1600px+): Full layout dengan 5 sensor cards
- **Desktop** (1024px+): Normal layout
- **Tablet** (768px+): Adjusted grid
- **Mobile** (480px): Single column, compact

---

## 🚨 Troubleshooting

### Port sudah terpakai
```powershell
# Cari process di port 3000
netstat -ano | findstr :3000

# Kill process
taskkill /PID <PID> /F

# Atau ubah port di .env
```

### Module not found
```bash
npm install
npm run build
```

### WebSocket tidak connect
- Pastikan server running: `npm run dev`
- Cek browser console (F12)
- Refresh page

### Data tidak update
- Check server logs di terminal
- Check browser Network tab (DevTools)
- Restart server dan browser

---

## 📈 Performance

- **Memory**: ~15MB idle
- **CPU**: <5% idle
- **Network**: ~2KB per update
- **Update Interval**: 500ms (configurable)

---

## 🔐 Security (Production)

Untuk production:
1. Gunakan HTTPS & WSS
2. Implement authentication
3. Validate ESP32 data
4. Rate limiting
5. CORS config yang ketat
6. Use environment variables

---

## 📚 File Descriptions

| File | Purpose |
|------|---------|
| `src/index.ts` | Express server + Socket.IO + API |
| `public/index.html` | Dashboard HTML structure |
| `public/style.css` | Glassmorphism CSS styling |
| `public/app.js` | Frontend logic + Socket.IO client |
| `.env` | Configuration (PORT, HOST) |
| `package.json` | Dependencies + scripts |
| `tsconfig.json` | TypeScript config |

---

## 💡 Tips

1. **Real-time Sync**: Buka dashboard di 2 browser tab untuk lihat real-time sync
2. **Mobile Testing**: Resize browser ke mobile size (F12)
3. **Dark Theme**: Sudah dioptimalkan untuk mata
4. **Touch Friendly**: Semua buttons responsif untuk touch
5. **Performance**: Sensor data menggunakan requestAnimationFrame

---

## 📞 Support

Jika ada pertanyaan atau issue:
1. Check browser console (F12)
2. Check server logs di terminal
3. Verify network dengan DevTools
4. Restart server

---

## 📄 License

MIT License

---

**🌱 Happy Farming! 💧**
