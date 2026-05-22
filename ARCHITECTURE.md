# 📚 Dokumentasi Teknis Dashboard Penyiraman Otomatis

## 🏗️ Arsitektur Dashboard

### Arsitektur Umum

```
┌─────────────────────────────────────────────────────────────────┐
│                      USER INTERFACE (FRONTEND)                   │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  React + TypeScript + Vite                              │   │
│  │  - Modern Component-Based UI                            │   │
│  │  - Real-time Data Visualization                         │   │
│  │  - Responsive Glassmorphism Design                      │   │
│  └──────────────────────────────────────────────────────────┘   │
└──────────────────────┬──────────────────────────────────────────┘
                       │
           ┌───────────┴───────────┐
           │                       │
    ┌──────▼──────┐        ┌──────▼──────┐
    │  REST API   │        │ WebSocket   │
    │  (HTTP)     │        │ (Real-time) │
    └──────┬──────┘        └──────┬──────┘
           │                       │
┌──────────▼────────────────────────▼──────────────────────────┐
│                   BACKEND LAYER                               │
│  ┌────────────────────────────────────────────────────────┐   │
│  │  Node.js / Express Server                             │   │
│  │  - API Endpoints (/api/sensors, /api/control)         │   │
│  │  - WebSocket Server untuk Real-time Updates           │   │
│  │  - Authentication & Authorization                      │   │
│  └────────────────────────────────────────────────────────┘   │
└──────────────────────┬──────────────────────────────────────────┘
                       │
           ┌───────────┴───────────┐
           │                       │
    ┌──────▼──────┐        ┌──────▼──────┐
    │  MQTT/Comm  │        │ InfluxDB    │
    │  Protocol   │        │ (Time-Series│
    │  (ESP32)    │        │  Database)  │
    └──────┬──────┘        └──────┬──────┘
           │                       │
┌──────────▼────────────────────────▼──────────────────────────┐
│                   DATA & DEVICE LAYER                         │
│  ┌────────────────────────────────────────────────────────┐   │
│  │  ESP32 Microcontroller + Sensors                       │   │
│  │  - 3x Soil Moisture Sensors                            │   │
│  │  - Temperature Sensors                                 │   │
│  │  - Water Pump Control                                  │   │
│  │  - Solenoid Valve Control                              │   │
│  └────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

### Data Flow

```
1. SENSOR READING (ESP32)
   ┌──────────────────┐
   │ 3 Soil Sensors   │ → Read moisture values (0-100%)
   │ Temp Sensors     │ → Read temperature (°C)
   └────────┬─────────┘
            │
            ▼
2. DATA TRANSMISSION (MQTT/Serial)
   ┌──────────────────────────┐
   │ Send data every 1-5s      │ → MQTT Broker / Serial / HTTP
   │ {humidity, temp, time}    │
   └────────┬─────────────────┘
            │
            ▼
3. STORAGE (InfluxDB)
   ┌──────────────────────────┐
   │ Time-Series Database      │ → Store: timestamp, value, tags
   │ Retention: 30+ days       │
   └────────┬─────────────────┘
            │
            ▼
4. API RESPONSE (Node.js)
   ┌──────────────────────────┐
   │ Query from InfluxDB       │ → REST API endpoint
   │ Process & Format          │ → WebSocket broadcast
   └────────┬─────────────────┘
            │
            ▼
5. FRONTEND RENDERING (React)
   ┌──────────────────────────┐
   │ Receive JSON data         │ → Update state
   │ Animate components        │ → Render chart/gauge
   │ Display real-time values  │ → User sees live dashboard
   └──────────────────────────┘
```

---

## 🛠️ Framework & Technology Stack

### Frontend Layer

#### **React (UI Framework)**

**Apa itu React?**
- Library JavaScript untuk membangun user interface yang dinamis
- Component-based: Setiap bagian UI adalah "komponen" yang reusable
- Virtual DOM: Rendering ultra-cepat

**Alasan Memilih React:**

1. **Component Reusability** ✅
   ```tsx
   // Komponen Sensor dapat dipakai 3x untuk 3 sensor
   <SensorCard sensor={sensor1} />
   <SensorCard sensor={sensor2} />
   <SensorCard sensor={sensor3} />
   ```

2. **State Management** ✅
   ```tsx
   const [humidity, setHumidity] = useState(65);
   // Update otomatis ketika data berubah
   ```

3. **Real-time Updates** ✅
   - Mudah melakukan WebSocket integration
   - Auto-render ketika data berubah
   - No manual DOM manipulation

4. **Large Ecosystem** ✅
   - Chart libraries: Chart.js, Recharts, Victory
   - UI components: Material-UI, Chakra UI
   - State management: Redux, Zustand

5. **Developer Experience** ✅
   - Hot Module Replacement (HMR) dengan Vite
   - Easy debugging dengan React DevTools
   - TypeScript support built-in

**Analogi:**
- Tanpa React: Anda harus manual update setiap elemen HTML ketika data berubah
- Dengan React: Anda cukup update state, React otomatis update tampilan

---

#### **TypeScript**

**Apa itu TypeScript?**
- JavaScript + Static Type System
- Mendeteksi error sebelum runtime

**Alasan Memilih TypeScript:**

```tsx
// DENGAN TYPESCRIPT (Type-Safe) ✅
interface SensorData {
  id: number;
  humidity: number; // 0-100
  temp: number;     // Celsius
}

