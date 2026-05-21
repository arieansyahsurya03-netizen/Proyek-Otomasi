// Mock sensor data (replace with real API calls)
const mockSensorData = [
  { id: 1, humidity: 65 },
  { id: 2, humidity: 72 },
  { id: 3, humidity: 58 },
  { id: 4, humidity: 81 },
  { id: 5, humidity: 70 }
];

// State
let appState = {
  mode: 'AUTO',
  pumpOn: true,
  valveOn: false,
  sensors: mockSensorData,
  emergencyActive: false
};

// Initialize gauges
function initGauges() {
  document.querySelectorAll('.gauge-card').forEach((card, index) => {
    const sensor = appState.sensors[index];
    const svg = card.querySelector('.gauge');
    const value = card.querySelector('.gauge-value');
    
    value.textContent = `${sensor.humidity}%`;
    drawSemiCircularGauge(svg, sensor.humidity);
  });
  
  updateAverageHumidity();
}

// Draw semi-circular gauge
function drawSemiCircularGauge(svg, percentage) {
  svg.innerHTML = '';
  
  // Background arc
  const bgArc = createArc(60, 30, 45, 0, 180, '#475569', 0.2);
  svg.appendChild(bgArc);
  
  // Filled arc (0-180 degrees based on percentage)
  const angle = (percentage / 100) * 180;
  const filledArc = createArc(60, 30, 45, 0, angle, '#10b981', 1);
  svg.appendChild(filledArc);
  
  // Center text
  const text = document.createElementNS('http://www.w3.org/2000/svg', 'text');
  text.setAttribute('x', '60');
  text.setAttribute('y', '45');
  text.setAttribute('text-anchor', 'middle');
  text.setAttribute('dominant-baseline', 'middle');
  text.setAttribute('font-size', '18');
  text.setAttribute('font-weight', '700');
  text.setAttribute('fill', '#ffffff');
  text.textContent = `${percentage}%`;
  svg.appendChild(text);
}

// Create SVG arc path
function createArc(cx, cy, r, startAngle, endAngle, color, opacity) {
  const startRad = (startAngle - 90) * Math.PI / 180;
  const endRad = (endAngle - 90) * Math.PI / 180;
  
  const x1 = cx + r * Math.cos(startRad);
  const y1 = cy + r * Math.sin(startRad);
  const x2 = cx + r * Math.cos(endRad);
  const y2 = cy + r * Math.sin(endRad);
  
  const largeArc = endAngle - startAngle > 180 ? 1 : 0;
  
  const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
  const d = `M ${x1} ${y1} A ${r} ${r} 0 ${largeArc} 1 ${x2} ${y2}`;
  
  path.setAttribute('d', d);
  path.setAttribute('stroke', color);
  path.setAttribute('stroke-width', '8');
  path.setAttribute('fill', 'none');
  path.setAttribute('stroke-linecap', 'round');
  path.setAttribute('opacity', opacity);
  
  return path;
}

// Update average humidity
function updateAverageHumidity() {
  const avg = Math.round(
    appState.sensors.reduce((sum, s) => sum + s.humidity, 0) / appState.sensors.length
  );
  document.getElementById('avgValue').textContent = `${avg}%`;
}

// Emergency stop
document.getElementById('emergencyBtn').addEventListener('click', () => {
  appState.emergencyActive = !appState.emergencyActive;
  const btn = document.getElementById('emergencyBtn');
  
  if (appState.emergencyActive) {
    btn.style.opacity = '0.5';
    btn.style.pointerEvents = 'none';
    appState.pumpOn = false;
    appState.valveOn = false;
  } else {
    btn.style.opacity = '1';
    btn.style.pointerEvents = 'auto';
    appState.pumpOn = true;
  }
  
  updateActuators();
});

// Mode switching
document.getElementById('autoBtn').addEventListener('click', () => {
  appState.mode = 'AUTO';
  updateModeButtons();
});

document.getElementById('manualBtn').addEventListener('click', () => {
  appState.mode = 'MANUAL';
  updateModeButtons();
});

function updateModeButtons() {
  const autoBtn = document.getElementById('autoBtn');
  const manualBtn = document.getElementById('manualBtn');
  
  autoBtn.classList.toggle('active', appState.mode === 'AUTO');
  manualBtn.classList.toggle('active', appState.mode === 'MANUAL');
}

// Actuator status
function updateActuators() {
  // Water Pump
  const pumpIcon = document.getElementById('pumpIcon');
  const pumpStatus = document.getElementById('pumpStatus');
  pumpIcon.setAttribute('data-on', appState.pumpOn ? 'true' : 'false');
  pumpStatus.textContent = appState.pumpOn ? 'ON' : 'OFF';
  
  // Solenoid Valve
  const valveIcon = document.getElementById('valveIcon');
  const valveStatus = document.getElementById('valveStatus');
  valveIcon.setAttribute('data-on', appState.valveOn ? 'true' : 'false');
  valveStatus.textContent = appState.valveOn ? 'ON' : 'OFF';
}

// Simulate sensor updates (remove for real API)
function simulateSensorUpdates() {
  setInterval(() => {
    appState.sensors.forEach(sensor => {
      // Random small change
      sensor.humidity = Math.max(0, Math.min(100, sensor.humidity + (Math.random() - 0.5) * 8));
    });
    initGauges();
  }, 3000);
}

// Initialize
document.addEventListener('DOMContentLoaded', () => {
  initGauges();
  updateModeButtons();
  updateActuators();
  simulateSensorUpdates();
});
