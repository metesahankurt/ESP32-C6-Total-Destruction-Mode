/*
  ESP32-C6 Hack Demo — Web Sunucusu
  ==================================
  Sunum sırasında bir web sayfası yayınlar.
  /durum endpoint'i varsayılan olarak "BEKLE" döndürür.
  /tetikle endpoint'ine girilince "SIL" moduna geçer.

  Kütüphaneler:
    - Arduino IDE -> Board: "ESP32C6 Dev Module"
    - Arduino IDE -> Manage Libraries -> "Adafruit NeoPixel"
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// ============================================================
// AP (Hotspot) Ayarları — ESP32 kendi Wi-Fi'sini açar
// ============================================================
const char* ap_ssid     = "HACK_DEMO";
const char* ap_password = "12345678";
// AP modunda ESP32'nin IP'si her zaman: 192.168.4.1
// ============================================================

// ESP32-C6 dahili RGB LED — GPIO 8
#define LED_PIN   8
#define LED_SAYI  1
Adafruit_NeoPixel led(LED_SAYI, LED_PIN, NEO_GRB + NEO_KHZ800);

WebServer server(80);

// Durum değişkeni: "BEKLE" veya "SIL"
String durum = "BEKLE";

// LED rengini ayarla
void ledRenk(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

// ---- Endpoint: /durum ----
// Python bu endpoint'i sürekli sorgular
void handleDurum() {
  server.send(200, "text/plain", durum);
}

// ---- Endpoint: /tetikle ----
// Sen telefonundan bu adrese girince durum "SIL" olur
void handleTetikle() {
  durum = "SIL";
  ledRenk(255, 0, 0); // Kırmızı — SIL modu
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
    "<h1>&#9888; S&#304;STEM &#304;HLAL&#304; &#9888;</h1>"
    "<p>[ SIL KOMUTU ALINDI ]<br>"
    "Hedef dosyalar imha ediliyor...<br>"
    "&#304;&#351;lem geri al&#305;namaz.</p>"
    "<p class='warn'>UYARI: Bu i&#351;lem kay&#305;t alt&#305;na al&#305;nd&#305;.</p>"
    "</div></body></html>"
  );
  Serial.println("[!] SIL komutu gönderildi!");
}

// ---- Endpoint: /sifirla ----
// Sunumu tekrar yapmak istersen durumu sıfırlar
void handleSifirla() {
  durum = "BEKLE";
  ledRenk(0, 255, 0); // Yeşil — BEKLE modu
  server.send(200, "text/html",
    "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<style>"
    "body{margin:0;background:#000;color:#00ff00;font-family:'Courier New',monospace;display:flex;align-items:center;justify-content:center;min-height:100vh;}"
    ".box{text-align:center;padding:40px;border:2px solid #00ff00;box-shadow:0 0 30px #00ff00;max-width:500px;}"
    "h1{font-size:1.8em;letter-spacing:3px;} p{color:#66ff66;}"
    "</style></head>"
    "<body><div class='box'>"
    "<h1>[ S&#304;STEM S&#304;F&#304;RLANDI ]</h1>"
    "<p>Durum: BEKLE moduna ge&#231;ildi.<br>Sistem yeniden haz&#305;r.</p>"
    "<p><a href='/' style='color:#00ff00;'>&#8592; Panele D&#246;n</a></p>"
    "</div></body></html>"
  );
  Serial.println("[*] Durum sıfırlandı: BEKLE");
}

// ---- Ana Kontrol Paneli ----
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
  html += "<div class='title'>[ KONTROL PANELI ]</div>";
  html += "<div class='subtitle'>ESP32-C6 // UZAK ERISIM</div>";
  html += "<div class='status'>SISTEM DURUMU: <span>" + durum + "</span></div>";
  html += "<a href='/tetikle' class='btn red'>&gt;&gt; SIL KOMUTUNU CALISTIR &lt;&lt;</a>";
  html += "<a href='/sifirla' class='btn dim'>[ SISTEMI SIFIRLA ]</a>";
  html += "<div class='footer'>YETKISIZ ERISIM YASAKTIR</div>";
  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // LED başlat — bağlanırken mavi
  led.begin();
  led.setBrightness(80);
  ledRenk(0, 0, 255);

  Serial.println("\n[*] ESP32-C6 Hack Demo Başlatılıyor...");
  Serial.println("[*] AP modu açılıyor...");

  WiFi.softAP(ap_ssid);

  ledRenk(0, 255, 0); // Yeşil — hazır, BEKLE modu
  Serial.println("[✓] Hotspot açıldı!");
  Serial.print("[✓] Ağ adı : "); Serial.println(ap_ssid);
  Serial.print("[✓] Şifre  : "); Serial.println(ap_password);
  Serial.print("[✓] IP     : "); Serial.println(WiFi.softAPIP());
  Serial.println("[*] Cihazları bu ağa bağla, tarayıcıdan 192.168.4.1 aç.\n");

  // Endpoint'leri tanımla
  server.on("/",        handleRoot);
  server.on("/durum",   handleDurum);
  server.on("/tetikle", handleTetikle);
  server.on("/sifirla", handleSifirla);

  server.begin();
  Serial.println("[✓] Web sunucusu başlatıldı. Hazır!\n");
}

void loop() {
  server.handleClient();
}
