@echo off
REM Penyiraman Otomatis Dashboard - Startup Script

title Penyiraman Otomatis Dashboard

cd /d "%~dp0"

echo.
echo ====================================
echo Penyiraman Otomatis Dashboard v1.0
echo ====================================
echo.

echo Installing dependencies if needed...
npm install

echo.
echo Building TypeScript...
npm run build

if %ERRORLEVEL% NEQ 0 (
  echo.
  echo ERROR: Build failed!
  pause
  exit /b 1
)

echo.
echo Starting server...
echo http://localhost:3000
echo.

npm start

pause