const processSensor = (data: SensorData): void => {
  if (data.humidity < 0 || data.humidity > 100) {
    throw new Error("Invalid humidity value");
  }
};

// TANPA TYPESCRIPT (Error-Prone) ❌
const processSensor = (data) => {
  // data bisa apa saja, bias error di production
  console.log(data.humidity);
};
```

**Keuntungan:**
1. **Error Detection** - Tangkap bug saat development
2. **Documentation** - Type menjadi dokumentasi kode
3. **Autocomplete** - IDE memberikan suggestion lebih baik
4. **Maintainability** - Kode lebih mudah dipahami tim

---

#### **Vite (Build Tool)**

**Apa itu Vite?**
- Build tool ultra-cepat untuk frontend
- Menggantikan Webpack/Create React App

**Alasan Memilih Vite:**

| Fitur | Webpack | Vite |
|-------|---------|------|
| Dev Start | 30-60 detik | 100ms |
| HMR | 1-2 detik | <100ms |
| Bundle Size | 500KB+ | 250KB |
| Config | Kompleks | Sederhana |

```bash
# Vite: Cepat dan ringan
npm run dev  # Instant start
npm run build # Optimized production bundle
```

**Keuntungan:**
1. **Blazing Fast Development** - Instant feedback loop
2. **ES Modules Native** - Modern JavaScript
3. **Optimized Build** - Smaller bundle untuk production
4. **Easy Config** - vite.config.ts minimal

---

### Backend Layer

#### **Node.js + Express**

**Apa itu Express?**
- Minimal web framework untuk Node.js
- Untuk membuat REST API dan WebSocket server

**Arsitektur Backend:**

```
EXPRESS SERVER
│
├── REST API Routes
│   ├── GET /api/sensors → Query InfluxDB → Return JSON
│   ├── POST /api/control → Update pump/valve
│   └── GET /api/history → Historical data
│
├── WebSocket Server
│   ├── Real-time sensor updates (1-5s interval)
│   ├── Live control feedback
│   └── Emergency alerts
│
└── Database Connection
    └── InfluxDB client → CRUD operations
```

**Alasan Memilih Node.js + Express:**

1. **JavaScript Full-Stack** ✅
   - Frontend dan Backend pakai JavaScript (TypeScript)
   - Same language = faster development
   - Code reuse (validation logic, types)

2. **Lightweight & Fast** ✅
   - Express minimal: ~40KB
   - Non-blocking I/O ideal untuk real-time
   - Handle banyak connections sekaligus

3. **Perfect untuk IoT** ✅
   ```javascript
   // MQTT Connection (dari ESP32)
   const mqtt = require('mqtt');
   const client = mqtt.connect('mqtt://broker.mosquitto.org');
   
   client.on('message', (topic, message) => {
     const data = JSON.parse(message);
     // Simpan ke InfluxDB
     // Broadcast ke WebSocket clients
   });
   ```

4. **Real-time Capabilities** ✅
   - WebSocket built-in support
   - Socket.io untuk live updates
   - Low latency (< 100ms)

---

### Communication Protocols

#### **1. MQTT (Publish-Subscribe)**

**Diagram Flow:**
```
ESP32 (Publisher)
   │
   └──► MQTT Broker ◄──┬─────────────────────────┐
                       │                         │
        ┌──────────────┴─────┐           (Subscribe)
        │                    │
   Node.js Backend    Other Devices
   (Consumer)
