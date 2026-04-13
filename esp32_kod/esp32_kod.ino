/*
  ESP32-C6 Hack Demo — Web Sunucusu
  ==================================
  Sunum sırasında bir web sayfası yayınlar.
  /durum endpoint'i varsayılan olarak "BEKLE" döndürür.
  /tetikle endpoint'ine girilince "SIL" moduna geçer.

  Bağlantı:
    - ESP32-C6 ve bilgisayar aynı Wi-Fi ağında olmalı
    - IP adresi seri monitörden öğrenilir

  Kütüphane: Arduino IDE -> Board: "ESP32C6 Dev Module"
*/

#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// Wi-Fi Bilgileri — Kendi ağını yaz
// ============================================================
const char* ssid     = "FiberHGW_ZTE4FE";
const char* password = "Trabzon61";
// ============================================================

WebServer server(80);

// Durum değişkeni: "BEKLE" veya "SIL"
String durum = "BEKLE";

// ---- Endpoint: /durum ----
// Python bu endpoint'i sürekli sorgular
void handleDurum() {
  server.send(200, "text/plain", durum);
}

// ---- Endpoint: /tetikle ----
// Sen telefonundan bu adrese girince durum "SIL" olur
void handleTetikle() {
  durum = "SIL";
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

  Serial.println("\n[*] ESP32-C6 Hack Demo Başlatılıyor...");
  Serial.print("[*] Wi-Fi'ye bağlanılıyor: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Bağlantı bekleniyor
  int deneme = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    deneme++;
    if (deneme > 30) {
      Serial.println("\n[HATA] Wi-Fi bağlantısı kurulamadı!");
      return;
    }
  }

  Serial.println("\n[✓] Wi-Fi bağlandı!");
  Serial.print("[✓] IP Adresi: ");
  Serial.println(WiFi.localIP());
  Serial.println("[*] Tarayıcıdan bu IP'ye gir ve 'SİL' butonuna bas.\n");

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
