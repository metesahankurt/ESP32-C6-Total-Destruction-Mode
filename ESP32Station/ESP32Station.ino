/*
  ESP32-C6 — Station Mode (Connect to Existing Wi-Fi)
  ===================================================
  ESP32 connects to the lab's existing Wi-Fi network.
  PC & ESP32 will be on the same network via the router — they can see each other.

  Usage:
    1. Fill in ssid and password with your lab Wi-Fi credentials
    2. Upload, read IP from Serial Monitor
    3. Update ESP32_IP in python_dinleyici.py with that IP
    4. Create exe, run it
    5. From phone (same Wi-Fi), visit IP address, press button

  Libraries:
    - Arduino IDE -> Board: "ESP32C6 Dev Module"
    - Arduino IDE -> Manage Libraries -> "Adafruit NeoPixel"
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// ============================================================
// Wi-Fi Credentials — Enter your lab's network info
// ============================================================
const char* ssid     = "FiberHGW_ZTE4FE";
const char* password = "Trabzon61";

// Static IP — adjust based on router subnet (router is usually 192.168.1.1)
IPAddress local_IP(192, 168, 1, 253);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
// ============================================================

// ESP32-C6 built-in RGB LED — GPIO 8
#define LED_PIN   8
#define LED_COUNT  1
Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

WebServer server(80);

String durum = "BEKLE";

void ledColor(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

// ---- Endpoint: /durum ----
void handleDurum() {
  server.send(200, "text/plain", durum);
}

// ---- Endpoint: /tetikle ----
void handleTetikle() {
  durum = "SIL";
  ledColor(255, 0, 0);
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
void handleSifirla() {
  durum = "BEKLE";
  ledColor(0, 255, 0);
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

  led.begin();
  led.setBrightness(80);
  ledColor(0, 0, 255); // Blue — connecting

  Serial.println("\n[*] ESP32-C6 Station Mode Starting...");
  Serial.print("[*] Connecting to Wi-Fi: ");
  Serial.println(ssid);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("[!] Static IP configuration failed, using DHCP.");
  }

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 30) {
      ledColor(255, 50, 0); // Orange — connection error
      Serial.println("\n[ERROR] Wi-Fi connection failed!");
      return;
    }
  }

  ledColor(0, 255, 0); // Green — ready
  Serial.println("\n[✓] Wi-Fi connected!");
  Serial.print("[✓] IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("[*] Connect your phone to the same Wi-Fi, visit this IP in browser.\n");

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
