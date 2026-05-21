import React, { useState, useEffect } from 'react';
import { io, Socket } from 'socket.io-client';

interface Sensor {
  id: number;
  moisture: number;
  temperature: number;
  timestamp?: number;
}

interface SystemState {
  sensors: Sensor[];
  averageHumidity: number;
  pump: boolean;
  valve: boolean;
  mode: 'AUTO' | 'MANUAL';
  lastUpdate: number;
}

const App: React.FC = () => {
  const [socket, setSocket] = useState<Socket | null>(null);
  const [isConnected, setIsConnected] = useState<boolean>(false);
  const [sensors, setSensors] = useState<Sensor[]>([]);
  const [pump, setPump] = useState<boolean>(false);
  const [valve, setValve] = useState<boolean>(false);
  const [mode, setMode] = useState<'AUTO' | 'MANUAL'>('AUTO');
  const [lastUpdateTime, setLastUpdateTime] = useState<string>('--:--:--');

  // Establish Socket.IO Connection
  useEffect(() => {
    const newSocket = io();
    setSocket(newSocket);

    newSocket.on('connect', () => {
      setIsConnected(true);
      console.log('Connected to WebSocket server');
    });

    newSocket.on('disconnect', () => {
      setIsConnected(false);
      console.log('Disconnected from WebSocket server');
    });

    newSocket.on('init', (data: SystemState) => {
      console.log('Init data received:', data);
      setSensors(data.sensors || []);
      setPump(data.pump);
      setValve(data.valve);
      setMode(data.mode);
      updateTime();
    });

    newSocket.on('sensorUpdate', (data: SystemState) => {
      setSensors(data.sensors || []);
      setPump(data.pump);
      setValve(data.valve);
      updateTime();
    });

    newSocket.on('modeChanged', (data: { mode: 'AUTO' | 'MANUAL' }) => {
      setMode(data.mode);
    });

    newSocket.on('pumpChanged', (data: { state: boolean }) => {
      setPump(data.state);
    });

    newSocket.on('valveChanged', (data: { state: boolean }) => {
      setValve(data.state);
    });

    newSocket.on('emergency', () => {
      setPump(false);
      setValve(false);
      setMode('MANUAL');
      console.log('EMERGENCY STOP received');
    });

    return () => {
      newSocket.disconnect();
    };
  }, []);

  // Update last update timestamp
  const updateTime = () => {
    const now = new Date();
    setLastUpdateTime(now.toLocaleTimeString('id-ID'));
  };

  // 1. hitung average moisture langsung di front-end!
  const averageMoisture = sensors.length > 0 
    ? Math.round(sensors.reduce((sum, s) => sum + s.moisture, 0) / sensors.length) 
    : 0;

  // Actions
  const handleModeChange = (newMode: 'AUTO' | 'MANUAL') => {
    if (socket) {
      socket.emit('changeMode', { mode: newMode });
    }
  };

  const handlePumpToggle = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (socket && mode === 'MANUAL') {
      socket.emit('setPump', { state: e.target.checked });
    }
  };

  const handleValveToggle = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (socket && mode === 'MANUAL') {
      socket.emit('setValve', { state: e.target.checked });
    }
  };

  const handleEmergencyStop = () => {
    if (socket && confirm('Trigger EMERGENCY STOP?')) {
      socket.emit('emergency');
    }
  };

  return (
    <div className="container">
      {/* HEADER */}
      <header className="header">
        <div className="header-left">
          <h1>💧 Penyiraman Otomatis</h1>
        </div>
        <button className="btn-emergency" onClick={handleEmergencyStop}>
          <span className="pulse"></span>
          EMERGENCY STOP
        </button>
      </header>

      {/* CONTROL PANEL */}
      <section className="card control-panel">
        <h2>Mode Kontrol</h2>
        <div className="mode-buttons">
          <button 
            className={`mode-btn ${mode === 'AUTO' ? 'active' : ''}`}
            onClick={() => handleModeChange('AUTO')}
          >
            🤖 AUTO
          </button>
          <button 
            className={`mode-btn ${mode === 'MANUAL' ? 'active' : ''}`}
            onClick={() => handleModeChange('MANUAL')}
          >
            🎮 MANUAL
          </button>
        </div>
        <p className="current-mode">Mode: <strong>{mode}</strong></p>
      </section>

      {/* AVERAGE HUMIDITY HERO */}
      <section className="card hero-metric">
        <h2>Rata-rata Kelembaban (Dihitung di Front-End)</h2>
        <div className="gauge-large">
          <div className="gauge-value">{averageMoisture}%</div>
          <p className="gauge-label">Kelembaban rata-rata dari {sensors.length} sensor</p>
        </div>
      </section>

      {/* SENSORS GRID */}
      <section className="sensors-grid">
        <h2>{sensors.length} Sensor Kelembaban</h2>
        <div className="cards-grid">
          {sensors.map((sensor) => (
            <div key={sensor.id} className="card sensor-card">
              <h3>Sensor {sensor.id}</h3>
              <div className="gauge-small">
                <div className="gauge-bar">
                  <div 
                    className="gauge-fill" 
                    style={{ width: `${sensor.moisture}%` }}
                  ></div>
                </div>
                <div className="gauge-info">
                  <span className="moisture">{Math.round(sensor.moisture)}%</span>
                  <span className="temp">{sensor.temperature.toFixed(1)}°C</span>
                </div>
              </div>
            </div>
          ))}
        </div>
      </section>

      {/* ACTUATORS */}
      <section className="card actuators">
        <h2>Status Aktuator</h2>
        <div className="actuator-list">
          {/* Pump */}
          <div className={`actuator-item ${pump ? 'active' : ''}`}>
            <div className="actuator-icon">💧</div>
            <div className="actuator-info">
              <h3>Pompa Air</h3>
              <p>{pump ? 'ON' : 'OFF'}</p>
            </div>
            <label className="toggle">
              <input 
                type="checkbox" 
                checked={pump} 
                onChange={handlePumpToggle}
                disabled={mode === 'AUTO'}
              />
              <span></span>
            </label>
          </div>

          {/* Valve */}
          <div className={`actuator-item ${valve ? 'active' : ''}`}>
            <div className="actuator-icon">🚰</div>
            <div className="actuator-info">
              <h3>Solenoid Valve</h3>
              <p>{valve ? 'ON' : 'OFF'}</p>
            </div>
            <label className="toggle">
              <input 
                type="checkbox" 
                checked={valve} 
                onChange={handleValveToggle}
                disabled={mode === 'AUTO'}
              />
              <span></span>
            </label>
          </div>
        </div>
      </section>

      {/* STATUS INFO */}
      <section className="card status-info">
        <h2>Informasi Sistem</h2>
        <div className="status-grid">
          <div className="status-item">
            <label>Status:</label>
            <span>{isConnected ? '🟢 Online' : '🔴 Offline'}</span>
          </div>
          <div className="status-item">
            <label>Mode:</label>
            <span>{mode}</span>
          </div>
          <div className="status-item">
            <label>Update:</label>
            <span>{lastUpdateTime}</span>
          </div>
        </div>
      </section>
    </div>
  );
};

export default App;
