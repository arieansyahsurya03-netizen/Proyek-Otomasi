import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'path';

export default defineConfig({
  plugins: [react()],
  root: path.resolve(__dirname, 'src/frontend'),
  build: {
    outDir: path.resolve(__dirname, 'public'),
    emptyOutDir: true
  },
  server: {
    port: 3001,
    proxy: {
      '/api': 'http://localhost:3000',
      '/socket.io': {
        target: 'ws://localhost:3000',
        ws: true
      }
    }
  }
});