```

**Topik yang digunakan:**
```
penyiraman/sensor/1/humidity    → 65.5 (%)
penyiraman/sensor/1/temp        → 28.3 (°C)
penyiraman/sensor/2/humidity    → 72.1 (%)
penyiraman/sensor/2/temp        → 27.9 (°C)
penyiraman/sensor/3/humidity    → 58.6 (%)
penyiraman/sensor/3/temp        → 29.1 (°C)
penyiraman/control/pump         → ON/OFF
penyiraman/control/valve        → ON/OFF
```

**Alasan Memilih MQTT:**

| Aspek | MQTT | HTTP | Serial |
|-------|------|------|--------|
| Real-time | ✅ Excellent | ❌ Polling | ✅ Good |
| Bandwidth | ✅ Very Low | ❌ High | ✅ Low |
| Latency | ✅ <100ms | ❌ 1-5s | ✅ Instant |
| Reliability | ✅ QoS levels | ⚠️ Basic | ❌ No retry |
| Scale | ✅ 1M+ devices | ⚠️ Limited | ❌ Serial only |
| Setup | ✅ Easy | ⚠️ Medium | ❌ Complex |

**Keuntungan MQTT:**
1. **Lightweight** - Perfect untuk IoT devices dengan limited resources
2. **Publish-Subscribe** - Decoupled architecture
3. **QoS Levels** - Guarantee delivery
4. **Last Will Testament** - Detect device disconnect

```cpp
// CONTOH: ESP32 MQTT Publishing
#include <PubSubClient.h>

void publishSensorData() {
  String topic = "penyiraman/sensor/1/humidity";
  String payload = String(sensorValue);
  
  client.publish(topic.c_str(), payload.c_str());
  // Dikirim ke Backend dalam milliseconds
}
```

---

#### **2. WebSocket (Real-time)**

**Diagram:**
```
FIRST CONNECTION (HTTP Upgrade)
Client                          Server
  │                               │
  ├──── GET /ws (HTTP) ────────►  │
  │                               │
  │  ◄──── 101 Switching ────────┤
  │                               │
  ├──────► TCP Connection ◄──────┤
  │        (Persistent)          │
  │        (Bidirectional)        │
  │                               │
  └ DARI SINI: Instant messaging ─┘
```

**Alasan untuk Real-time Dashboard:**

1. **Instant Updates** - <100ms latency
2. **Bidirectional** - Server push, client listen
3. **Low Overhead** - Single connection reuse
4. **Perfect untuk Dashboard** - Live sensor values

```typescript
// FRONTEND: WebSocket Listener
const ws = new WebSocket('ws://localhost:3000/ws');

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  // {
  //   sensor: 1,
  //   humidity: 65.5,
  //   temp: 28.3,
  //   timestamp: "2026-05-22T10:30:00Z"
  // }
  
  setHumidity(data.humidity); // React state update
  // → Auto-render on dashboard ✨
};
```

---

## 🗄️ InfluxDB: Time-Series Database

### Apa itu InfluxDB?

**Definisi:**
- Database khusus untuk data **time-series** (data berubah seiring waktu)
- Optimized untuk data dengan timestamp
- Excellent untuk sensor dan monitoring

### Kenapa InfluxDB Cocok untuk Sistem Anda?

#### **1. Struktur Data Time-Series**

Sensor penyiraman = **data yang terus berubah seiring waktu**

```
Sensor 1 Humidity Over Time:
═════════════════════════════════════

65% ├─ Point (2026-05-22 10:00:00)
    │
70% ├─────── Point (2026-05-22 10:05:00)
    │    ╱
72% ├───╱───── Point (2026-05-22 10:10:00)
    │  ╱  
68% ├─────────── Point (2026-05-22 10:15:00)
    │
    └────────────────────────────►
    Time →
```

**Database lain (MySQL, PostgreSQL):**
- Row-based: Setiap nilai = 1 row
- 1000 sensor × 60 detik × 24 jam = 1.44 juta rows per hari
- Slow queries pada data besar

**InfluxDB:**
- Optimized untuk time-series
- Compression: 1 hari data ≈ 1MB (vs 50MB dengan MySQL)
- Query super cepat dengan aggregate functions

#### **2. Aggregate Functions (Perfect untuk Dashboard)**

```sql
-- Hitung rata-rata humidity per jam
SELECT MEAN(humidity) FROM sensor GROUP BY time(1h)
-- Result: 1 value per jam (vs 3600 raw values)

-- Hitung max temp per hari
SELECT MAX(temp) FROM sensor GROUP BY time(1d)

-- Hitung perubahan humidity (derivative)
SELECT DERIVATIVE(humidity) FROM sensor
-- Deteksi pompa sedang berjalan atau tidak

