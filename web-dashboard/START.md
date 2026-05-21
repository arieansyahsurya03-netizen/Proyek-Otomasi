![Penyiraman Otomatis](https://img.shields.io/badge/Penyiraman-Otomatis-success)
![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)

# 💧 Penyiraman Otomatis - Dashboard Monitoring

Dashboard web profesional untuk sistem penyiraman otomatis berbasis ESP32 dengan antarmuka glassmorphism yang elegan, responsif, dan real-time.

## 🌟 Fitur Utama

### Dashboard Interface
- ✨ **Glassmorphism Design** - Efek blur modern dengan gradient colors
- 🌙 **Dark Professional Theme** - Mudah dipandang, professional
- 📱 **Responsive Design** - Mobile, Tablet, Desktop, 4K
- ⚡ **Real-time Updates** - WebSocket communication via Socket.IO
- 🎨 **Smooth Animations** - Transitions yang halus dan natural

### Komponen Dashboard
- **Header** - Judul branding + Emergency Stop button (merah pulsing)
- **Mode Control** - Toggle AUTO/MANUAL dengan visual feedback
- **5 Sensor Gauges** - Progress bar untuk setiap soil moisture sensor
- **Average Humidity** - Large hero metric di tengah dashboard
- **Actuator Control** - Toggle switch untuk Water Pump & Solenoid Valve
- **Status Display** - Connection status, mode, dan last update time

### Backend Features
- 🚀 **Express.js Server** - Fast dan lightweight
- 📡 **Socket.IO** - Real-time bidirectional communication
- 🔌 **REST API** - Complete API untuk kontrol sistem
- 📊 **Data Simulation** - Otomatis generate sensor data untuk testing
- 🔄 **State Management** - Central system state management

---

## 📋 Prerequisites

- **Node.js** v18+ ([Download](https://nodejs.org))
- **npm** v9+ (included dengan Node.js)
- **Git** (optional)
- **ESP32** (untuk production use)

---

## 🚀 Quick Start (30 detik)

### 1. Navigate ke Folder Project
```bash
cd "d:\Project PPO\web-dashboard"
```

### 2. Install Dependencies
```bash
npm install
```

### 3. Jalankan Server
```bash
npm start
```

Atau untuk development dengan watch mode:
```bash
npm run dev
```

### 4. Buka Browser
```
http://localhost:3000
```

✅ **Dashboard siap digunakan!**

---

## 📖 Dokumentasi Lengkap

| File | Deskripsi |
|------|-----------|
| 📘 [README.md](README.md) | Dokumentasi lengkap, API reference, customization |
| ⚡ [QUICKSTART.md](QUICKSTART.md) | Quick start guide (30 detik) |
| 🧪 [TESTING.md](TESTING.md) | Testing guide dan feature checklist |
| 📑 [INDEX.md](INDEX.md) | File index dan struktur project |
| 📊 [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) | Project overview dan tech stack |

---

## 🎮 Cara Menggunakan Dashboard

### Mode Control
```
Klik tombol AUTO atau MANUAL
→ Indicator akan glow dengan warna cyan
→ Real-time sync ke semua connected clients
```

### Emergency Stop
```
Klik tombol RED "EMERGENCY STOP"
→ Confirmation dialog muncul
→ Pump & Valve instant OFF
→ Animasi pulsing untuk warning
```

### Monitor Sensor
```
Lihat 5 gauge sensor:
  - Progress bar (0-100%)
  - Temperature display
  - Auto-update setiap 500ms
→ Rata-rata humidity di hero metric
```

### Control Actuators
```
Toggle switch untuk:
  - Water Pump (💧)
  - Solenoid Valve (🚰)
→ Green glow ketika active
→ Instant response
```

---

## 🔌 Integrasi ESP32

### HTTP POST Method (Recommended)

ESP32 mengirim data setiap 5 detik:

```cpp
#include <HTTPClient.h>
#include <ArduinoJson.h>

void sendSensorData() {
  HTTPClient http;
  http.begin("http://SERVER_IP:3000/api/esp32-data");
  http.addHeader("Content-Type", "application/json");
  
  DynamicJsonDocument doc(512);
  JsonArray sensors = doc.createNestedArray("sensors");
  
  // Baca sensor 1-5
  sensors[0]["moisture"] = analogRead(SENSOR1);
  sensors[0]["temperature"] = 25.5;
  // ... sensor 2-5
  
  doc["pump"] = digitalRead(PUMP_PIN);
  doc["valve"] = digitalRead(VALVE_PIN);
  
  String payload;
  serializeJson(doc, payload);
  
  http.POST(payload);
  http.end();
}
```

**Lihat file `ESP32_EXAMPLE.ino` untuk kode lengkap!**

---

## 📡 API Reference

### GET /api/status
Ambil status sistem lengkap
```bash
curl http://localhost:3000/api/status
```

Response:
```json
{
  "sensors": [{"id": 1, "moisture": 65, "temperature": 28, ...}, ...],
  "averageHumidity": 68,
  "pump": false,
  "valve": false,
  "mode": "AUTO",
  "lastUpdate": 1715553600000
}
```

### POST /api/mode
Ubah mode (AUTO/MANUAL)
```bash
curl -X POST http://localhost:3000/api/mode \
  -H "Content-Type: application/json" \
  -d '{"mode":"MANUAL"}'
```

### POST /api/pump
Kontrol water pump
```bash
curl -X POST http://localhost:3000/api/pump \
  -H "Content-Type: application/json" \
  -d '{"state":true}'
```

### POST /api/valve
Kontrol solenoid valve
```bash
curl -X POST http://localhost:3000/api/valve \
  -H "Content-Type: application/json" \
  -d '{"state":true}'
```

### POST /api/emergency
Emergency stop
```bash
curl -X POST http://localhost:3000/api/emergency
```

### POST /api/esp32-data
Terima data dari ESP32
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

---

## 🎨 Customization

### Ubah Port (Default: 3000)
Edit `.env`:
```env
PORT=3001
HOST=localhost
```

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

### Ubah Sensor Update Interval
Edit `src/index.ts` baris ~160:
```typescript
}, 500);  // Update setiap 500ms (default)
```

---

## 🛠️ Commands

```bash
# Setup & Install
npm install          # Install semua dependencies

# Development
npm run dev          # Run dengan TypeScript watch mode
npm run build        # Compile TypeScript saja
npm run watch:ts     # Watch TypeScript tanpa server

# Production
npm start            # Run production server
npm run build        # Build sebelum production

# Cleanup
npm run clean        # Hapus dist & node_modules
```

---

## 📊 Tech Stack

```
Frontend               Backend
├── HTML5             ├── Node.js
├── CSS3 (Glass UI)   ├── Express.js
└── JavaScript        ├── Socket.IO
    └── Socket.IO     └── TypeScript
                      
Build                 Package Manager
├── TypeScript        └── npm v9+
└── Compiler
```

---

## 🚨 Troubleshooting

### Port sudah terpakai
```bash
# Windows - Cari process di port 3000
netstat -ano | findstr :3000

# Kill process
taskkill /PID <PID> /F

# Atau ubah port di .env
PORT=3001
```

### Module not found error
```bash
# Reinstall dependencies
npm install

# Clean dan install ulang
npm run clean
npm install
npm run build
```

### Server tidak start
```bash
# Check untuk syntax errors
npm run build

# Lihat error detail
npm start
```

### WebSocket tidak connect
- Pastikan server running: `npm run dev` atau `npm start`
- Buka DevTools (F12) → Console tab
- Check untuk error messages
- Refresh page

### Dashboard tidak update
- Check browser Network tab (DevTools)
- Verify server logs di terminal
- Check Socket.IO connection status
- Restart server dan browser

---

## 📱 Responsive Breakpoints

Dashboard responsive di semua ukuran:

| Device | Width | Layout |
|--------|-------|--------|
| 4K Monitor | 1600px+ | Full layout |
| Desktop | 1024px+ | Normal layout |
| Tablet | 768px+ | Adjusted grid |
| Mobile | 480px | Single column |

Coba dengan F12 → Device Toolbar untuk testing!

---

## 🎯 Feature Checklist

- [x] Glassmorphism UI design
- [x] Header dengan title + Emergency Stop
- [x] Mode selector (AUTO/MANUAL)
- [x] 5 sensor moisture gauges
- [x] Average humidity hero metric
- [x] Pump control toggle
- [x] Valve control toggle
- [x] System status display
- [x] Real-time WebSocket sync
- [x] Responsive design
- [x] Dark theme
- [x] REST API endpoints
- [x] ESP32 integration
- [x] Data simulation
- [x] Complete documentation

---

## 🔐 Production Tips

Untuk deployment ke production:

1. **Security**
   - Gunakan HTTPS (SSL/TLS)
   - Implement authentication
   - Validate semua input
   - Use environment variables

2. **Performance**
   - Enable gzip compression
   - Minify CSS/JS
   - Setup CDN untuk static files
   - Monitor server resources

3. **Monitoring**
   - Setup logging system
   - Monitor error rates
   - Track performance metrics
   - Setup alerts

4. **Backup**
   - Database backup regular
   - Configuration backup
   - Code version control

---

## 📚 Learning Resources

- [Express.js Documentation](https://expressjs.com)
- [Socket.IO Guide](https://socket.io/docs)
- [TypeScript Handbook](https://www.typescriptlang.org/docs)
- [MDN Web Docs](https://developer.mozilla.org)
- [Arduino & ESP32](https://www.arduino.cc)

---

## 📄 License

MIT License - Anda bebas menggunakan, modify, dan distribute project ini!

---

## 🙋 Support & Kontribusi

Punya pertanyaan atau mau berkontribusi?

1. Baca dokumentasi di README.md
2. Check TESTING.md untuk testing guide
3. Lihat browser console untuk error (F12)
4. Check server logs di terminal

---

## 🌱 Roadmap (Future Features)

- [ ] Database integration (sensor history)
- [ ] User authentication & authorization
- [ ] Advanced scheduling system
- [ ] Mobile app (React Native)
- [ ] Alert system & notifications
- [ ] Data export & analytics
- [ ] Multi-user support
- [ ] Cloud deployment ready

---

## 👨‍💻 Development Notes

### Project Structure
```
src/index.ts          Main server (Express + Socket.IO)
public/index.html     Dashboard HTML
public/style.css      Glassmorphism CSS
public/app.js         Frontend logic + WebSocket
```

### Key Files
- `.env` - Configuration
- `package.json` - Dependencies
- `tsconfig.json` - TypeScript config
- `dist/` - Compiled output

### Development Workflow
1. Edit code
2. Save file
3. TypeScript auto-compile
4. Browser auto-reload
5. Test & verify

---

## 📞 Quick Links

| Resource | Link |
|----------|------|
| 📘 Full Docs | [README.md](README.md) |
| ⚡ Quick Start | [QUICKSTART.md](QUICKSTART.md) |
| 🧪 Testing | [TESTING.md](TESTING.md) |
| 📑 File Index | [INDEX.md](INDEX.md) |
| 📊 Summary | [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) |
| 💻 ESP32 Code | [ESP32_EXAMPLE.ino](ESP32_EXAMPLE.ino) |

---

## 🎉 Siap Mulai?

**1. Clone/Download project**
```bash
cd "d:\Project PPO\web-dashboard"
```

**2. Install & run**
```bash
npm install
npm start
```

**3. Buka browser**
```
http://localhost:3000
```

✨ **Dashboard sudah siap digunakan!**

---

## 📝 Changelog

**v1.0.0** (Initial Release)
- Complete dashboard UI dengan glassmorphism design
- Express.js server dengan Socket.IO
- 5 sensor monitoring dengan gauges
- Mode selector (AUTO/MANUAL)
- Emergency stop functionality
- Actuator control (pump & valve)
- Real-time WebSocket sync
- Responsive design
- Full documentation
- ESP32 integration example

---

**Created with ❤️ untuk Sistem Penyiraman Otomatis**

**Happy Farming! 🌱💧**

---

*Last Updated: May 12, 2026*
