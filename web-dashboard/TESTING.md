# 🧪 TESTING GUIDE

## ✅ Server Status

Server sudah running di: **http://localhost:3000**

---

## 📱 Browser Testing

### 1. Buka Dashboard
```
http://localhost:3000
```

Anda akan melihut:
- ✓ Dashboard dengan 5 sensor
- ✓ Hero metric average humidity
- ✓ Control panel dengan mode AUTO/MANUAL
- ✓ Emergency stop button (red)
- ✓ Toggle switch untuk pump dan valve

### 2. Test Controls

**Mode Button:**
- Klik AUTO → indikator berubah
- Klik MANUAL → indikator berubah
- Real-time sync ke semua tab/client yang buka

**Emergency Stop:**
- Klik tombol RED
- Dialog konfirmasi muncul
- Pump & Valve akan OFF

**Toggle Switches:**
- Toggle Pump ON/OFF
- Toggle Valve ON/OFF
- Status langsung update

**Monitoring:**
- Lihat gauge setiap sensor berubah setiap 500ms
- Lihat temperature update
- Average humidity update otomatis

---

## 🔌 API Testing

### Get Status
```powershell
Invoke-WebRequest 'http://localhost:3000/api/status'
```

Response:
```json
{
  "sensors": [...],
  "averageHumidity": 68,
  "pump": false,
  "valve": false,
  "mode": "AUTO",
  "lastUpdate": 1715553600000
}
```

### Change Mode
```powershell
$body = '{"mode":"MANUAL"}'
Invoke-WebRequest 'http://localhost:3000/api/mode' `
  -Method POST `
  -Body $body `
  -ContentType 'application/json'
```

### Control Pump
```powershell
$body = '{"state":true}'
Invoke-WebRequest 'http://localhost:3000/api/pump' `
  -Method POST `
  -Body $body `
  -ContentType 'application/json'
```

### Control Valve
```powershell
$body = '{"state":false}'
Invoke-WebRequest 'http://localhost:3000/api/valve' `
  -Method POST `
  -Body $body `
  -ContentType 'application/json'
```

### Emergency Stop
```powershell
Invoke-WebRequest 'http://localhost:3000/api/emergency' `
  -Method POST
```

### Send ESP32 Data
```powershell
$body = @{
  sensors = @(
    @{moisture = 65; temperature = 28},
    @{moisture = 72; temperature = 27},
    @{moisture = 58; temperature = 29},
    @{moisture = 81; temperature = 26},
    @{moisture = 68; temperature = 28}
  )
  pump = $true
  valve = $false
} | ConvertTo-Json

Invoke-WebRequest 'http://localhost:3000/api/esp32-data' `
  -Method POST `
  -Body $body `
  -ContentType 'application/json'
```

---

## 🎥 WebSocket Testing

Buka 2 browser tab:
1. Tab 1: http://localhost:3000
2. Tab 2: http://localhost:3000

Aksi di Tab 1 (Toggle pump):
- Pump state berubah di Tab 1
- Pump state juga berubah di Tab 2 (real-time!)
- Ini adalah WebSocket communication

---

## ✨ Features Checklist

### Header
- [x] Title "Penyiraman Otomatis" dengan gradient
- [x] Emergency Stop button merah dengan animasi pulsing
- [x] Responsive di semua ukuran

### Control Panel
- [x] Mode buttons (AUTO/MANUAL)
- [x] Active state dengan cyan glow
- [x] Current mode display

### Hero Metric
- [x] Large gauge dengan value besar
- [x] Background dengan gradient
- [x] Smooth animation

### Sensors Grid
- [x] 5 sensor cards
- [x] Progress bar gauge
- [x] Moisture percentage
- [x] Temperature display
- [x] Responsive grid

### Actuators
- [x] Pump icon + info + toggle
- [x] Valve icon + info + toggle
- [x] Status display (ON/OFF)
- [x] Green glow when active
- [x] Smooth toggle animation

### Status Info
- [x] Connection status (online/offline)
- [x] Current mode display
- [x] Last update time
- [x] Auto-update setiap detik

---

## 🎨 Visual Testing

### Dark Theme
- Background dark (gradient)
- Text light (white/cyan/green)
- Glassmorphism effect (blur + transparency)
- Soft shadows

### Animations
- [x] Slide-in animation saat load
- [x] Pulse animation emergency button
- [x] Smooth gauge fill animation (500ms)
- [x] Toggle switch smooth transition
- [x] Hover effects on buttons

### Glassmorphism
- [x] Backdrop blur 10px
- [x] Semi-transparent background
- [x] Glass border (1px, rgba)
- [x] Soft box-shadow

---

## 📊 Data Simulation

Server otomatis generate sensor data:
- Every 500ms sensor data berubah slightly
- Moisture: ±3% random change
- Temperature: ±0.5°C random change
- Average humidity auto-calculate
- Update dibcast ke semua connected clients

---

## 🚀 Performance Check

### Network
- Open DevTools (F12)
- Go to Network tab
- Monitor WebSocket messages (~2KB per update)
- Check latency

### CPU/Memory
- Task Manager → Performance
- Should be <5% CPU idle
- Memory ~30-50MB

### Responsiveness
- Toggle switch should respond immediately
- Mode change instant
- Gauge animation smooth (not stuttering)

---

## 🐛 Debugging

### Browser Console (F12)
```javascript
// Connected?
console.log(socket.connected);

// Send event
socket.emit('changeMode', {mode: 'MANUAL'});

// Listen event
socket.on('sensorUpdate', (data) => console.log(data));
```

### Server Logs
Watch terminal untuk:
```
✓ Client connected
✓ sensorUpdate
✓ modeChanged
✓ pumpChanged
✓ valveChanged
✗ Client disconnected
```

---

## 📱 Mobile Testing

Resize browser window:
- [x] Desktop (1600px+)
- [x] Tablet (768px+)
- [x] Mobile (480px)

Check:
- Layout respond correctly
- Touch-friendly buttons
- Scrollable on small screen
- All features accessible

---

## ✅ Final Checklist

- [ ] Dashboard load tanpa error
- [ ] Mode buttons work
- [ ] Emergency stop work
- [ ] Toggle switches work
- [ ] Real-time sync antar tab
- [ ] Gauge animations smooth
- [ ] Mobile responsive
- [ ] Dark theme looks good
- [ ] API endpoints respond
- [ ] WebSocket connected

---

**All tests passed? 🎉 Project ready for production!**
