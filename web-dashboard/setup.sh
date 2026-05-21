#!/bin/bash
# Setup script untuk Penyiraman Otomatis Dashboard
# Run: bash setup.sh

echo ""
echo "╔════════════════════════════════════════════╗"
echo "║  Penyiraman Otomatis - Setup Script       ║"
echo "║  Version 1.0                              ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Check if npm is installed
if ! command -v npm &> /dev/null; then
  echo "❌ npm tidak ditemukan!"
  echo "Silakan install Node.js terlebih dahulu dari https://nodejs.org"
  exit 1
fi

echo "✓ Node.js & npm ditemukan"
node --version
npm --version
echo ""

# Install dependencies
echo "📦 Installing dependencies..."
npm install

if [ $? -ne 0 ]; then
  echo "❌ Error saat install dependencies"
  exit 1
fi

echo "✓ Dependencies installed"
echo ""

# Build TypeScript
echo "🔨 Building TypeScript..."
npm run build

if [ $? -ne 0 ]; then
  echo "❌ Error saat build"
  exit 1
fi

echo "✓ TypeScript compiled successfully"
echo ""

# Done
echo "╔════════════════════════════════════════════╗"
echo "║  Setup Complete! ✨                        ║"
echo "╚════════════════════════════════════════════╝"
echo ""
echo "🚀 Start server dengan:"
echo "   npm start      (production)"
echo "   npm run dev    (development with watch)"
echo ""
echo "📱 Open browser:"
echo "   http://localhost:3000"
echo ""
echo "📚 Documentation:"
echo "   - README.md         (Full docs)"
echo "   - QUICKSTART.md     (Quick start)"
echo "   - TESTING.md        (Testing guide)"
echo ""
echo "Happy Farming! 🌱💧"
echo ""
