/*
  ESP32-C6 — Web Server
  =====================
  Hosts a control interface during operation.
  /durum endpoint returns status by default.
  When accessing /tetikle endpoint, it switches to destruction mode.

  Libraries:
    - Arduino IDE -> Board: "ESP32C6 Dev Module"
    - Arduino IDE -> Manage Libraries -> "Adafruit NeoPixel"
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// ============================================================
// AP (Hotspot) Settings — ESP32 creates its own Wi-Fi
// ============================================================
const char* ap_ssid     = "mede";
const char* ap_password = "12345678";
// In AP mode, ESP32 IP is always: 192.168.4.1
// ============================================================

// ESP32-C6 built-in RGB LED — GPIO 8
#define LED_PIN   8
#define LED_COUNT  1
Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

WebServer server(80);

// Status variable: "BEKLE" or "SIL"
String durum = "BEKLE";

// Set LED color
void ledColor(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

// ---- Endpoint: /durum ----
// Python polls this endpoint continuously
void handleDurum() {
  server.send(200, "text/plain", durum);
}

// ---- Endpoint: /tetikle ----
// When you visit this address from your phone, status becomes "SIL"
void handleTetikle() {
  durum = "SIL";
  ledColor(255, 0, 0); // Red — SIL mode
  server.send(200, "text/html",
    "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>"
    "body{margin:0;background:#000;color:#ff0000;font-family:'Courier New',monospace;display:flex;align-items:center;justify-content:center;min-height:100vh;}"
    ".box{text-align:center;padding:40px;border:2px solid #ff0000;box-shadow:0 0 30px #ff0000;max-width:500px;}"
    "h1{font-size:2em;text-transform:uppercase;letter-spacing:4px;animation:blink 0.5s step-end infinite;}"
    "p{color:#ff6666;font-size:1em;line-height:1.8;}"
    ".warn{color:#ffff00;font-size:0.85em;margin-top:20px;border-top:1px solid #ff0000;padding-top:15px;}"
    "@keyframes blink{0%,100%{opacity:1;}50%{opacity:0;}}"
    "</style></head>"
    "<body><div class='box'>"
    "<h1>&#9888; SYSTEM FAILURE &#9888;</h1>"
    "<p>[ DELETE COMMAND RECEIVED ]<br>"
    "Target files being destroyed...<br>"
    "Operation cannot be undone.</p>"
    "<p class='warn'>WARNING: This operation has been logged.</p>"
    "</div></body></html>"
  );
  Serial.println("[!] DELETE command sent!");
}

// ---- Endpoint: /sifirla ----
// Resets status if you want to run the demonstration again
void handleSifirla() {
  durum = "BEKLE";
  ledColor(0, 255, 0); // Green — BEKLE mode
  server.send(200, "text/html",
    "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>"
    "body{margin:0;background:#000;color:#00ff00;font-family:'Courier New',monospace;display:flex;align-items:center;justify-content:center;min-height:100vh;}"
    ".box{text-align:center;padding:40px;border:2px solid #00ff00;box-shadow:0 0 30px #00ff00;max-width:500px;}"
    "h1{font-size:1.8em;letter-spacing:3px;} p{color:#66ff66;}"
    "</style></head>"
    "<body><div class='box'>"
    "<h1>[ SYSTEM RESET ]</h1>"
    "<p>Status: Switched to BEKLE mode.<br>System is ready again.</p>"
    "<p><a href='/' style='color:#00ff00;'>&#8592; Return to Panel</a></p>"
    "</div></body></html>"
  );
  Serial.println("[*] Status reset: BEKLE");
}

// ---- Main Control Panel ----
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{margin:0;background:#000;color:#00ff00;font-family:'Courier New',monospace;min-height:100vh;display:flex;flex-direction:column;align-items:center;justify-content:center;padding:20px;box-sizing:border-box;}";
  html += ".terminal{border:1px solid #00ff00;padding:30px 40px;max-width:480px;width:100%;box-shadow:0 0 40px #00ff0055;}";
  html += ".title{font-size:1.1em;letter-spacing:6px;text-transform:uppercase;margin-bottom:4px;color:#00ff00;}";
  html += ".subtitle{font-size:0.7em;color:#007700;letter-spacing:3px;margin-bottom:30px;}";
  html += ".status{font-size:0.85em;color:#aaa;margin-bottom:30px;border:1px solid #333;padding:10px;}";
  html += ".status span{color:#ffff00;font-weight:bold;}";
  html += ".btn{display:block;width:100%;padding:16px;margin:10px 0;font-family:'Courier New',monospace;font-size:1em;font-weight:bold;letter-spacing:2px;cursor:pointer;text-decoration:none;text-align:center;box-sizing:border-box;border:none;}";
  html += ".red{background:#8b0000;color:#ff4444;border:1px solid #ff0000;box-shadow:0 0 15px #ff000066;}";
  html += ".red:hover{background:#ff0000;color:#fff;}";
  html += ".dim{background:#0a0a0a;color:#444;border:1px solid #222;font-size:0.8em;}";
  html += ".footer{margin-top:20px;font-size:0.65em;color:#333;letter-spacing:2px;}";
  html += "</style></head>";
  html += "<body><div class='terminal'>";
  html += "<div class='title'>[ CONTROL PANEL ]</div>";
  html += "<div class='subtitle'>ESP32-C6 // REMOTE ACCESS</div>";
  html += "<div class='status'>SYSTEM STATUS: <span>" + durum + "</span></div>";
  html += "<a href='/tetikle' class='btn red'>&gt;&gt; RUN DELETE COMMAND &lt;&lt;</a>";
  html += "<a href='/sifirla' class='btn dim'>[ RESET SYSTEM ]</a>";
  html += "<div class='footer'>UNAUTHORIZED ACCESS PROHIBITED</div>";
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize LED — blue while connecting
  led.begin();
  led.setBrightness(80);
  ledColor(0, 0, 255);

  Serial.println("\n[*] ESP32-C6 Starting...");
  Serial.println("[*] Opening AP (Access Point) mode...");

  WiFi.softAP(ap_ssid);

  ledColor(0, 255, 0); // Green — ready, BEKLE mode
  Serial.println("[✓] Hotspot created!");
  Serial.print("[✓] Network name: "); Serial.println(ap_ssid);
  Serial.print("[✓] Password  : "); Serial.println(ap_password);
  Serial.print("[✓] IP        : "); Serial.println(WiFi.softAPIP());
  Serial.println("[*] Connect devices to this network, open 192.168.4.1 in browser.\n");

  // Define endpoints
  server.on("/",        handleRoot);
  server.on("/durum",   handleDurum);
  server.on("/tetikle", handleTetikle);
  server.on("/sifirla", handleSifirla);

  server.begin();
  Serial.println("[✓] Web server started. Ready!\n");
}

void loop() {
  server.handleClient();
}
