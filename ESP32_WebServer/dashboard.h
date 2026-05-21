// dashboard.h - Embedded HTML/CSS/JS for ESP32 Web Dashboard
// Auto-generated - Do not edit manually

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>Penyiraman Otomatis</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{height:100%;overflow:hidden;font-family:'Segoe UI',system-ui,sans-serif;background:#080c18;color:#e0e6f0}

.app{height:100vh;display:flex;flex-direction:column;padding:12px;gap:10px}

/* Header */
.hdr{display:flex;justify-content:space-between;align-items:center;padding:10px 20px;
  background:rgba(255,255,255,.03);border:1px solid rgba(255,255,255,.06);border-radius:14px;flex-shrink:0}
.hdr h1{font-size:1rem;color:#00d4ff;font-weight:600}
.hdr .st{display:flex;align-items:center;gap:6px;font-size:.75rem;color:#6b7394}
.dot{width:8px;height:8px;border-radius:50%;background:#ff4444;transition:background .3s}
.dot.on{background:#00ff88}

/* Main area */
.main{flex:1;display:grid;grid-template-columns:1fr 1fr;grid-template-rows:1fr 1fr;gap:10px;min-height:0}

/* Cards */
.card{background:rgba(255,255,255,.025);border:1px solid rgba(255,255,255,.06);
  border-radius:16px;padding:14px;display:flex;flex-direction:column;overflow:hidden}
.card h2{font-size:.7rem;color:#00d4ff;text-transform:uppercase;letter-spacing:1.5px;margin-bottom:8px;flex-shrink:0}

/* Sensor gauges */
.sensors{grid-column:1/3}
.s-grid{display:flex;gap:10px;justify-content:center;align-items:center;flex:1}
.s-item{display:flex;flex-direction:column;align-items:center;gap:4px;flex:1;max-width:140px}
.s-item.inactive{opacity:.25}

/* SVG Gauge */
.gauge{position:relative;width:90px;height:90px}
.gauge svg{width:100%;height:100%;transform:rotate(-90deg)}
.g-bg{fill:none;stroke:rgba(255,255,255,.06);stroke-width:7}
.g-fill{fill:none;stroke:#00d4ff;stroke-width:7;stroke-linecap:round;
  stroke-dasharray:251.3;stroke-dashoffset:251.3;transition:stroke-dashoffset .8s ease,stroke .5s}
.g-center{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);text-align:center}
.g-pct{font-size:1.2rem;font-weight:700;color:#fff}
.g-lbl{font-size:.55rem;color:#6b7394}
.s-name{font-size:.65rem;color:#6b7394;font-weight:600}

/* Badge */
.badge{display:inline-block;padding:1px 8px;border-radius:10px;font-size:.55rem;font-weight:600}
.badge.kering{background:rgba(255,60,60,.15);color:#ff6b6b}
.badge.agak{background:rgba(255,170,0,.15);color:#ffaa00}
.badge.ideal{background:rgba(0,255,100,.15);color:#00ff88}
.badge.basah{background:rgba(0,150,255,.15);color:#00aaff}

/* Average card */
.avg-card{display:flex;flex-direction:column;align-items:center;justify-content:center}
.avg-num{font-size:3.5rem;font-weight:700;
  background:linear-gradient(135deg,#00d4ff,#00ff88);-webkit-background-clip:text;-webkit-text-fill-color:transparent;
  line-height:1}
.avg-lvl{margin-top:6px}

/* Controls card */
.ctrl-card{display:flex;flex-direction:column;gap:8px}
.act-row{display:flex;gap:8px;flex:1}
.act{flex:1;display:flex;align-items:center;gap:10px;padding:10px 14px;
  background:rgba(255,255,255,.02);border:1px solid rgba(255,255,255,.06);border-radius:12px;transition:all .3s}
.act.on{border-color:rgba(0,255,100,.25);background:rgba(0,255,100,.04)}
.act-icon{font-size:1.3rem}
.act-name{font-size:.75rem;font-weight:600}
.act-st{font-size:.65rem;color:#6b7394}
.act.on .act-st{color:#00ff88}

/* Mode buttons */
.mode-row{display:flex;gap:6px}
.mode-btn{flex:1;padding:8px;border:1px solid rgba(255,255,255,.08);border-radius:10px;
  background:transparent;color:#6b7394;font-size:.7rem;font-weight:600;cursor:pointer;transition:all .3s}
.mode-btn.active{background:rgba(0,212,255,.12);color:#00d4ff;border-color:rgba(0,212,255,.3)}

/* Status card */
.info-card{display:flex;flex-direction:column;justify-content:center;gap:6px}
.info-row{display:flex;justify-content:space-between;padding:6px 10px;
  background:rgba(255,255,255,.02);border-radius:8px;font-size:.72rem}
.info-row label{color:#6b7394}
.info-row span{color:#00d4ff;font-weight:600}

/* Emergency */
.emer{flex-shrink:0;padding:8px;background:linear-gradient(135deg,#ff0000,#cc0000);
  border:none;border-radius:10px;color:#fff;font-size:.75rem;font-weight:700;cursor:pointer;
  letter-spacing:1px;transition:all .3s;width:100%}
.emer:hover{box-shadow:0 0 20px rgba(255,0,0,.5);transform:scale(1.02)}

/* Responsive */
@media(max-width:768px){
  .main{grid-template-columns:1fr;grid-template-rows:auto auto auto auto}
  .sensors{grid-column:1}
  .gauge{width:60px;height:60px}
  .g-pct{font-size:.9rem}
  .avg-num{font-size:2.2rem}
  .app{padding:8px;gap:6px}
}
@media(max-height:600px){
  .gauge{width:65px;height:65px}
  .g-pct{font-size:.95rem}
  .avg-num{font-size:2.5rem}
  .hdr{padding:6px 14px}
  .card{padding:10px}
}
</style>
</head>
<body>
<div class="app">
  <!-- Header -->
  <div class="hdr">
    <h1>&#128167; Penyiraman Otomatis</h1>
    <div class="st"><div class="dot" id="dot"></div><span id="connTxt">Offline</span></div>
  </div>

  <div class="main">
    <!-- Sensors -->
    <div class="card sensors">
      <h2>Sensor Kelembaban Tanah</h2>
      <div class="s-grid" id="sGrid"></div>
    </div>

    <!-- Average -->
    <div class="card avg-card">
      <h2>Rata-rata</h2>
      <div class="avg-num" id="avgVal">--%</div>
      <div class="badge ideal avg-lvl" id="avgBadge">--</div>
    </div>

    <!-- Controls -->
    <div class="card ctrl-card">
      <h2>Kontrol Aktuator</h2>
      <div class="act-row">
        <div class="act" id="pumpAct">
          <div class="act-icon">&#128167;</div>
          <div><div class="act-name">Pompa</div><div class="act-st" id="pumpSt">OFF</div></div>
        </div>
        <div class="act" id="valveAct">
          <div class="act-icon">&#128688;</div>
          <div><div class="act-name">Valve</div><div class="act-st" id="valveSt">OFF</div></div>
        </div>
      </div>
      <div class="mode-row">
        <button class="mode-btn active" id="btnAuto" onclick="setMode('AUTO')">&#129302; AUTO</button>
        <button class="mode-btn" id="btnManual" onclick="setMode('MANUAL')">&#127918; MANUAL</button>
      </div>
      <button class="emer" onclick="emergency()">&#9888; EMERGENCY STOP</button>
    </div>

    <!-- Status -->
    <div class="card info-card">
      <h2>Informasi Sistem</h2>
      <div class="info-row"><label>Status</label><span id="infoConn">Offline</span></div>
      <div class="info-row"><label>Mode</label><span id="infoMode">AUTO</span></div>
      <div class="info-row"><label>IP Address</label><span id="infoIp">--</span></div>
      <div class="info-row"><label>Uptime</label><span id="infoUp">--</span></div>
      <div class="info-row"><label>Update</label><span id="infoTime">--</span></div>
    </div>
  </div>
</div>

<script>
// Build sensor gauge HTML
(function(){
  var g='';
  for(var i=1;i<=3;i++){
    g+='<div class="s-item'+(i>1?' inactive':'')+'" id="si'+i+'">'
      +'<div class="gauge"><svg viewBox="0 0 100 100">'
      +'<circle cx="50" cy="50" r="40" class="g-bg"/>'
      +'<circle cx="50" cy="50" r="40" class="g-fill" id="gf'+i+'"/>'
      +'</svg><div class="g-center"><div class="g-pct" id="gp'+i+'">--</div>'
      +'<div class="g-lbl">S'+i+'</div></div></div>'
      +'<div class="badge" id="gb'+i+'">--</div></div>';
  }
  document.getElementById('sGrid').innerHTML=g;
})();

var ws,connected=false;

function connect(){
  var host=location.hostname;
  ws=new WebSocket('ws://'+host+':81/');
  ws.onopen=function(){
    connected=true;
    document.getElementById('dot').className='dot on';
    document.getElementById('connTxt').textContent='Online';
    document.getElementById('infoConn').textContent='Online';
  };
  ws.onclose=function(){
    connected=false;
    document.getElementById('dot').className='dot';
    document.getElementById('connTxt').textContent='Offline';
    document.getElementById('infoConn').textContent='Offline';
    setTimeout(connect,2000);
  };
  ws.onerror=function(){ws.close()};
  ws.onmessage=function(e){
    try{var d=JSON.parse(e.data);update(d)}catch(x){}
  };
}

function update(d){
  // Sensors
  if(d.sensors){
    d.sensors.forEach(function(s){
      var i=s.id;
      var item=document.getElementById('si'+i);
      var fill=document.getElementById('gf'+i);
      var pct=document.getElementById('gp'+i);
      var bdg=document.getElementById('gb'+i);
      if(!item)return;

      if(s.active){
        item.classList.remove('inactive');
        var offset=251.3*(1-s.percent/100);
        fill.style.strokeDashoffset=offset;
        fill.style.stroke=getColor(s.percent);
        pct.textContent=s.percent+'%';
        bdg.textContent=s.level;
        bdg.className='badge '+getBadgeClass(s.level);
      }else{
        item.classList.add('inactive');
        pct.textContent='--';
        bdg.textContent='N/A';
        bdg.className='badge';
        fill.style.strokeDashoffset=251.3;
      }
    });
  }

  // Average
  document.getElementById('avgVal').textContent=d.average+'%';
  var ab=document.getElementById('avgBadge');
  ab.textContent=d.avgLevel||'--';
  ab.className='badge avg-lvl '+getBadgeClass(d.avgLevel);

  // Actuators
  var pa=document.getElementById('pumpAct');
  var va=document.getElementById('valveAct');
  pa.className='act'+(d.pump?' on':'');
  va.className='act'+(d.valve?' on':'');
  document.getElementById('pumpSt').textContent=d.pump?'ON':'OFF';
  document.getElementById('valveSt').textContent=d.valve?'ON':'OFF';

  // Mode
  document.getElementById('btnAuto').className='mode-btn'+(d.mode==='AUTO'?' active':'');
  document.getElementById('btnManual').className='mode-btn'+(d.mode==='MANUAL'?' active':'');
  document.getElementById('infoMode').textContent=d.mode;

  // Info
  if(d.ip)document.getElementById('infoIp').textContent=d.ip;
  if(d.uptime!==undefined){
    var m=Math.floor(d.uptime/60),s=d.uptime%60;
    document.getElementById('infoUp').textContent=m+'m '+s+'s';
  }
  document.getElementById('infoTime').textContent=new Date().toLocaleTimeString('id-ID');
}

function getColor(p){
  if(p<30)return'#ff4444';
  if(p<=45)return'#ffaa00';
  if(p<=70)return'#00ff88';
  return'#00aaff';
}
function getBadgeClass(l){
  if(!l)return'';
  if(l.indexOf('SANGAT')>=0)return'kering';
  if(l.indexOf('AGAK')>=0)return'agak';
  if(l.indexOf('IDEAL')>=0)return'ideal';
  return'basah';
}

function send(obj){if(ws&&connected)ws.send(JSON.stringify(obj))}
function setMode(m){send({action:'setMode',mode:m})}
function emergency(){if(confirm('EMERGENCY STOP?'))send({action:'emergency'})}

// Click actuators to toggle in MANUAL mode
document.getElementById('pumpAct').addEventListener('click',function(){
  send({action:'setPump',state:!document.getElementById('pumpSt').textContent.includes('ON')});
});
document.getElementById('valveAct').addEventListener('click',function(){
  send({action:'setValve',state:!document.getElementById('valveSt').textContent.includes('ON')});
});

connect();
</script>
</body>
</html>
)rawliteral";