-- Hitung total downtime
SELECT COUNT(DISTINCT status) FROM sensor
-- Monitoring reliability
```

#### **3. Retention Policy (Otomatis Cleanup)**

```toml
# InfluxDB Config
[retention]
  # Keep raw data 7 days
  CREATE RETENTION POLICY raw ON penyiraman DURATION 7d REPLICATION 1
  
  # Keep 1-hour aggregates 30 days
  CREATE CONTINUOUS QUERY hourly_avg 
  ON penyiraman BEGIN
    SELECT MEAN(humidity) INTO hourly FROM sensor 
    GROUP BY time(1h)
  END
  
  # Keep 1-day aggregates 1 year
  CREATE RETENTION POLICY yearly ON penyiraman DURATION 365d
```

**Keuntungan:**
- ✅ Database tidak membengkak
- ✅ Otomatis cleanup old data
- ✅ Compliance dengan data retention policy

#### **4. Tag vs Field (Efficient Indexing)**

```
Measurement: sensor
├── Tags (indexed, for filtering)
│   ├── location: "pot_1"
│   ├── type: "humidity"
│   └── unit: "percent"
│
└── Fields (values, not indexed)
    ├── humidity: 65.5
    ├── temp: 28.3
    └── sensor_id: 1
```

**Query cepat:**
```sql
-- Fast (tag lookup)
SELECT humidity FROM sensor WHERE location = 'pot_1' AND time > now() - 1h

-- Slower (field scan)
SELECT * FROM sensor WHERE humidity > 60 AND time > now() - 1h
```

#### **5. Continuous Queries (Auto-Aggregation)**

```sql
-- Setiap 5 menit, hitung rata-rata
CREATE CONTINUOUS QUERY cq_5min ON penyiraman BEGIN
  SELECT MEAN(humidity), MEAN(temp) 
  INTO sensor_5m FROM sensor 
  GROUP BY time(5m), location
END
```

**Manfaat:**
- Reduce query load
- Dashboard lebih cepat
- Storage efficient

#### **6. Integration dengan Grafana (Visualization)**

InfluxDB bekerja seamlessly dengan Grafana:

```
InfluxDB (Data) → Grafana (Visualization) → Browser
      │                  │
      └─ Query data  ─┬──┘
                      │
              ┌───────┴────────┐
              │                │
          Real-time      Historical
           Dashboard    Analytics
```

---

## 📊 Perbandingan Database untuk Sistem Anda

| Feature | MySQL | PostgreSQL | InfluxDB |
|---------|-------|------------|----------|
| **Time-Series** | ❌ Bad | ⚠️ OK | ✅ Excellent |
| **Compression** | ❌ 50MB/day | ⚠️ 20MB/day | ✅ 1MB/day |
| **Query Speed** | ❌ Slow | ⚠️ Medium | ✅ Fast |
| **Aggregates** | ⚠️ Manual | ⚠️ Complex | ✅ Built-in |
| **Retention** | ❌ Manual | ❌ Manual | ✅ Automatic |
| **Real-time** | ❌ Bad | ⚠️ OK | ✅ Excellent |
| **Scale** | ⚠️ OK | ✅ Good | ✅ Excellent |
| **Setup** | ✅ Easy | ✅ Easy | ⚠️ Medium |

**Kesimpulan:** Untuk sistem monitoring sensor IoT → **InfluxDB adalah pilihan terbaik**

---

## 🔄 Complete Data Flow (End-to-End)

### Timeline: 1 Siklus Update

```
TIME 0:00
├── ESP32 reads sensors
│   ├── Sensor 1: 65.2% humidity, 28.3°C
│   ├── Sensor 2: 72.1% humidity, 27.9°C
│   └── Sensor 3: 58.6% humidity, 29.1°C
│
├── ESP32 publishes via MQTT
│   ├── Topic: penyiraman/sensor/1/humidity → Payload: 65.2
│   ├── Topic: penyiraman/sensor/1/temp → Payload: 28.3
│   └── ... (6 total messages)
│
├── MQTT Broker receives & distributes
│   └── Node.js backend (subscribed) receives all messages
│
├── Node.js backend processes
│   ├── Parse MQTT message
│   ├── Validate data
│   └── Write to InfluxDB
│
├── InfluxDB stores
│   ├── Point {
│   │   measurement: "sensor",
│   │   tags: {sensor_id: "1", location: "pot_1"},
│   │   fields: {humidity: 65.2, temp: 28.3},
│   │   timestamp: 1716379200000
│   │ }
│   └── Auto-aggregate (1-hour average, etc.)
│
├── WebSocket broadcast to clients
│   └── All connected browsers receive update
│
├── React Frontend receives
│   ├── Parse WebSocket message
│   ├── Update state (setHumidity, setTemp)
│   └── Trigger re-render
│
└── Browser displays
    ├── Circular progress bar updates (smooth animation)
    ├── Temperature display updates
    ├── Chart adds new data point
    └── User sees live dashboard (< 100ms latency)

