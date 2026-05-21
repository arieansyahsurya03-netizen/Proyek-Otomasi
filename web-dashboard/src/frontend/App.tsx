import React, { useState, useEffect, useRef } from 'react';
import './style.css';

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

// Demo data generator for when no backend is available (e.g. Vercel)
const generateDemoSensors = (prev: Sensor[]): Sensor[] => {
  if (prev.length === 0) {
    return [
      { id: 1, moisture: 65, temperature: 28.2, timestamp: Date.now() },
      { id: 2, moisture: 72, temperature: 27.1, timestamp: Date.now() },
      { id: 3, moisture: 58, temperature: 29.5, timestamp: Date.now() },
    ];
  }
  return prev.map(s => ({
    ...s,
    moisture: Math.max(0, Math.min(100, s.moisture + (Math.random() - 0.5) * 4)),
    temperature: Math.max(20, Math.min(40, s.temperature + (Math.random() - 0.5) * 0.5)),
    timestamp: Date.now(),
  }));
};

const App: React.FC = () => {
  const [isConnected, setIsConnected] = useState<boolean>(false);
  const [isDemoMode, setIsDemoMode] = useState<boolean>(false);
  const [sensors, setSensors] = useState<Sensor[]>([]);
  const [pump, setPump] = useState<boolean>(false);
  const [valve, setValve] = useState<boolean>(false);
  const [mode, setMode] = useState<'AUTO' | 'MANUAL'>('AUTO');
  const [lastUpdateTime, setLastUpdateTime] = useState<string>('--:--:--');
  const socketRef = useRef<any>(null);
  const demoIntervalRef = useRef<ReturnType<typeof setInterval> | null>(null);

  // Try Socket.IO connection, fall back to demo mode
  useEffect(() => {
    let connectionTimeout: ReturnType<typeof setTimeout>;

    const tryConnect = async () => {
      try {
        // Dynamically import socket.io-client so Vercel doesn't crash if no server
        const { io } = await import('socket.io-client');
        const newSocket = io(window.location.origin, {
          timeout: 3000,
          reconnectionAttempts: 2,
          reconnectionDelay: 1000,
        });

        socketRef.current = newSocket;

        newSocket.on('connect', () => {
          setIsConnected(true);
          setIsDemoMode(false);
          // Stop demo simulation if running
          if (demoIntervalRef.current) {
            clearInterval(demoIntervalRef.current);
            demoIntervalRef.current = null;
          }
          console.log('Connected to WebSocket server');
        });

        newSocket.on('disconnect', () => {
          setIsConnected(false);
          console.log('Disconnected from WebSocket server');
        });

        newSocket.on('init', (data: SystemState) => {
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
        });

        // If not connected within 4 seconds, switch to demo mode
        connectionTimeout = setTimeout(() => {
          if (!newSocket.connected) {
            console.log('Server unreachable — switching to Demo Mode');
            newSocket.disconnect();
            startDemoMode();
          }
        }, 4000);

      } catch (err) {
        console.log('Socket.IO import/connection failed — starting Demo Mode');
        startDemoMode();
      }
    };

    const startDemoMode = () => {
      setIsDemoMode(true);
      setIsConnected(false);
      setSensors(generateDemoSensors([]));
      updateTime();

      demoIntervalRef.current = setInterval(() => {
        setSensors(prev => generateDemoSensors(prev));
        updateTime();
      }, 2000);
    };

    tryConnect();

    return () => {
      clearTimeout(connectionTimeout);
      if (socketRef.current) socketRef.current.disconnect();
      if (demoIntervalRef.current) clearInterval(demoIntervalRef.current);
    };
  }, []);

  const updateTime = () => {
    const now = new Date();
    setLastUpdateTime(now.toLocaleTimeString('id-ID'));
  };

  // Hitung average moisture langsung di front-end
  const averageMoisture = sensors.length > 0
    ? Math.round(sensors.reduce((sum, s) => sum + s.moisture, 0) / sensors.length)
    : 0;

  // Auto pump logic in demo mode
  useEffect(() => {
    if (isDemoMode && mode === 'AUTO') {
      if (averageMoisture < 45) {
        setPump(true);
        setValve(true);
      } else if (averageMoisture >= 50) {
        setPump(false);
        setValve(false);
      }
    }
  }, [averageMoisture, isDemoMode, mode]);

  // Actions
  const handleModeChange = (newMode: 'AUTO' | 'MANUAL') => {
    if (socketRef.current && isConnected) {
      socketRef.current.emit('changeMode', { mode: newMode });
    }
    setMode(newMode);
  };

  const handlePumpToggle = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (mode === 'MANUAL') {
      const state = e.target.checked;
      if (socketRef.current && isConnected) {
        socketRef.current.emit('setPump', { state });
      }
      setPump(state);
    }
  };

  const handleValveToggle = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (mode === 'MANUAL') {
      const state = e.target.checked;
      if (socketRef.current && isConnected) {
        socketRef.current.emit('setValve', { state });
      }
      setValve(state);
    }
  };

  const handleEmergencyStop = () => {
    if (window.confirm('Trigger EMERGENCY STOP?')) {
      if (socketRef.current && isConnected) {
        socketRef.current.emit('emergency');
      }
      setPump(false);
      setValve(false);
      setMode('MANUAL');
    }
  };

  return (
    <div className="container">
      {/* DEMO MODE BANNER */}
      {isDemoMode && (
        <div className="demo-banner">
          📡 Mode Demo — Data simulasi ditampilkan. Hubungkan ke ESP32 lokal untuk data real-time.
        </div>
      )}

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
            <span>{isConnected ? '🟢 Online' : isDemoMode ? '🟡 Demo' : '🔴 Offline'}</span>
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
