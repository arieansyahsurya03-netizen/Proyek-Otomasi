import express, { Express, Request, Response } from 'express';
import { createServer } from 'http';
import { Server as SocketIOServer, Socket } from 'socket.io';
import cors from 'cors';
import path from 'path';
import dotenv from 'dotenv';

dotenv.config();

const app: Express = express();
const httpServer = createServer(app);
const io = new SocketIOServer(httpServer, {
  cors: { origin: '*', methods: ['GET', 'POST'] }
});

const PORT = process.env.PORT || 3000;
const HOST = process.env.HOST || 'localhost';

// Middleware
app.use(cors());
app.use(express.json());

// Resolve public directory - works from both src/ (tsx dev) and dist/ (compiled)
const publicDir = path.join(process.cwd(), 'public');
app.use(express.static(publicDir));

// System State
interface Sensor {
  id: number;
  moisture: number;
  temperature: number;
  timestamp: number;
}

interface SystemState {
  sensors: Sensor[];
  averageHumidity: number;
  pump: boolean;
  valve: boolean;
  mode: 'AUTO' | 'MANUAL';
  lastUpdate: number;
}

let systemState: SystemState = {
  sensors: [
    { id: 1, moisture: 65, temperature: 28, timestamp: Date.now() },
    { id: 2, moisture: 72, temperature: 27, timestamp: Date.now() },
    { id: 3, moisture: 58, temperature: 29, timestamp: Date.now() }
  ],
  averageHumidity: 0,
  pump: false,
  valve: false,
  mode: 'AUTO',
  lastUpdate: Date.now()
};

// REST API Routes
app.get('/', (req: Request, res: Response) => {
  res.sendFile(path.join(publicDir, 'index.html'));
});

app.get('/api/status', (req: Request, res: Response) => {
  res.json(systemState);
});

app.post('/api/mode', (req: Request, res: Response) => {
  const { mode } = req.body;
  if (mode === 'AUTO' || mode === 'MANUAL') {
    systemState.mode = mode;
    io.emit('modeChanged', { mode });
    res.json({ success: true, mode });
  } else {
    res.status(400).json({ error: 'Invalid mode' });
  }
});

app.post('/api/pump', (req: Request, res: Response) => {
  const { state } = req.body;
  systemState.pump = state;
  io.emit('pumpChanged', { state });
  res.json({ success: true, state });
});

app.post('/api/valve', (req: Request, res: Response) => {
  const { state } = req.body;
  systemState.valve = state;
  io.emit('valveChanged', { state });
  res.json({ success: true, state });
});

app.post('/api/emergency', (req: Request, res: Response) => {
  systemState.pump = false;
  systemState.valve = false;
  io.emit('emergency');
  res.json({ success: true });
});

app.post('/api/esp32-data', (req: Request, res: Response) => {
  const { sensors, pump, valve } = req.body;
  
  if (Array.isArray(sensors)) {
    systemState.sensors = sensors.map((s: any, idx: number) => ({
      id: idx + 1,
      moisture: s.moisture || 0,
      temperature: s.temperature || 0,
      timestamp: Date.now()
    }));
  }
  
  if (typeof pump !== 'undefined') systemState.pump = pump;
  if (typeof valve !== 'undefined') systemState.valve = valve;
  
  systemState.lastUpdate = Date.now();
  io.emit('sensorUpdate', systemState);
  
  res.json({ success: true });
});

// Socket.IO
io.on('connection', (socket: Socket) => {
  console.log(`✓ Client connected: ${socket.id}`);
  socket.emit('init', systemState);

  socket.on('changeMode', (data) => {
    systemState.mode = data.mode;
    io.emit('modeChanged', data);
  });

  socket.on('setPump', (data) => {
    systemState.pump = data.state;
    io.emit('pumpChanged', data);
  });

  socket.on('setValve', (data) => {
    systemState.valve = data.state;
    io.emit('valveChanged', data);
  });

  socket.on('emergency', () => {
    systemState.pump = false;
    systemState.valve = false;
    io.emit('emergency');
  });

  socket.on('disconnect', () => {
    console.log(`✗ Client disconnected: ${socket.id}`);
  });
});

// Simulate sensor data
setInterval(() => {
  systemState.sensors = systemState.sensors.map(s => ({
    ...s,
    moisture: Math.max(0, Math.min(100, s.moisture + (Math.random() - 0.5) * 3)),
    temperature: s.temperature + (Math.random() - 0.5) * 0.5,
    timestamp: Date.now()
  }));
  
  systemState.lastUpdate = Date.now();
  io.emit('sensorUpdate', systemState);
}, 500);

// Start server
httpServer.listen(PORT, () => {
  console.log(`🚀 Server running at http://${HOST}:${PORT}`);
});