TIME 0:00 → 0:05
└── Repeat every 1-5 seconds
```

---

## 🏆 Architecture Summary

### Why This Specific Stack?

| Component | Choice | Reason |
|-----------|--------|--------|
| **Frontend** | React + TS + Vite | Fast dev, reusable components, real-time ready |
| **Backend** | Node.js + Express | Same language as frontend, fast, IoT-friendly |
| **Communication** | MQTT | Lightweight, reliable, perfect for IoT |
| **Real-time** | WebSocket | <100ms latency, perfect for dashboard |
| **Database** | InfluxDB | Time-series optimized, compression, aggregates |

### Design Principles

1. **Scalability** - Add more sensors without rewriting code
2. **Real-time** - <100ms end-to-end latency
3. **Reliability** - Retry logic, data persistence
4. **Efficiency** - Low bandwidth, low storage
5. **Developer Experience** - Easy to understand and extend

---

## 📈 Future Scalability

### Current Setup (3 sensors)
```
Sensor reading: 1 per 5 seconds
Data points/day: 3 × 17,280 = 51,840
Storage/day: ~1MB
Queries/sec: < 1
```

### Scaled Up (100 sensors)
```
Sensor reading: 1 per 30 seconds
Data points/day: 100 × 2,880 = 288,000
Storage/day: ~5MB
Queries/sec: < 10
```

**InfluxDB can handle 1M+ data points/sec** ✅

---

## 🎓 Analogi Sederhana

### Visualisasi Arsitektur

```
┌─────────────────────────────────────────────────┐
│  DASHBOARD (React)                              │
│  "Menampilkan data dalam bentuk yang cantik"    │
└─────────────────────────┬───────────────────────┘
                          │
                     WebSocket
                     (<100ms)
                          │
┌─────────────────────────▼───────────────────────┐
│  BACKEND (Node.js + Express)                    │
│  "Menerima data, memproses, dan mendistribusi"  │
└─────────────────────────┬───────────────────────┘
                          │
                        MQTT
                     (Lightweight)
                          │
┌─────────────────────────▼───────────────────────┐
│  DATABASE (InfluxDB)                            │
│  "Menyimpan semua data sensor berdasarkan waktu"│
└─────────────────────────┬───────────────────────┘
                          │
                   Internal Query
                          │
┌─────────────────────────▼───────────────────────┐
│  IoT DEVICES (ESP32 + Sensors)                  │
│  "Mengumpulkan data dari dunia nyata"           │
└─────────────────────────────────────────────────┘
```

### Analogi Restoran

```
🍽️ DASHBOARD = Meja Tamu (menampilkan menu, pesanan, progress)
👨‍🍳 BACKEND = Dapur (memasak, mengolah, mendistribusi)
📦 DATABASE = Gudang (menyimpan ingredient, resep, riwayat)
🛒 IoT = Petani/Supplier (menyediakan ingredient segar)
```

---

## 📝 Kesimpulan

### Why React + TypeScript + Vite?
- **React**: Komponen reusable, state management, ecosystem
- **TypeScript**: Type-safety, better IDE, fewer bugs
- **Vite**: Ultra-fast dev experience, optimized bundle

### Why Node.js + Express?
- Same language full-stack
- Perfect for real-time IoT applications
- Lightweight dan scalable

### Why MQTT?
- Lightweight protocol
- Perfect untuk sensor dengan limited resources
- Publish-subscribe pattern ideal untuk IoT

### Why WebSocket?
- Real-time updates (<100ms)
- Persistent connection
- Perfect untuk live dashboard

### Why InfluxDB?
- Designed for time-series data
- Automatic compression (1MB/day vs 50MB with MySQL)
- Built-in aggregations
- Scalable to millions of data points

---

## 🔗 Resources

- [React Documentation](https://react.dev)
- [TypeScript Handbook](https://www.typescriptlang.org/docs/)
- [Vite Guide](https://vitejs.dev/guide/)
- [Express.js](https://expressjs.com/)
- [MQTT Protocol](https://mqtt.org/)
- [InfluxDB](https://www.influxdata.com/)
- [WebSocket API](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket)

---

**Last Updated:** May 22, 2026  
**Version:** 1.0.0  
**Status:** ✅ Complete
