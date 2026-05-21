# Penyiraman Otomatis — Automated Watering System Dashboard

A professional, high-fidelity web dashboard UI for an automated watering system with real-time sensor monitoring and control.

## Features

✨ **Modern Design**
- Glassmorphism aesthetic with soft shadows
- Nature-inspired color palette (emerald green, water blue, slate grey)
- Fully responsive design (mobile to 4K)
- Smooth animations and transitions

📊 **Sensor Monitoring**
- 5 soil moisture sensors with semi-circular gauges
- Real-time average humidity calculation
- Live data visualization

🎛️ **Control Panel**
- AUTO/MANUAL mode switching
- Emergency Stop button with visual feedback
- Water Pump and Solenoid Valve status indicators
- Glowing ON/OFF indicators with state management

## Project Structure

```
index.html       — Main HTML structure
css/styles.css   — All styling (glassmorphism, responsive, 4K)
js/app.js        — Interactivity (gauges, controls, state management)
```

## Quick Start

### Option 1: Open Locally (Simplest)
1. Navigate to the project folder
2. Right-click `index.html` → Open with Browser
   - Or drag `index.html` into your browser

### Option 2: Local HTTP Server (Recommended)

**Using Python:**
```bash
cd "D:\Project PPO"
python -m http.server 8000
```

Then visit: `http://localhost:8000`

**Using Node.js (http-server):**
```bash
npm install -g http-server
cd "D:\Project PPO"
http-server
```

**Using PowerShell:**
```powershell
cd "D:\Project PPO"
python -m http.server 8000
```

### Option 3: Vercel Deployment

Push to GitHub, then:
1. Go to [vercel.com](https://vercel.com)
2. Click "New Project"
3. Import your GitHub repository
4. Deploy

Your dashboard will be live at `your-domain.vercel.app`

## Configuration

### Mock Data
By default, the dashboard uses mock sensor data. To connect real sensors:

1. **Edit `js/app.js`** — Modify `mockSensorData`
2. **Add API endpoint** — Replace `simulateSensorUpdates()` with real API calls:

```javascript
async function fetchSensorData() {
  const response = await fetch('/api/sensors');
  appState.sensors = await response.json();
  initGauges();
}
```

3. **Update ESP32 WebServer** — Serve JSON from your Arduino:

```cpp
server.on("/api/sensors", HTTP_GET, [](AsyncWebServerRequest *request){
  String json = "{\"sensors\":[{\"id\":1,\"humidity\":65},...]}";
  request->send(200, "application/json", json);
});
```

## Responsive Breakpoints

- **Mobile**: < 480px
- **Tablet**: 480px – 768px
- **Desktop**: 768px – 2560px
- **4K**: ≥ 2560px

## Color Palette

| Color | Hex Value | Usage |
|-------|-----------|-------|
| Emerald | `#10b981` | Primary accent, gauges |
| Water Blue | `#0ea5e9` | Secondary accent |
| Slate Grey | `#475569` | Text, labels |
| Dark BG | `#0f172a` | Page background |
| Red | `#ef4444` | Emergency Stop |

## Browser Support

- Chrome/Edge ✅
- Firefox ✅
- Safari ✅
- Mobile browsers ✅

## Troubleshooting

### Dashboard looks broken on Vercel?
- **Check file paths**: Ensure `css/styles.css` and `js/app.js` are deployed
- **Clear cache**: Hard refresh (Ctrl+Shift+R or Cmd+Shift+R)
- **Check console**: Open DevTools (F12) for error messages

### Gauges not rendering?
- Ensure JavaScript is enabled
- Check browser console for errors
- Verify `js/app.js` is loaded and executed

### Responsive issues?
- Use browser DevTools to test different viewport sizes
- Check media queries in `css/styles.css`

## Performance Notes

- Lightweight: No external frameworks (no React, Vue, etc.)
- Fast loading: Pure HTML/CSS/JS
- Optimized: SVG gauges render efficiently
- Smooth: 60fps animations on modern devices

## Future Enhancements

- [ ] Dark/Light theme toggle
- [ ] Historical data charts (Chart.js)
- [ ] Watering schedule configuration
- [ ] Push notifications
- [ ] Multi-user authentication
- [ ] Export sensor logs (CSV)
- [ ] WebSocket real-time updates

## License

MIT — Feel free to use and modify.

---

**Need Help?**
- Check browser console (F12) for errors
- Verify all files are in correct folders
- Test on different devices/browsers
