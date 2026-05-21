# 📑 Project File Index

## 📂 Folder Structure

```
web-dashboard/
│
├── src/
│   └── index.ts              [SERVER] Express + Socket.IO + API
│
├── public/
│   ├── index.html            [UI] Dashboard HTML
│   ├── style.css             [UI] Glassmorphism CSS
│   └── app.js                [UI] Frontend logic + Socket.IO
│
├── dist/                     [AUTO] Compiled JavaScript (generated)
├── node_modules/             [AUTO] Dependencies (generated)
│
├── package.json              [CONFIG] Dependencies & scripts
├── tsconfig.json             [CONFIG] TypeScript configuration
├── .env                      [CONFIG] Environment variables
│
├── README.md                 [DOCS] Full documentation
├── QUICKSTART.md             [DOCS] Quick start (30 sec)
├── TESTING.md                [DOCS] Testing guide
├── PROJECT_SUMMARY.md        [DOCS] Project overview
│
├── ESP32_EXAMPLE.ino         [CODE] ESP32 Arduino example
├── setup.sh                  [SCRIPT] Setup script (Linux/Mac)
├── start.bat                 [SCRIPT] Startup script (Windows)
│
└── .gitignore                Git ignore file
```

---

## 🔑 Key Files Explained

### Backend (src/index.ts)
```typescript
// Main server file containing:
// - Express.js HTTP server
// - Socket.IO real-time communication
// - REST API endpoints
// - System state management
// - Sensor data simulation
```

### Frontend HTML (public/index.html)
```html
<!-- Dashboard components:
  - Header with title + emergency stop
  - Control panel (mode selector)
  - Hero metric (average humidity)
  - 5 sensor cards
  - Actuator controls (pump + valve)
  - System status info
-->
```

### Frontend CSS (public/style.css)
```css
/* Glassmorphism styling:
  - Dark theme background
  - Blur effect cards
  - Gradient text & borders
  - Responsive breakpoints
  - Smooth animations
  - Color variables
*/
```

### Frontend JS (public/app.js)
```javascript
// Dashboard logic:
// - Socket.IO client
// - UI update functions
// - Event listeners
// - Real-time data binding
// - Gauge animations
```

---

## 🚀 Quick Commands

```bash
# Setup
npm install          # Install dependencies once

# Development
npm run dev          # Watch + hot reload (dev mode)

# Production
npm run build        # Compile TypeScript
npm start            # Run server

# Maintenance
npm run clean        # Clean dist & node_modules
npm run build        # Build again
```

---

## 📱 Accessing Dashboard

### Local
```
http://localhost:3000
```

### Configure Port
Edit `.env`:
```env
PORT=3001  # Change from 3000
```

---

## 📡 API Endpoints Reference

| Endpoint | Method | Purpose |
|----------|--------|---------|
| `/` | GET | Load dashboard
| `/api/status` | GET | Get system state
| `/api/mode` | POST | Change mode (AUTO/MANUAL)
| `/api/pump` | POST | Control pump (ON/OFF)
| `/api/valve` | POST | Control valve (ON/OFF)
| `/api/emergency` | POST | Emergency stop
| `/api/esp32-data` | POST | Receive ESP32 data |

---

## 🔌 WebSocket Events

**Client → Server:**
- `changeMode` - Change operation mode
- `setPump` - Set pump state
- `setValve` - Set valve state
- `emergency` - Trigger emergency stop

**Server → Client:**
- `init` - Initial connection data
- `sensorUpdate` - Sensor data update
- `modeChanged` - Mode change notification
- `pumpChanged` - Pump state changed
- `valveChanged` - Valve state changed
- `emergency` - Emergency triggered

---

## 🎨 Color Scheme

```css
--primary:      #00d4ff   (Cyan - Main)
--secondary:    #ff006e   (Magenta - Accent)
--danger:       #ff0000   (Red - Warning/Stop)
--success:      #00ff41   (Green - Active)
--dark:         #0a0e27   (Dark bg)
--darker:       #050813   (Darker bg)
--text:         #ffffff   (Light text)
--text-dim:     #b0b8cc   (Dimmed text)
```

---

## 📊 Data Structure

### System State (server)
```typescript
{
  sensors: [
    { id, moisture, temperature, timestamp },
    ...
  ],
  averageHumidity: number,
  pump: boolean,
  valve: boolean,
  mode: 'AUTO' | 'MANUAL',
  lastUpdate: timestamp
}
```

### Sensor Data (from ESP32)
```json
{
  "sensors": [
    {"moisture": 65, "temperature": 28},
    ...
  ],
  "pump": false,
  "valve": false
}
```

---

## 🛠️ Development Workflow

1. **Edit backend** (`src/index.ts`)
   → Build: `npm run build`
   → Server auto-restart

2. **Edit frontend** (`public/*.js`, `*.css`, `*.html`)
   → Instant reload in browser

3. **Testing**
   → Open DevTools (F12)
   → Check Console & Network tab

---

## 📚 Documentation Files

| File | Content |
|------|---------|
| **README.md** | Full API docs + detailed features |
| **QUICKSTART.md** | 30-second setup guide |
| **TESTING.md** | Testing & verification guide |
| **PROJECT_SUMMARY.md** | High-level overview |
| **INDEX.md** (this file) | File reference guide |

---

## 🔐 Configuration (.env)

```env
PORT=3000                # Server port
HOST=localhost          # Server host
NODE_ENV=development    # dev or production
```

---

## 📦 Dependencies

### Production
- `express` - Web framework
- `socket.io` - Real-time communication
- `cors` - Cross-origin support
- `dotenv` - Environment variables

### Development
- `@types/express` - TypeScript types
- `@types/node` - Node.js types
- `typescript` - TypeScript compiler
- `concurrently` - Run multiple commands
- `@types/cors` - CORS types

---

## 🎯 Quick Troubleshooting

| Problem | Solution |
|---------|----------|
| Port 3000 in use | Change PORT in .env |
| Module not found | Run `npm install` |
| TypeScript error | Run `npm run build` |
| Server won't start | Check console for errors |
| WebSocket disconnected | Check server is running |
| Dashboard doesn't load | Check http://localhost:3000 |

---

## 🚀 Deployment Checklist

- [ ] Run `npm run build` - compile TypeScript
- [ ] Set `.env` with production values
- [ ] Change `NODE_ENV=production`
- [ ] Setup HTTPS/SSL
- [ ] Configure CORS for production domain
- [ ] Test all API endpoints
- [ ] Monitor server logs
- [ ] Setup database (optional)
- [ ] Setup authentication (optional)

---

## 📞 Quick Reference

**Start Development:**
```bash
npm run dev
```

**Start Production:**
```bash
npm run build && npm start
```

**Dashboard URL:**
```
http://localhost:3000
```

**Test API (Windows PowerShell):**
```powershell
Invoke-WebRequest 'http://localhost:3000/api/status'
```

**Stop Server:**
```
Press Ctrl+C
```

---

## 💡 Pro Tips

1. **Multi-client testing**: Open dashboard in 2 browser tabs to see real-time sync
2. **Mobile testing**: F12 → DevTools → Toggle device toolbar
3. **API testing**: Use cURL or Postman for HTTP requests
4. **WebSocket testing**: Use browser Console to emit events
5. **Performance**: DevTools → Network tab to monitor data flow

---

**Everything you need is documented. Happy coding! 🎉**
